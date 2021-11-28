class Debugger
{
  constructor()
  {
//    this.promise = Promise.resolve();
    this.server = "http://localhost:8382"
    this.symbols = {};
    this.localSymbols = false;
    this.code = null;
  }

  initialize(code)
  {
    // request device type - TODO: not exported by gabuino
    // request device hash - TODO: not exported by gabuino
    if (this.code == code)
      return Promise.resolve();

    this.code = code;

    this.deviceType = "DS213";
    this.deviceHash = "bb9698a0";
    var promise = Promise.resolve();

    // get static resources
    if (!this.symbols["os"])
      promise = promise
        .then(() => this.requestLocalSymbols.bind(this)(this.deviceType.toLowerCase()+"_os_"+this.deviceHash) )
        .then((symbols) => this.symbols["os"] = symbols)
        .then(() => this.requestLocalSymbols.bind(this)(this.deviceType.toLowerCase()+"_gabuino") )
        .then((symbols) => this.symbols["gabuino"] = symbols);

    // get application resources
    return promise
      .then(this.requestSymbols.bind(this))
      .then(symbols => this.symbols["app"] = symbols)
      .then(this.requestAssembly.bind(this))
      .then(this.parseAssembly.bind(this))
      .then(assembly => this.assembly = assembly)
      .then( () =>
      {
//        console.log(this.symbols["os"]);
//        console.log(this.symbols);
//        console.log(this.assembly);
      });
  }

  // variables
  getVariables()
  {
    return this.symbols["app"]
      .filter(s => s.type == "data")
      .filter(s => s.name != "_impure_ptr")
  }

  // symbols
  decodeAddr(addr)
  {
    var ret = [];
    for (var i in this.symbols)
      for (var j in this.symbols[i])
      {
        var symbol = this.symbols[i][j];
        if (symbol.type == "code" && addr >= symbol.addr && addr < symbol.addr + symbol.len)
        {
          var fl = this.findLine(addr);
          var ofs = (addr & ~1) - symbol.addr;
          ret.push({module:i, name:symbol.name, offset:ofs, line:fl, addr:addr & ~1});
        }
      }

    if (!ret.length)
      return null
    if (ret.length > 1)
      throw "multiple symbols!";
    return ret[0];
  }

  findLine(addr)
  {
    if (addr < this.assembly[0].addr || addr > this.assembly[this.assembly.length-1].addr)
      return;

    var lastLine = null;
    for (var i=0; i<this.assembly.length; i++)
    {
      if (typeof(this.assembly[i].line) != "undefined")
        lastLine = this.assembly[i].line;

      if (this.assembly[i].addr >= addr) 
      {
        // we should check the instr at this address and before it
        // stack points to the next address after branch, so lets return one
        // line above of this  
        return lastLine-1;
      }
    }
  }

  // assembly
  parseAssembly(asm)
  {
    var lines = asm.split("\n");
    lines = lines.map(l => l.match("([0-9a-fA-f]+):\x09([0-9a-fA-f ]+)\x09(\\S+)\x09?([^;]*)(; .+:(.+))?"))
      .filter(r=>r)
      .map(r=>(r[6] ? 
        {addr:parseInt("0x"+r[1]), opcode:r[2].trim().split(" "), instruction:r[3], arguments:r[4].trim(), line:parseInt(r[6])} :
        {addr:parseInt("0x"+r[1]), opcode:r[2].trim().split(" "), instruction:r[3], arguments:r[4].trim()}));
    return lines;
  }

  findNearestLine(line)
  {
    for (var i=0; i<this.assembly.length; i++)
      if (this.assembly[i].line >= line)
        return i;
    return -1;
  }

  // local requests
  requestLocalSymbols(name)
  {
    return this.requestFile("symbols/" + name + ".js")
      .then((data)=>
      {
        var lines = data.split("\n");
        lines[0] = "[";
        data = lines.join("");
        return eval(data);
      });
  }

  requestFile(name)
  {
    return new Promise( (resolve, reject) =>
    {
      if (typeof(module) != "undefined" && typeof(module.exports) != "undefined")
      {
        // nodejs
        if (!this.fs)
          this.fs = require("fs");
        resolve(this.fs.readFileSync("../web/v1/"+name).toString());
      } else
      {
        // html
        var xhr = new XMLHttpRequest();
        var formData = new FormData();
        xhr.open('get', name, true);
        xhr.send();
        xhr.onload  = function() {
          if (!xhr.responseText)
            throw "Service failed";
          resolve(xhr.responseText);
        }
      }
    });  
  }

  // requests
  requestSymbols()
  {
    return this.requestService("symbols")
      .then((stdout)=>
      {
        if (stdout.substr(0, 15) != "dsym['test'] = ")
          throw "Failed to request symbols - wrong response";
        var code = stdout.substr(15);
        return eval(code);
      });
  }

  requestAssembly()
  {
    return this.requestService("assembly");
  }

  requestService(api)
  {
    return new Promise( (resolve, reject) =>
    {
      if (typeof(module) != "undefined" && typeof(module.exports) != "undefined")
      {
        // nodejs
        if (!this.request)
          this.request = require("request");

        var req = this.request.post(this.server + "/" + api, function (err, resp, body) {
          if (err) {
            throw "Request failed"
          } else {      
            var jsonResponse = JSON.parse(body);
            if (!jsonResponse || jsonResponse.code !== 0)
              throw "Service failed";
            resolve(jsonResponse.stdout);
          }
        });

        var form = req.form();
        form.append('file', this.code, {
          filename: 'app.cpp',
          contentType: 'text/plain'
        });
      } else
      {
        // html
        var xhr = new XMLHttpRequest();
        var formData = new FormData();
        formData.append("file", new Blob([this.code], {type : 'text/plain'}), "app.cpp");
        xhr.open('post', this.server + "/" + api, true);
        xhr.send(formData);
        xhr.onload  = function() {
          var jsonResponse = JSON.parse(xhr.responseText);
          if (!jsonResponse || jsonResponse.code !== 0)
            throw "Service failed";
          resolve(jsonResponse.stdout);
        }
      }
    });  
  }
};

if (typeof(module) != "undefined" && typeof(module.exports) != "undefined")
  module.exports = Debugger;