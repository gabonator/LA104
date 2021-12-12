class Debugger
{
  constructor()
  {
//    this.server = "http://localhost:8382"
//    this.server = "http://cloud.valky.eu:8382"; // I do not have ssl enabled server :(
    this.server = "https://api.valky.eu/gabuino"; // php trampoline for http cloud server
    this.symbols = {};
    this.localSymbols = false;
    this.code = null;
    this.rawAssembly = null;
    this.assembly = null;

    this.compiledBinary = null;
    this.compiledError = null;

    this.debuggerResources = false;
    this.compilerResources = false;

    this.deviceType = "ds213"; // lower case!
    this.osHash = "bb9698a0";
    this.hostHash = "00000000";
    this.deviceUid = "00000000";
  }

  buildUrl(api)
  {
    if (this.server.indexOf("api.valky.eu") != -1)
      return this.server + "/" + api + "_" + this.deviceType + ".php";
    else
      return this.server + "/" + api + "/" + this.deviceType;
  }

  setDeviceInfo(deviceType, osHash, hostHash, deviceUid)
  {
    this.deviceType = deviceType.toLowerCase();
    this.osHash = ("00000000"+osHash.toString(16)).substr(-8);
    this.hostHash = ("00000000"+hostHash.toString(16)).substr(-8);
    this.deviceUid = ("00000000"+deviceUid.toString(16)).substr(-8);
    console.log({deviceType:this.deviceType, osHash:this.osHash, hostHash:this.hostHash, uid:this.deviceUid});
  }

  hasDeviceInfo()
  {
    return this.deviceUid != "00000000"; 
  }

  setCode(code)
  {
    if (this.code == code)
      return;
    this.code = code;
    this.debuggerResources = false;
    this.compilerResources = false;
  }

  initializeDebugger()
  {
    if (this.debuggerResources)
      return Promise.resolve();

    var promise = Promise.resolve();

    // get static resources
    if (!this.symbols["os"])
      promise = promise
        .then(() => this.requestLocalSymbols.bind(this)(this.deviceType+"_os_"+this.osHash) )
        .then((symbols) => this.symbols["os"] = symbols)
        .catch(() => console.log("Unable to collect os symbols"))
        .then(() => this.requestLocalSymbols.bind(this)(this.deviceType+"_gabuino_"+this.hostHash) )
        .then((symbols) => this.symbols["gabuino"] = symbols)
        .catch(() => console.log("Unable to collect gabuino symbols"))

    // get application resources
    return promise
      .then(this.requestSymbols.bind(this))
      .then(symbols => this.symbols["app"] = symbols)
      .then(this.requestAssembly.bind(this))
      .then(rawAssembly => this.rawAssembly = rawAssembly.stdout)
      .then(this.parseAssembly.bind(this))
      .then(assembly => this.assembly = assembly)
      .then( () =>
      {
        this.debuggerResources = true;
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
      .filter(s => s.name != "_impure_ptr" && s.name != "impure_data")
  }

  // symbols
  decodeAddr(addr, stackRa)
  {
    var ret = [];
    for (var i in this.symbols)
      for (var j in this.symbols[i])
      {
        var symbol = this.symbols[i][j];
        if (symbol.type == "code" && addr >= symbol.addr && addr < symbol.addr + symbol.len)
        {
          var fl = this.findLine(addr & ~1, stackRa); // remove thumb flag
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

  findLine(addr, stack)
  {
    if (addr < this.assembly[0].addr || addr > this.assembly[this.assembly.length-1].addr)
      return;

    var lastLine = null;
    for (var i=0; i<this.assembly.length; i++)
    {
      if (stack && this.assembly[i].addr == addr) 
      {
        // when decoding stack, the return address points to next instruction

        // check if previous instr is "bl"
        //if (["bl", "ble.n", "bge.n", "b.n"].indexOf(this.assembly[i-1]).instruction != -1)
          // return previous line, because stack points to return address before branching
        return lastLine;
      }

      if (typeof(this.assembly[i].line) != "undefined")
        lastLine = this.assembly[i].line;

      if (this.assembly[i].addr >= addr) 
        return lastLine;
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
    var nearesti = -1, nearestd = 10;
    for (var i=0; i<this.assembly.length; i++)
      if (this.assembly[i].line && Math.abs(this.assembly[i].line-line) < nearestd)
      {
        nearesti = i;
        nearestd = Math.abs(this.assembly[i].line-line);
      }
    return nearesti;
  }

  findInstruction(addr)
  {
    for (var i=0; i<this.assembly.length; i++)
      if (this.assembly[i].addr == addr)
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
          {
//            throw "Service failed";
            reject();
            return;
          }
          resolve(xhr.responseText);
        }
      }
    });  
  }

  // requests
  requestSymbols()
  {
    return this.requestService("symbols")
      .then((resp)=>
      {
        var stdout = resp.stdout;
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
        var req = this.request.post(this.buildUrl(api), function (err, resp, body) {
          if (err) {
            throw "Request failed"
          } else {      
            var jsonResponse = JSON.parse(body);
            if (!jsonResponse || jsonResponse.code !== 0)
            {
              if (jsonResponse.code == -2)
                throw "Paths not correctly set!";
              reject(jsonResponse);
            }
            else
              resolve(jsonResponse);
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
        xhr.open('post', this.buildUrl(api), true);
        xhr.send(formData);
        xhr.onload  = function() {
          var jsonResponse = JSON.parse(xhr.responseText);
          if (!jsonResponse || jsonResponse.code !== 0)
            reject(jsonResponse);
          else
            resolve(jsonResponse);
        }
      }
    });  
  }

  // compile
  compile(code)
  {
    if (this.compilerResources)
      return this.compiledBinary;

    this.compiledBinary = null;
    this.compiledError = null;

    return this.requestService("compile")
      .then((resp) => 
      { 
        this.compilerResources = true; 
        this.compiledBinary = Uint8Array.from(atob(resp.files["app.elf"]), c => c.charCodeAt(0));
        return this.compiledBinary;
      } )
      .catch((resp) =>
      {
        this.compilerError = resp.stderr;
        return Promise.reject(this.compilerError)
      })
  }
};

if (typeof(module) != "undefined" && typeof(module.exports) != "undefined")
  module.exports = Debugger;