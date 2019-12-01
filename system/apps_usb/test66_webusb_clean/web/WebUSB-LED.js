(function() {
  'use strict';
 
  document.addEventListener('DOMContentLoaded', event => {
    let connectButton = document.querySelector("#connect");
    let statusDisplay = document.querySelector('#status');
    let ledCheckbox = document.querySelector('#led');
    let butCheckbox = document.querySelector('#but');
    let port;

    let ascii = (s => s.split('').map(c=>c.charCodeAt(0)) );
    
    function connect() {
      port.connect().then(() => {
        statusDisplay.textContent = '';
        connectButton.textContent = 'Disconnect';

        port.onReceive = data => {
          let textDecoder = new TextDecoder();
          let msg= textDecoder.decode(data)
          console.log(msg);
          if( msg.indexOf("but=0")>=0 ) { butCheckbox.checked=false; ledCheckbox.checked=false }
          if( msg.indexOf("but=1")>=0 ) { butCheckbox.checked=true;  ledCheckbox.checked=true  }
        }
        port.onReceiveError = error => {
          console.error(error);
        };
      }, error => {
        statusDisplay.textContent = error;
      });
    }

    function onUpdateLed() {
      if (!port) {
        return;
      }
      let cmd = ledCheckbox.checked ? "1" : "0" // Single char commands
      port.send(Uint8Array.from( ascii(cmd)));
    };

    function onUpdateBut() {
      butCheckbox.checked= !butCheckbox.checked
      alert("Do not click the box; it is checked by pressing the button on the USB device.")
    };

    ledCheckbox.addEventListener('input', onUpdateLed);
    butCheckbox.addEventListener('input', onUpdateBut);

    connectButton.addEventListener('click', function() {
      if (port) {
        port.disconnect();
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
})();
