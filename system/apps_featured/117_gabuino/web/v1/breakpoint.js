//dsym = {};

function debuggerTryBreakpoint(line)
{
//  if (!running)
//  {
//    alert("Program not running");
//    return Promise.resolve(-1);
//  }
  var update = dbg.initializeDebugger();

  return update.then(() => {
    console.log("matching line... " + line);
    var nline = dbg.findNearestLine(line /*+1*/);
    if (nline == -1)
    {
      alert("Unable to place breakpoint at line " + line);
      console.log("empty assembly!?");
      return -1;
    }

    var nearest = dbg.assembly[nline];
    applyBreakpoint(nearest);
    return nearest.line/* -1*/;
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
}

function revertBreakpoint(addr)
{
  console.log("Reverting breakpoint: " + addr);
  if (!addr)
  {
    alert("Not trapped");
    return;
  }
  var l = dbg.findInstruction(addr);
  if (l==-1)
    throw "Unable to revert breakpoint!";

  l = dbg.assembly[l];
  var opcode = parseInt("0x" + l.opcode[0]);

  return BIOS.memRead(l.addr, 2).then(buf => {
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