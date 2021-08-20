dsym = {};

function debug()
{
  return new Promise( (resolve, reject) =>
  {
    var code = html_editor.getValue();
    code = code.split("int main(").join("int __attribute__((__section__(\".entry\"))) main(");
    var xhr = new XMLHttpRequest();
    var formData = new FormData();
    formData.append("file", new Blob([code], {type : 'text/plain'}), "moj.txt");

//    xhr.open('post', "http://cloud.valky.eu:8382/debug", true);
    xhr.open('post', "https://api.valky.eu/gabuino/symbols.php", true);
//    xhr.open('post', "localhost:8382/compile", true);
    xhr.send(formData);
    xhr.onload  = function() {
      var jsonResponse = JSON.parse(xhr.responseText);
      setTimeout(jsonResponse.stdout, 0);

      console.log(jsonResponse);
    };
  });
}

function decodeAddr(addr)
{
  var ret = [];
  for (var i in dsym)
  {
    var symlist = dsym[i];
    for (var j in symlist)
    {
      var symbol = symlist[j];
      if (symbol.type != "range" && addr >= symbol.addr && addr < symbol.addr + symbol.len)
      {
        ret.push("("+i+") " + symbol.name);
      }
    }
  }
  if (!ret.length)
    return null
  if (ret.length > 1)
    throw "multiple symbols!";
  return ret[0];
}

function frame()
{
  BIOS.frame().then(f => {
    var arr = [];
//    for (var i=0; i<f.length; i++) arr.push("0x"+ ("00000000"+f[i].toString(16)).substr(-8));
    for (var i=0; i<f.length; i++) arr.push(f[i]); //"0x"+ ("00000000"+f[i].toString(16)).substr(-8));

//    console.log(arr.map(p => "0x"+ ("00000000"+p.toString(16)).substr(-8)));
//    console.log(arr.map(decodeAddr).filter(x => x));

  console.log(arr.map(a => {
    dec = decodeAddr(a);
    if (!dec)
      return "0x"+ ("00000000"+a.toString(16)).substr(-8);
    else
      return "0x"+ ("00000000"+a.toString(16)).substr(-8) + " " + dec;
  }));
    //console.log(f.map(x=>"0x"+x.toString(16)))
  });
}

function screenshot()
{
  BIOS.screenshot().then(data => 
  {

    var offscreenCanvas = document.createElement('canvas');
    offscreenCanvas.style = "border:3px solid #000000; width:300px; height:300px"
    offscreenCanvas.width = 320;
    offscreenCanvas.height = 240;
    //document.getElementById("_debugText").appendChild(offscreenCanvas);
    var context = offscreenCanvas.getContext('2d');
    context.fillStyle = "red";
    context.fillRect(10,10,100,100);


    var imagedata = context.createImageData(320, 240);
    var  j=0;
      for (var x=0; x<320; x++)
    for (var y=0; y<240; y++)
      {
        var rgb = data[j++];
        rgb = ((rgb << 8) | (rgb >> 8)) & 0xffff;
        var i = ((239-y)*320+x)*4;
        imagedata.data[i++] = (((rgb)&0x1f)<<3);
        imagedata.data[i++] = ((((rgb)>>5)&0x3f)<<2);
        imagedata.data[i++] = ((((rgb)>>11)&0x1f)<<3);
        imagedata.data[i++] = 255;
      }
    context.putImageData(imagedata, 0, 0);

    var element = "<img src='"+offscreenCanvas.toDataURL()+"'>";
    _DBGPRINT(element);
  });

}

function assembly()
{
  return new Promise( (resolve, reject) =>
  {
    var code = html_editor.getValue();
    code = code.split("int main(").join("int __attribute__((__section__(\".entry\"))) main(");
    var xhr = new XMLHttpRequest();
    var formData = new FormData();
    formData.append("file", new Blob([code], {type : 'text/plain'}), "moj.txt");

//    xhr.open('post', "https://api.valky.eu/gabuino/debug.php", true);
//    xhr.open('post', "http://cloud.valky.eu:8382/debug", true);
    xhr.open('post', "http://localhost:8382/debug", true);
    xhr.send(formData);
    xhr.onload  = function() {
      var jsonResponse = JSON.parse(xhr.responseText);
//      setTimeout(jsonResponse.stdout, 0);
      $("#_testing").html(jsonResponse.stdout)
      console.log(jsonResponse);
    };
  });
}
