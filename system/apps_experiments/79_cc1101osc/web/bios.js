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

    Init: () => BIOS.rpcCallR('CC1101::Init();'),
    Start: () => BIOS.rpcCallR('CC1101::Start();'),
    Stop: () => BIOS.rpcCallR('CC1101::Stop();'),
    Status: () => BIOS.rpcCallR('CC1101::Status();'),

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

}

var BIOS =
{
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
  rpcCallR: (command) => BIOS.rpcCall(command).then( json => BIOS.retval(json) ),
  rpcCallBinary: (command) =>
  {                 
    return new Promise((resolve, reject) =>
    {
      COMM.onReceive = resolve;
      COMM.send(command);
    });
  }
};
