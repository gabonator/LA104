var fs = require("fs");
var buf = [...fs.readFileSync("data512.txt")];

var out = [];
var bits = 0;
var buffer = 0;
while (buf.length > 0)
{
  var byte = buf.shift();
  buffer |= (byte | 0x000) << bits;
  bits += 9;
  while (bits >= 8)
  {
    var final = buffer & 0xff;
    buffer >>= 8;
    out.push(final);
    bits -= 8;
  }
}

console.log(out.reverse().map(x=>("0"+x.toString(16)).substr(-2)).join("").toUpperCase());