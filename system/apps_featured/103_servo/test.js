var fs = require('fs'),
PNG = require('pngjs').PNG;

var RGB565RGB = (r, g, b) => (((r)>>3)|(((g)>>2)<<5)|(((b)>>3)<<11));

//fs.createReadStream('back24.png')
fs.createReadStream('knob24w.png')
  .pipe(new PNG())
  .on('parsed', function() {

    var pixels = [];
//    console.log([this.width, this.height]);

    for (var y = 0; y < this.height; y++) {
      for (var x = 0; x < this.width; x++) {
        var idx = (this.width * y + x) << 2;
        var r = this.data[idx];
        var g = this.data[idx+1];
        var b = this.data[idx+2];
        var a = this.data[idx+3];
//        if (a < 255)
//        {
          
//        }
//        if (1) //a >= 200)
//        {
          var pix16 = RGB565RGB(r, g, b);
          pixels.push(pix16 & 0xff);
          pixels.push(pix16 >> 8);
//        } else {
//          var pix16 = 0xaaaa;
//          pixels.push(pix16 & 0xff);
//          pixels.push(pix16 >> 8);
//        }
      }
    }

   var buf = Buffer.from(pixels);
   console.log(exportBuffer(this, buf));

/*
    for (var y = 0; y < this.height; y++) {
        for (var x = 0; x < this.width; x++) {
            var idx = (this.width * y + x) << 2;

            // invert color
            this.data[idx] = 255 - this.data[idx];
            this.data[idx+1] = 255 - this.data[idx+1];
            this.data[idx+2] = 255 - this.data[idx+2];

            // and reduce opacity
            this.data[idx+3] = this.data[idx+3] >> 1;
        }
    }

    this.pack().pipe(fs.createWriteStream('out.png'));
*/
});

function exportBuffer(img, buf)
{
  var id = "knob";
  var aux = [
    "const int "+id+"_width = " + img.width + ";",
    "const int "+id+"_height = " + img.height + ";",
    "static const uint16_t "+id+"_data["+buf.length/2+"] = {"];
  var ofs = 0;

  while (ofs<buf.length)
  {   
    var count = Math.min(buf.length - ofs, 16);
    var line = "  ";
    for (var i=0; i<count; i+=2)
    {
      var v = buf[i+ofs] + buf[i+ofs+1]*256;
      line += "0x" + ("000"+v.toString(16)).substr(-4) + ", ";
    }
    aux.push(line);
    ofs += count;
  }
  aux.push("};");
  return aux.join("\n");
}                