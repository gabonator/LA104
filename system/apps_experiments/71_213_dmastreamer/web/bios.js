var ANAL = {
  Configure: () => BIOS.rpcCall("ANAL::Configure();"),
  Restart: () => BIOS.rpcCall("ANAL::Restart();"),
  Status: () => BIOS.rpcCall("ANAL::Status();").then( json => BIOS.retval(json) ),
  Get: () => BIOS.rpcCallBinary("ANAL::Get();").then( (data) => new Promise((resolve, reject) =>
  { 
    var arr = [];
    for (var i=0; i<data.byteLength; i+=4)
    {
      // ....  ....  .... ....  .... ....  .... ....
      // LOGIC =============time=========  --STATUS-
      var code = data.getUint32(i, true);
      var record = { 
        ch:[(code>>28)&1, (code>>29)&1, (code>>30)&1, (code>>31)&1],
        t: (code>>8)&(0xfffff),
        s: code & 0xff
      };
      arr.push(record);
    }
    COMM.onReceive = () => {COMM.onReceive = null; resolve(arr);}; 
//    return arr; 
  }) ),
};

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
  biosSpiBegin: (baud) => BIOS.rpcCall('SPI::begin('+baud+');'),
  biosSpiWrite: (addr, len) => BIOS.rpcCall('SPI::write('+addr+', '+len+');'),
  biosMemWrite: (addr, data) => BIOS.rpcCall('RPC::MemoryWrite(0x'+addr.toString(16)+', "'+data.map(i => ("0"+i.toString(16)).substr(-2) ).join("")+'");'),
  safeeval: (json) => { if (json[0] == "{") return eval("("+json+")") },
  retval: (json) => { var j = BIOS.safeeval(json); if (j && typeof(j.ret) != "undefined") return j.ret },
  biosMemGetBufferPtr: () => BIOS.rpcCall('RPC::GetBufferPtr();').then( json => BIOS.retval(json) ), 
  biosLcdBuffer: (x1, y1, x2, y2, pixels) =>
  {
    var numpix = pixels.length / 2;
    return BIOS.rpcCall('LCD::BufferBegin(CRect(' + x1+ ',' + y1 + ',' + x2 + ',' + y2 + '));')
      .then( () => BIOS.biosMemBulk(bufferptr, pixels) )
      .then( () => BIOS.biosLcdBufferWrite(bufferptr, numpix) )
  },
  biosLcdBufferWrite: (ptr, count) => BIOS.rpcCall('LCD::BufferWrite(' + ptr + ',' + count +');'),
  biosMemBulk: (addr, data) =>
  {
    var bulkLen = 40;
    if (data.length < bulkLen)
      return BIOS.biosMemWrite(addr, data);
    else
    {
      var section = data.splice(0, bulkLen);
      return BIOS.biosMemWrite(addr, section)
        .then( () => BIOS.biosMemBulk(addr + section.length, data));
    }
  },
  rpcCall: (command) =>
  {                 
    return new Promise((resolve, reject) =>
    {
      COMM.onReceive = (x) =>
      {         
        resolve(new TextDecoder().decode(x));
      }
      COMM.send(command);
    });
  },
  rpcCallBinary: (command) =>
  {                 
    return new Promise((resolve, reject) =>
    {
      COMM.onReceive = resolve;
      COMM.send(command);
    });
  }
};

