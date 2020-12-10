var xml2json = require('xml2json');
var fs = require("fs");

var json = xml2json.toJson(fs.readFileSync("test.sch"), {object: true, coerce:true});
var s = json.eagle.drawing.schematic;
var _parts = s.parts.part;
var _instances = s.sheets.sheet.instances.instance;
var _nets = s.sheets.sheet.nets.net;

var fixArray = a => Array.isArray(a) ? a : (a ? [a] : []);
var wires = [];
var junctions = [];
var instances = [];
var parts = [];
var pinrefs = [];
var nets = [];

for (var i in _nets)
{ 
  var net = _nets[i];
  var wire = fixArray(net.segment.wire);
  var junc = fixArray(net.segment.junction)
  var pinref = fixArray(net.segment.pinref);

  nets.push(net["name"]);

  for (var j in wire)
    wires.push({x1:wire[j]["x1"], y1:wire[j]["y1"], x2:wire[j]["x2"], y2:wire[j]["y2"], name:net["name"]});
  for (var j in junc)
    junctions.push({x:junc[j]["x"], y:junc[j]["y"], name:net["name"]});
  for (var j in pinref)
    pinrefs.push({part:pinref[j]["part"], pin:pinref[j]["pin"], name:net["name"]});

}

for (var i in _instances)
{
  var instance = _instances[i];
  instances.push({part:instance["part"], x:instance["x"], y:instance["y"], rot:instance["rot"] ? instance["rot"] : "R0"});
}

for (var i in _parts)
{
  var part = _parts[i];
  parts.push({name:part["name"], device:part["device"], deviceset:part["deviceset"]});
}

wires = wires.map( w=>({x1:w.x1*100, y1:w.y1*100, x2:w.x2*100, y2:w.y2*100, name:w.name}));
junctions = junctions.map( w=>({x:w.x*100, y:w.y*100, name:w.name}));
instances = instances.map( w=>({name:w.part, device:parts.find(p=>p.name==w.part).deviceset, x:w.x*100, y:w.y*100, rot:parseInt(w.rot.substr(1))}));
var conns = [];
for (var i in nets)
{
  var conn = [nets[i]];
  for (var j in pinrefs)
    if (pinrefs[j].name == nets[i])
    {
      conn.push(pinrefs[j].part + "." + pinrefs[j].pin);
    }
  if (conn.length > 1)
    conns.push(conn);
}

var res = {wires:wires, junctions:junctions, instances:instances, graph:conns};
console.log(res);
