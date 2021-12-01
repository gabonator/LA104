dsym = {};

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

function debuggerTryBreakpoint(line)
{
  if (!running)
  {
    alert("Program not running");
    return Promise.resolve(-1);
  }
  var update = dbg.initializeDebugger();

  return update.then(() => {
    console.log("matching line... " + line);
    var nline = dbg.findNearestLine(line+1);
    if (nline == -1)
    {
      console.log("empty assembly!?");
      return -1;
    }

    var nearest = dbg.assembly[nline];
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
  var l = dbg.findInstruction(addr);
  if (l==-1)
    throw "Unable to revert breakpoint!";

  l = dbg.assembly[l];
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