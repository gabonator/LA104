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

var MODEM = {
    SetFrequency: (arg) => BIOS.rpcCallR('CC1101::SetFrequency('+arg+');'),
    GetFrequency: () => BIOS.rpcCallR('CC1101::GetFrequency();'),
    SetBandwidth: (arg) => BIOS.rpcCallR('CC1101::SetBandwidth('+arg+');'),
    GetBandwidth: () => BIOS.rpcCallR('CC1101::GetBandwidth();'),
    SetBandwidth: (arg) => BIOS.rpcCallR('CC1101::SetBandwidth('+arg+');'),
    GetBandwidth: () => BIOS.rpcCallR('CC1101::GetBandwidth();'),
    SetGain: (arg) => BIOS.rpcCallR('CC1101::SetGain('+arg+');'),
    GetGain: () => BIOS.rpcCallR('CC1101::GetGain();'),
    SetDataRate: (arg) => BIOS.rpcCallR('CC1101::SetDataRate('+arg+');'),
    GetDataRate: (arg) => BIOS.rpcCallR('CC1101::GetDataRate();'),
    SetOutputPower: (arg) => BIOS.rpcCallR('CC1101::SetOutputPower('+arg+');'),
    GetOutputPower: (arg) => BIOS.rpcCallR('CC1101::GetOutputPower();'),

    Init: () => BIOS.rpcCallR('CC1101::Init();'),
    Start: () => BIOS.rpcCallR('CC1101::Start();'),
    Stop: () => BIOS.rpcCallR('CC1101::Stop();'),
    Status: () => BIOS.rpcCallR('CC1101::Status();'),
    Calibrate: () => BIOS.rpcCallR('CC1101::Calibrate();'),

    Send: (ptr, len, divisor) => BIOS.rpcCallR('CC1101::Send('+ptr+','+len+','+divisor+');'),

    Transfer: () => BIOS.rpcCallBinary("CC1101::Transfer();").then( (data) => new Promise((resolve, reject) =>
    { 
      var arr = [];
      if (data.byteLength == 7)
      {
        resolve([]);
        return;
      }

      if (data.byteLength & 1)
        throw "unaligned buffer";

      for (var i=0; i<data.byteLength; i+=2)
      {
        var word = data.getUint16(i, false);
        arr.push(word);
      }
      COMM.onReceive = () => {COMM.onReceive = null; resolve(arr);}; 
    }) ),

    Strobe: (arg) => BIOS.rpcCallR('CC1101::Strobe('+arg+');'), 
    Write: (addr, value) => BIOS.rpcCallR('CC1101::Write('+addr+","+value+');'), 
    Read: (addr) => BIOS.rpcCallR('CC1101::Read('+addr+');'), 
}

var APP =
{
  Info: () => new Promise( (resolve, reject) => 
  {
    var timeout = setTimeout(reject, 100);
    BIOS.rpcCallR('APP::Info()')
      .then( (ptr) => { clearTimeout(timeout); return BIOS.GetString(ptr); } )
      .then( resolve );
  }),
  GetConfigPtr: () => BIOS.rpcCall('APP::GetConfigPtr();').then( json => BIOS.retval(json) ),
  SetPrescaler: (arg) => BIOS.rpcCall('APP::SetPrescaler('+arg+');').then( json => BIOS.retval(json) ), 
};

var BIOS =
{
  memRead32: (addr) => BIOS.rpcCallR('MEM::Read32(0x'+addr.toString(16)+');'),
  memWrite32: (addr, val) => BIOS.rpcCallR('MEM::Write32(0x'+addr.toString(16)+','+val+');'),
  GetString: (addr, aux) =>
  {
    return BIOS.memRead32(addr).then( x =>
    {
      var chars = [x&255, (x>>8)&255, (x>>16)&255, x >> 24];
      if (!aux)
        aux = "";
      for (var i=0; i<chars.length; i++)
      {
        if (chars[i] == 0)
          return aux;
        aux += String.fromCharCode(chars[i]);
      }

      return BIOS.GetString(addr+4, aux);
    })
  },
  biosMemWrite: (addr, data) => BIOS.rpcCall('MEM::Write(0x'+addr.toString(16)+', "'+data.map(i => ("0"+i.toString(16)).substr(-2) ).join("")+'");'),
  biosMemBulkWrite: (addr, data) =>
  {
    data = [... data]; // copy
    var bulkLen = 16;
    if (data.length < bulkLen)
      return BIOS.biosMemWrite(addr, data);
    else
    {
      var section = data.splice(0, bulkLen);
      return BIOS.biosMemWrite(addr, section)
        .then( () => BIOS.biosMemBulkWrite(addr + section.length, data));
    }
  },
  safeeval: (json) => { if (json[0] == "{") return eval("("+json+")") },
  retval: (json) => { var j = BIOS.safeeval(json); if (j && typeof(j.ret) != "undefined") return j.ret },
  biosMemGetBufferPtr: () => BIOS.rpcCall('MEM::GetBufferPtr();').then( json => BIOS.retval(json) ), 
  biosLcdBuffer: (x1, y1, x2, y2, pixels) =>
  {
    var numpix = pixels.length / 2;
    return BIOS.rpcCall('LCD::BufferBegin(CRect(' + x1+ ',' + y1 + ',' + x2 + ',' + y2 + '));')
      .then( () => BIOS.biosMemBulk(bufferptr, pixels) )
      .then( () => BIOS.biosLcdBufferWrite(bufferptr, numpix) )
  },
  biosLcdBufferWrite: (ptr, count) => BIOS.rpcCall('LCD::BufferWrite(' + ptr + ',' + count +');'),
  rpcCall: (command) =>
  {                 
    return new Promise((resolve, reject) =>
    {
      COMM.onReceive = (x) =>
      {         
        COMM.onReceive = null;
        resolve(new TextDecoder().decode(x));
      }
      COMM.send(command);
    });
  },
  rpcCallR: (command) => BIOS.rpcCall(command).then( json => BIOS.retval(json) ),
  rpcCallBinary: (command) =>
  {                 
    return new Promise((resolve, reject) =>
    {
      COMM.onReceive = (d) => 
      {
        COMM.onReceive = null;
        resolve(d);
      }
      COMM.send(command);
    });
  }
};
