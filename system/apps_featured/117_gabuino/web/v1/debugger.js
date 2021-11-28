dsym = {};
/*
function debug()
{
  return new Promise( (resolve, reject) =>
  {
    var code = html_editor.getValue();
//    code = code.split("int main(").join("int __attribute__((__section__(\".entry\"))) main(");
    var xhr = new XMLHttpRequest();
    var formData = new FormData();
    formData.append("file", new Blob([code], {type : 'text/plain'}), "moj.txt");

//    xhr.open('post', "http://cloud.valky.eu:8382/debug", true);
//    xhr.open('post', "https://api.valky.eu/gabuino/symbols.php", true);
    xhr.open('post', "http://localhost:8382/symbols", true);
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
*/
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
  store_code();
  return new Promise( (resolve, reject) =>
  {
    var code = html_editor.getValue();
//    code = code.split("int main(").join("int __attribute__((__section__(\".entry\"))) main(");
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
      resolve(jsonResponse.stdout);
//      $("#_testing").html("<pre>"+jsonResponse.stdout+"</pre>")
//      console.log(jsonResponse);
    };
  });
}


function parseAssembly(asm)
{
/*
20005000:	b580      	push	{r7, lr}
20005002:	b082      	sub	sp, #8
20005004:	af00      	add	r7, sp, #0
20005006:	481e      	ldr	r0, [pc, #120] ; ../source/test.cpp:7
20005008:	f000 f87c 	bl	20005104 
2000500c:	2300      	movs	r3, #0 ; ../source/test.cpp:8
2000500e:	607b      	str	r3, [r7, #4]
20005010:	687b      	ldr	r3, [r7, #4] ; ../source/test.cpp:8
20005012:	f640 32b7 	movw	r2, #2999

  if (tokens = l.match("([0-9a-fA-f]+):\x09([0-9a-fA-f ]+)\x09(\\S+)"))

*/
  var lines = asm.split("\n");
  lines = lines.map(l => l.match("([0-9a-fA-f]+):\x09([0-9a-fA-f ]+)\x09(\\S+)\x09?(.*) ; .*:(.+)"))
    .filter(r=>r)
    .map(r=>({addr:parseInt("0x"+r[1]), opcode:r[2].trim().split(" "), instruction:r[3], arguments:r[4], line:parseInt(r[5])}));
//  console.log(lines);
  return lines;
}

function assemblyFindNearestLine(line)
{
  for (var i=0; i<lastAssembly.length; i++)
    if (lastAssembly[i].line >= line)
      return i;
  return -1;
}

function assemblyFindAddress(addr)
{
  for (var i=0; i<lastAssembly.length; i++)
    if (lastAssembly[i].addr == addr)
      return i;
  return -1;
}

function debuggerTryBreakpoint(line)
{
  if (!running)
  {
    alert("Program not running");
    return Promise.resolve(-1);
  }
  var update = Promise.resolve();

  if (typeof(lastAssembly) == "undefined" || !lastAssembly)
  {
    console.log("requesting assembly...");
    update = assembly().then(asm =>
    {
      console.log("parsing assembly...");
      lastAssembly = parseAssembly(asm);
    })
  }

  return update.then(() => {
//    console.log("matching line... " + line);
    var nline = assemblyFindNearestLine(line+1);
    if (nline == -1)
    {
      console.log("empty assembly!?");
      return -1;
    }

    var nearest = lastAssembly[nline];
    applyBreakpoint(nearest);
    return nearest.line -1;
  })
}

function dumpBuf(buf)
{
  var aux = "";
  for (var i=0; i<buf.length; i++)
    aux += ("0" + buf[i].toString(16)).substr(-2) + " ";
  return aux;
}

function applyBreakpoint(l)
{
  BIOS.memRead(l.addr, 2).then(buf => {
    console.log("Setting breakpoint at 0x" + l.addr.toString(16));
    console.log("Memory read: " + dumpBuf(buf));
    console.log("Should equal to: " + l.opcode[0]);
  })
  .then(() => BIOS.memWrite(l.addr, [0x00, 0xbe]))
//  .then(() => BIOS.memRead(l.addr, 2))
//  .then(buf => {
//    console.log("Memory check: " + dumpBuf(buf));
//  });
}

function revertBreakpoint(addr)
{
  var l = assemblyFindAddress(addr);
  if (l==-1)
    throw "Unable to revert breakpoint!";

  l = lastAssembly[l];
  var opcode = parseInt("0x" + l.opcode[0]);

  BIOS.memRead(l.addr, 2).then(buf => {
    console.log("Reverting breakpoint at 0x" + l.addr.toString(16));
    console.log("Memory read: " + dumpBuf(buf));
    console.log("Should equal to: 0xbe, 0x00");
    console.log("Reverting to: " + ("0000"+opcode.toString(16)).substr(-4));

    var breakpoints = html_editor.session.getBreakpoints(/*newRow, 0*/);

    if(typeof breakpoints[l.line-1] === typeof undefined)
      console.log("No breakpoint in editor!?");
    else
      html_editor.session.clearBreakpoint(l.line-1);

  }).then(() => BIOS.memWrite(l.addr, [opcode & 0xff, opcode >> 8]))
  .then(() => BIOS.memRead(l.addr, 2))
  .then(buf => {
    console.log("Memory check: " + dumpBuf(buf));
  })
  .then(() => BIOS.resume())
}