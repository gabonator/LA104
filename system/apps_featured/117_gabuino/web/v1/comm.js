_defEvalBuf = [];

var COMM = {
  debug: false, //true, //true,
  _open: false,
  _send: () => 0,
  _defReceive: (data) => {
    var msg = new TextDecoder().decode(data);
    COMM._defEval(msg)
  },
  _defEval: (msg) => 
  {
    var _msg = msg.split("\n").join("\\\\n");
    var _match = _msg.match("_DBGEVENT\\((.*)\\)")
    if (_match)
    {
       eval(_msg);
    }
    var _match = _msg.match("_DBGPRINT\\((.*)\\)")
    if (_match)
    {
      var cont = _match[1].split("\\\\n").join("\n");
      if (cont.substr(0, 1) == cont.substr(-1) && (cont[0] == "\"" || cont[0] == "'" || cont[0] == "`"))
      {
        evalRaw(cont.substr(1, cont.length-2));
      } else
        throw "Wrong format";
    }
/*
    if (msg.indexOf("_DBGPRINT('<script src=") == 0 || msg.indexOf("_DBGPRINT(`<script src=") == 0)
    {
      var inside = msg.split("\n").join("<_br_>").match("<script src=\"(.*)\"></script>");
      if (inside)
      {
        console.log("Embedding head: " + inside[1]);
        var script = document.createElement("script");  
        script.src = inside[1]; 
        document.head.appendChild(script); 
      } else
        throw "Cannot find src";
      return;
    }
    if (msg.indexOf("_DBGPRINT('<script>") == 0 || msg.indexOf("_DBGPRINT(`<script>") == 0)
    {
      var inside = msg.split("\n").join("<_br_>").match("<script>(.*)</script>");
      if (inside)
        setTimeout(inside[1].split("<_br_>").join("\n"), 0);
      else
        throw "No match";
      return;
    }
    if (msg.indexOf("_DBGPRINT(") == 0 || msg.indexOf("_DBGEVENT(") == 0)
    {
      console.log("Executing global: " + msg);
      setTimeout(msg, 0);
    }
*/
  },
  _onReceive: () => 0,

  setDisconnected: () => 0,
  setConnected: () => 0,
  setConnecting: () => 0,
  setNoDevices: () => 0,
  setConnectFailed: () => 0,
  doConnect: () => 0
};


function evalRaw(msg)
{
//  var promises = [];
  msg = msg.split("\n").join("\\\\n");

  var ranges = [];
  for (match of msg.matchAll("<script.*?/script>"))
    ranges.push([match.index, match[0].length, match[0]]);

  ranges.reverse();
  for (var i in ranges)
  { 
    var range = ranges[i];
    msg = msg.substr(0, range[0]) + msg.substr(range[0] + range[1]);
  }

  msg = msg.split("\\\\n").join("\n");

  _DBGPRINT(msg);

//  var head = document.getElementsByTagName('head')[0];
  var promise = Promise.resolve();

  var head = document.head;

  ranges.reverse();
  for (var i in ranges)
  {
    var script = ranges[i][2];
    var content = script.match('<script type="module">(.*)</scri'+'pt>');
    if (content)
    {
      console.log("load module: " + content[1])
      content = content[1].split("\\\\n").join("\n");
      promise = promise.then( () => new Promise( (resolve) =>
      {
        var script = document.createElement('script');
        script.type = 'module';
        script.innerHTML = content;
        script.onload = resolve;
        head.appendChild(script);
      }));
      continue;
    }

    var content = script.match('<script>(.*)</scri'+'pt>');
    if (content)
    {
      console.log("eval script: " + content[1])
      content = content[1].split("\\\\n").join("\n");
/*
      var script = document.createElement('script');
      script.innerHTML = content;
      head.appendChild(script);
*/
//      promise = promise.then( () => new Promise( (resolve) =>
      promise = promise.then( (function() { 

        return new Promise( (resolve) =>
          {
            eval(content);
            resolve();
          });

        }).bind({content:content}) );

      continue;
    }

    var content = script.match('<script src="(.*)"></scri'+'pt>');
    if (content)
    {
      console.log("load script: " + content[1])
      promise = promise.then( (function() { 

          return new Promise( (resolve) =>
          {
            var content = this.content;
            var scr = document.createElement('script');
            scr.onload = () => { 
    console.log("script loaded!"); 
    resolve(); }
            scr.src = content;
    //        script.async = false; 
            head.appendChild(scr);
          } )

        }).bind({content:content[1]}) );
      continue;
    }
    throw "Unable to process script tag"
  }

  return promise;
//console.log(msg);

}



COMM._onReceive = COMM._defReceive;


  document.addEventListener('DOMContentLoaded', event => {
//    let connectButton = document.querySelector("#connect");
//    let statusDisplay = document.querySelector('#status');
    let port;

    let ascii = (s => s.split('').map(c=>c.charCodeAt(0)) );
    
    function connect() {

COMM._send = function(msg)
{
  if (COMM.debug)
    console.log("> " + msg);
  if (msg.indexOf("undefined") != -1) 
  { 
    console.log(">>>> ignoring command >>>> " + msg);
    return;
  }
  port.send(Uint8Array.from(ascii(msg)));
}
COMM._sendRaw = function(msg)
{
  port.send(Uint8Array.from(msg));
}

      port.connect().then(() => {
        COMM._open = true;

        COMM.setConnected();

        port.onReceive = data => {
          if (COMM.debug)
          {
            let msg = new TextDecoder().decode(data)
            console.log("< " + msg);
          }
          COMM._onReceive(data);
        }
        port.onReceiveError = error => {
          console.error(error);
        };
      }, error => {
        COMM.setConnectFailed(error);
//        statusDisplay.textContent = error;
      });
    }

    function onUpdateLed() {
      if (!port) {
        return;
      }
      toggle();     
    };

    COMM.doConnect = () => {
      if (port) {
        port.disconnect();
        COMM._open = false;
        COMM.setDisconnected();
//        connectButton.textContent = 'Connect';
//        statusDisplay.textContent = '';
        port = null;
      } else {
        serial.requestPort().then(selectedPort => {
          port = selectedPort;
          connect();
        }).catch(error => {
          //statusDisplay.textContent = error;
          COMM.setConnectFailed(error);
        });
      }
    };

    serial.getPorts().then(ports => {
      if (ports.length == 0) {
        COMM.setNoDevices();

//        statusDisplay.textContent = 'No device found.';
      } else {
        COMM.setConnecting();

//        statusDisplay.textContent = 'Connecting...';
        port = ports[0];
        connect();
      }
    });
  });
