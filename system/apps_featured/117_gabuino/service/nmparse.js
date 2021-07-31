var input = process.argv[2];
var id = process.argv[3];
var fs=require("fs");
var s = fs.readFileSync(input).toString().split("\n");

// 200048a0 00000001 t std::piecewise_construct
// 200048e0 00000004 b main::dwTick__LINE__
//20000000 D _addressRamBegin
//200027e4 B _addressRamEnd
//08008000 T _addressRomBegin
//0801301c T _addressRomEnd

var syms = [];
var consts = {};
for(var i in s)
{
  var l = s[i];
  var m = l.match("(\\S{8}) (\\S) (_address.+)$");
  if (m)
  {
    consts[m[3]] = parseInt("0x"+m[1]);
    continue;
  }
  var m = l.match("(\\S{8}) (\\S{8}) (\\S) (.+)$");
//console.log(m);
  if (!m) continue;
  var type="";
  if (m[3] == "T") type="code";
  else if (m[3] == "D") type="data";
  else continue;
  
  syms.push({addr:parseInt("0x"+m[1]),len:parseInt("0x"+m[2]),type:type,id:m[4]});
}
function fadr(e) { return "0x"+ ("00000000"+e.toString(16)).substr(-8); }

///console.log(syms); return;
console.log("dsym['"+id+"'] = [");
if (consts["_addressRamBegin"])
  console.log("{addr:"+fadr(consts["_addressRamBegin"])+",len:"+(consts["_addressRamEnd"]-consts["_addressRamBegin"])+",type:'range',name:'ram'}")
if (consts["_addressRomBegin"])
  console.log("{addr:"+fadr(consts["_addressRomBegin"])+",len:"+(consts["_addressRomEnd"]-consts["_addressRomBegin"])+",type:'range',name:'rom'}")
console.log(syms.map(s=>"{addr:" + fadr(s.addr) + ",len:"+s.len+",type:'"+s.type+"',name:'"+s.id+"'},").join("\n"))
console.log("]");