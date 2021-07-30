var COMM = {
  debug: true,
  _open: false,
  _send: () => 0,
  _defReceive: (data) => {
    var msg = new TextDecoder().decode(data);
    COMM._defEval(msg)
  },
  _defEval: (msg) => 
  {
    if (msg.indexOf("_DBGPRINT('<script>") == 0)
    {
      var inside = msg.match("<script>(.*)</script>");
      if (inside)
        setTimeout(inside[1], 0);
      return;
    }
    if (msg.indexOf("_DBGPRINT(") == 0 || msg.indexOf("_DBGEVENT(") == 0)
      setTimeout(msg, 0);
  },
  _onReceive: () => 0,

  setDisconnected: () => 0,
  setConnected: () => 0,
  setConnecting: () => 0,
  setNoDevices: () => 0,
  setConnectFailed: () => 0,
  doConnect: () => 0
};

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
