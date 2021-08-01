var LCD = {
  Background: (rect, c0, c1) => BIOS.rpcCall("LCD::Background("+LCD.CRect(rect)+","+LCD.rgb565(c0)+","+LCD.rgb565(c1)+");"),
  Bar: (rect, c) => BIOS.rpcCall("LCD::Bar("+LCD.CRect(rect)+","+LCD.rgb565(c)+");"),
  Print: (x, y, cfront, cback, text) => BIOS.rpcCall("LCD::Print("+[Math.floor(x), Math.floor(y), LCD.rgb565(cfront), LCD.rgb565(cback), "\"" + text + "\""].join(",")+");"),

  CRect:(rc) => "CRect("+rc.map(i=>Math.floor(i)).join(",")+")",
  transparent: 0xf700ff,
  rgb565: (c) =>
  {
    var r = c >> 16;
    var g = (c >> 8) & 255;
    var b = (c) & 255;
    var clr =( (r>>3)) | ((g>>2)<<5) | ((b>>3)<<11);
    return "0x"+clr.toString(16);
  }
};

var BIOS =
{
  safeeval: (json) => { if (json[0] == "{") return eval("("+json+")") },
  retval: (json) => { var j = BIOS.safeeval(json); if (j && typeof(j.ret) != "undefined") return j.ret },
  rpcCall: (command) =>
  {                 
    return new Promise((resolve, reject) =>
    {
      COMM._onReceive = data => { COMM._onReceive = COMM._defReceive; resolve(new TextDecoder().decode(data));  };
      COMM._send(command);
    });
  },
  rpcPeekRaw: () =>
  {                 
    return new Promise((resolve, reject) =>
    {
      COMM._onReceive = data => { COMM._onReceive = COMM._defReceive; resolve(data); };
    });
  },
  rpcPeek: () =>
  {                 
    return new Promise((resolve, reject) =>
    {
      COMM._onReceive = data => { COMM._onReceive = COMM._defReceive; resolve(new TextDecoder().decode(data)); }
    });
  },

  memRead: (addr, len) => 
  {
    BIOS._rawData = null;
    return BIOS.rpcCall('MEM::Read(0x'+addr.toString(16)+','+len+');')
    .then( json => { if (typeof(BIOS.safeeval(json).bulk) == "undefined") throw "problem"; return BIOS.rpcPeekRaw(); } )
    .then( rawdata => { BIOS._rawData = rawdata; /*console.log("raw:"+rawdata.byteLength); */return BIOS.rpcPeek(); })
    .then( json => { if (typeof(BIOS.safeeval(json).ret) == "undefined") throw "problem"; 
      return Promise.resolve(new Uint8Array(BIOS._rawData.buffer)); })
  },
  memWrite: (addr, buf) =>
  {
    return BIOS.rpcCall('MEM::Write(0x'+addr.toString(16)+','+buf.length+');')
    .then( json => { if (typeof(BIOS.safeeval(json).ret) == "undefined") throw "problem"; 
      console.log("Sending " + buf.length);
      COMM._sendRaw(buf);
      return BIOS.rpcPeek();
       })
    .then( json => { if (typeof(BIOS.safeeval(json).ret) == "undefined") throw "problem"; return BIOS.retval(json); })
  },
  getProcAddr: (name) => BIOS.rpcCall('SYS::GetProcAddress(\"'+name+'\");').then( BIOS.retval ),
  exec: (addr) => BIOS.rpcCall('DBG::Exec(0x'+addr.toString(16)+')').then( ret =>
  {
    if (BIOS.safeeval(ret))
      return BIOS.retval(ret);
    else
      COMM._defEval(ret);
  }),
  stop: () => BIOS.rpcCall('DBG::Stop()').then( BIOS.retval ),
  frame: () => BIOS.rpcCall('DBG::Frame()')
    .then( json => { if (typeof(BIOS.safeeval(json).bulk) == "undefined") throw "problem"; return BIOS.rpcPeekRaw(); } )
    .then( rawdata => { BIOS._rawData = rawdata; /*console.log("raw:"+rawdata.byteLength); */return BIOS.rpcPeek(); })
    .then( json => { if (typeof(BIOS.safeeval(json).ret) == "undefined") throw "problem"; 
      return Promise.resolve(new Uint32Array(BIOS._rawData.buffer)); }),
  screenshot: () => BIOS.rpcCall('DBG::Screenshot()')
    .then( json => { if (typeof(BIOS.safeeval(json).bulk) == "undefined") throw "problem"; COMM.debug--; return BIOS.rpcPeekRaw(); } )
    .then( rawdata => { BIOS._rawData = [rawdata]; return BIOS.rpcPeekRaw(); })
    .then( rawdata => { BIOS._rawData.push(rawdata); return BIOS.rpcPeekRaw(); })
    .then( rawdata => { BIOS._rawData.push(rawdata); COMM.debug++; return BIOS.rpcPeek(); })
    .then( json => { 
      // workaround for 64kb transfer limitation
      if (typeof(BIOS.safeeval(json).ret) == "undefined") throw "problem"; 
        return Promise.resolve(Uint16Array.from(
          function*() { 
            yield* new Uint16Array(BIOS._rawData[0].buffer); 
            yield* new Uint16Array(BIOS._rawData[1].buffer); 
            yield* new Uint16Array(BIOS._rawData[2].buffer); 
          }()
        )) 
    } ),
};
