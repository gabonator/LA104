var gccasm = process.argv[2];
var objdumpasm = process.argv[3];

var fs = require("fs");
var v = fs.readFileSync(gccasm);

var insn1 = [];
var lines = [];
v = v.toString().split("\n");
var info = "";
for (var i=0; i<v.length; i++)
{
  var l = v[i];
  if (tokens = l.match("^@ ([^:]*):(\\d*):"))
  {
    info = tokens[1] + ":" + tokens[2]
  }

  if (tokens = l.match("\x09(\\S*)\x09"))
  {
    if (tokens[1][0] != '.' && tokens[1][0] != '@')
    {
      //console.log(tokens[1]);
      insn1.push(tokens[1]);
      lines.push(info);
      info = "";
    }
  }
}


var v = fs.readFileSync(objdumpasm);
var insn2 = [];
v = v.toString().split("\n");

for (var i=0; i<v.length; i++)
{
  var l = v[i];
  if (tokens = l.match("([0-9a-fA-f]+):\x09([0-9a-fA-f ]+)\x09(\\S+)\x09"))
  {
    if (tokens[3][0] != '.')
//    console.log(tokens[3]);
    {
      insn2.push(tokens[3]);
      if (insn2.length > insn1.length)
        break;
      if (insn1[insn2.length-1][0] != insn2[insn2.length-1][0])
        throw "not matching instructions!";
 
//      console.log((lines[insn2.length-1] + "               ").substr(0, 15) +  l);
      var line = lines[insn2.length-1];
      if (line != "")
        console.log(line + "\x09" + l);
      else
        console.log("\x09\x09" +  l);
    }
  }
}
//20004804:	af00      	add	r7, sp, #0
//20004806:	f000 f871 	bl	200048ec <BIOS::KEY::GetKey()@plt>

//for (var i=0; i<insn1.length; i++)
//  console.log(insn1[i] + " " + insn2[i]);