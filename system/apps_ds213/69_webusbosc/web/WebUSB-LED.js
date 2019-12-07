
  var canvas = new Renderer(800, 600);


  function Redraw(rawdata)
  {
    var data = [];
    for (var i =0; i<rawdata.length; i+=5)
    {
      var s = parseInt("0x" + rawdata.substr(i, 5));
      data.push([s&255, (s>>8)&255, (s>>16)&1, (s>>17)&1]);
    }

    var path1 = [];
    for (var i=0; i<data.length; i++)
      path1.push({x:i, y:600-data[i][0]*3});

    var path2 = [];
    for (var i=0; i<data.length; i++)
      path2.push({x:i, y:600-data[i][1]*3});

    canvas.Clear();
    canvas.Poly(path1, "#ffff00");
    canvas.Poly(path2, "#00ffff");
  }

  var init = false;
  var dummy = false;
  var promise = null;
  setInterval(() =>
  {
    if (COMM._open)
    {
      if (promise)
        return;

      if (!dummy)
      {
        /// first request is without response?
        promise = BIOS.biosMemGetBufferPtr();
        setTimeout(() => promise = false, 300);
        dummy = true;
        return;
      }

      if (!init)
      {
        promise = Promise.resolve()
          .then( () => OSC.ConfigureTrigger(0, 0, 8, 0) )
          .then( () => OSC.ConfigureInput(OSC.Enums["CH1"], OSC.Enums["DC"], OSC.Enums["500mV"], 50) )
          .then( () => OSC.ConfigureTimebase("10e-3") )
          .then( () => OSC.Enable(1) )
          .then( () => promise = false );
        init = true;
        return;
      }

      promise = Promise.resolve()
//        .then( () => OSC.Ready() )
        .then( () => true )
        .then( (ready) =>  
        { 
          if (ready)
          {
            return OSC.Transfer(150, 512)
            .then( data => Redraw(data) )
            .then( () => Promise.resolve() )
            .catch( () => Promise.resolve() );
          }
          return Promise.resolve();
        })
        .then( () => promise = false );
    }
  }, 100);








///////////


  document.addEventListener('DOMContentLoaded', event => {
    let connectButton = document.querySelector("#connect");
    let statusDisplay = document.querySelector('#status');
    let port;

    let ascii = (s => s.split('').map(c=>c.charCodeAt(0)) );
    
    function connect() {

COMM._send = function(msg)
{
  if (COMM.debug)
    console.log("> " + msg);
  port.send(Uint8Array.from(ascii(msg)));
}

      port.connect().then(() => {
        COMM._open = true;

        statusDisplay.textContent = '';
        connectButton.textContent = 'Disconnect';

        port.onReceive = data => {
          let textDecoder = new TextDecoder();
          let msg= textDecoder.decode(data)

          if (COMM.debug)
            console.log("< " + msg);

          COMM._onReceive(msg);
        }
        port.onReceiveError = error => {
          console.error(error);
        };
      }/*, error => {
        statusDisplay.textContent = error;
      }*/);
    }

    function onUpdateLed() {
      if (!port) {
        return;
      }
      toggle();     
    };

    connectButton.addEventListener('click', function() {
      if (port) {
        port.disconnect();
        COMM._open = false;
        connectButton.textContent = 'Connect';
        statusDisplay.textContent = '';
        port = null;
      } else {
        serial.requestPort().then(selectedPort => {
          port = selectedPort;
          connect();
        }).catch(error => {
          statusDisplay.textContent = error;
        });
      }
    });

    serial.getPorts().then(ports => {
      if (ports.length == 0) {
        statusDisplay.textContent = 'No device found.';
      } else {
        statusDisplay.textContent = 'Connecting...';
        port = ports[0];
        connect();
      }
    });
  });
