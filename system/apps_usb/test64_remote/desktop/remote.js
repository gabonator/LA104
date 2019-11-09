const SerialPort = require('serialport')
const port = new SerialPort("/dev/tty.usbmodemLA104RCS1", { baudRate: 115200 })

function send(b)
{
  port.write(new Buffer(b));
}

var keypress = require('keypress');

keypress(process.stdin);

process.stdin.on('keypress', function (ch, key) {
  if (key.name == "left") send("-");
  if (key.name == "right") send("+");
  if (key.name == "up") send("<");
  if (key.name == "down") send(">");
  if (key.name == "return") send("1");
  if (key.name == "backspace") send("2");

  if (key && key.ctrl && key.name == 'c') {
    process.stdin.pause();
  }
});

process.stdin.setRawMode(true);
process.stdin.resume();