var fs = require('fs'),
PNG = require('pngjs').PNG;


function getPixels(img)
{
  return new Promise((resolve, reject) =>
  {
    fs.createReadStream(img)
      .pipe(new PNG())
      .on('parsed', function() {
        resolve(this); // data, width, height
    });
  });
}

function section(png, left, top, right, bottom)
{
  var aux = [];
  for (var y=top; y<bottom; y++)
    for (var x=left; x<right; x++)
    {
      var p = (y*png.width+x)*4;
      var r = png.data[p];
      var g = png.data[p+1];
      var b = png.data[p+2];
      var a = png.data[p+3];
      if (a==0)
        aux.push(0xfe00fe);
      else
        aux.push(r | (g<<8) | (b<<16));
    }
  return aux;
}

function compress(pixels)
{
  var palette = [];
  var indices = [];
  for (var i=0; i<pixels.length; i++)
  {
    var p = pixels[i];
    if (palette.indexOf(p) == -1)
      palette.push(p);
    indices.push(palette.indexOf(p));
  }                                        
  return {indices:indices, palette:palette};
}

function compact(ind)
{
  var out = [];
  for (var i=0; i<ind.length; i+=8)
  {
    var word = "0x";
    for (var j=0; j<8; j++)
    {
//      word <<= 4;
//      word |= ind[i+j];
      word += ind[i+j].toString(16);
    }
    if (word.length != 10)
    {
      throw "bug"
    }
    out.push(word);
  }
  return out;
}

function exportc(pixels, idi, idp)
{
  var image = compress(pixels);
  var bitmap = compact(image.indices);
//  console.log("static const uint32_t " + idi + "[] = {"+ bitmap.map(x=>"0x"+("00000000"+x.toString(16)).substr(-8)).join(", ") + "};");
  console.log("static const uint32_t " + idi + "[] = {"+ bitmap.join(", ") + "};");
  console.log("static const uint16_t " + idp + "[] = {" + image.palette.map(x=>{ 
    if (x==0xfe00fe)
      return "RGBTRANS";
    else
      return "RGB565RGB("+(x&255)+","+((x>>8)&255)+","+((x>>16)&255)+")";
  }).join(", ") + "};");
}

if (0)
getPixels('powerups.png').then(png =>
{
  for (var index = 0; index<10; index++)
  {
    var pixels = section(png, 0, index*8, 128, index*8+7);
    exportc(pixels, "powerup" + index, "powerup"+index+"pal");
  }       
});

getPixels('enemies.png').then(png =>
{
  var row = 2;
  var cols = 24;
  var pixels = section(png, 0, row*16, cols*16, row*16+16);
  exportc(pixels, "enemy" + row, "enemy"+row+"pal");
});