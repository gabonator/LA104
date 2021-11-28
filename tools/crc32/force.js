if (process.argv.length != 6)
  throw "No file specified"

var input_bin = process.argv[2];
var input_elf = process.argv[3];
var input_magic = parseInt(process.argv[4])
var output_hash = process.argv[5];

//console.log([input_bin, input_elf, input_magic.toString(16)]);
                         
var fs = require("fs");
var exec = require('child_process').exec;
var path = require("path");

var offset_bin;
var crc_bin;

Promise.resolve()
  .then(() => findMagic(input_bin, input_magic))
  .then((offset) => offset_bin = offset)
  .then(() => patchCrc(input_bin, offset_bin))
  .then(() => readDword(input_bin, offset_bin))
  .then((crc) => {
    var dword = [crc[3], crc[2], crc[1], crc[0]].
      map(b=>("0"+b.toString(16)).substr(-2)).join("");
    fs.writeFileSync(output_hash, dword);
    crc_bin = crc;
  })
  .then(() => findMagic(input_elf, input_magic))
  .then((offset) => patchDword(input_elf, offset, crc_bin))
  .catch(exc => console.log("Exception: " + exc) );

function findMagic(filename, magic)
{
  if (!magic)
    throw "No magic provided"

  var buf = fs.readFileSync(filename);
  var bytes = [magic & 0xff, (magic >> 8) & 0xff, (magic >> 16) & 0xff, (magic >> 24) & 0xff];

  var offsets = [];
  for (var i=0; i<buf.length-4; i++)
  {
    if (buf[i] == bytes[0] && buf[i+1] == bytes[1] && 
      buf[i+2] == bytes[2] && buf[i+3] == bytes[3])
    {
      console.log("Found magic token at offset 0x" + i.toString(16) + " in " + filename);
      offsets.push(i);
    }
  }

  if (offsets.length != 1)
  {
    console.log("Single instance of magic sequence was not found!");
    throw "Single instance of magic sequence was not found!"
  }
  return offsets[0];
}

function readDword(filename, offset)
{
  var buf = fs.readFileSync(filename);
  return [buf[offset], buf[offset+1], buf[offset+2], buf[offset+3]];
}

function patchDword(filename, offset, bytes)
{
  var buf = fs.readFileSync(filename);
  buf[offset] = bytes[0];
  buf[offset+1] = bytes[1];
  buf[offset+2] = bytes[2];
  buf[offset+3] = bytes[3];
  fs.writeFileSync(filename, buf);
}

function patchCrc(filename, offset)
{
  return new Promise( (resolve, reject)=>
  {
    var cmd = [path.resolve(__dirname, 'forcecrc32'), 
      "\"" + filename + "\"", 
      offset, 
      "00000000"]

    exec(cmd.join(" "), function(err, data) {  
      if (data.indexOf("New CRC-32 successfully verified") == -1)
        throw "Patching failed"
      //console.log("Patching done");
      resolve();
    });  
  });
}
