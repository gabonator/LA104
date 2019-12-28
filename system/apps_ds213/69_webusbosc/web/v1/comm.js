var COMM = {
  debug: false,
  _open: false,
  _send: () => 0,
  _onReceive: () => 0
};


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
  if (msg.indexOf("undefined") != -1) 
  { 
    console.log(">>>> ignoring command >>>> " + msg);
    return;
  }
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
