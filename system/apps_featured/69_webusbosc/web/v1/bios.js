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

var OSC = {
  Enums: {"CH1": 0, "CH2": 1, "CH3": 2, "CH4": 3,
    // coupling
    "DC": 0, "AC": 1, 
    // resolution
    "50mV":0, "100mV":1, "200mV":2, "500mV":3, "1V":4, "2V":5, "5V":6, "10V":7,
    // trigger
    "EdgeHL":0, "EdgeLH":1, "LevelLow":2, "LevelHigh":3, "LowerDTLow":4, "GreaterDTLow":5, "LowerDTHigh":6,
    "GreaterDTHigh":7, "None":8,
    // timebase
    "100ns":"100e-9", "200ns":"200e-9", "500ns":"500e-9", 
    "1us":"1e-6", "2us":"2e-6", "5us":"2e-6",
    "10us":"10e-6", "20us":"20e-6", "50us":"50e-6", "100us":"100e-6", "200us":"200e-6", "500us":"500e-6",
    "1ms":"1e-3", "2ms":"2e-3", "5ms":"5e-3",
    "10ms":"10e-3", "20ms":"20e-3", "50ms":"50e-3", "100ms":"100e-3", "200ms":"200e-3", "500ms":"500e-3", 
    "1s":"1.0"
  },
  ResampleTable:// florian
  {
    "100ns":1, "200ns":492/1024, "500ns":614/1024, 
    "1us":819/1024, "2us":983/1024, 
    "5us":2.4, "10us":1, "20us":1, "50us":1, "100us":1, "200us":1, "500us":1,
    "1ms":1, "2ms":1, "5ms":1, "10ms":1, "20ms":1, "50ms":1, "100ms":1, "200ms":1, "500ms":1, "1s":1
  },

  Enable: (enable) => BIOS.rpcCall('OSC::Enable('+enable+');'),
  ConfigureTrigger: (time, value, type, source) => BIOS.rpcCall('OSC::ConfigureTrigger('+time+','+value+','+type+','+source+');'),
  ConfigureTimebase: (timebase) => BIOS.rpcCall('OSC::ConfigureTimebase('+timebase+');'),
  ConfigureInput: (input, couple, res, offset) => BIOS.rpcCall('OSC::ConfigureInput('+input+','+couple+','+res+','+offset+');'),
  Ready: () => BIOS.rpcCall('OSC::Ready();').then( json => BIOS.retval(json) ),
  Restart: () => BIOS.rpcCall('OSC::Restart();'),
  GetPointer: () => BIOS.rpcCall('OSC::GetPointer();').then( json => BIOS.retval(json) ),
  Transfer: (begin, len) => 
  {
    var requestNibbles = 0;
    var requestBuffer = "";
    var requestData = () => new Promise((resolve, reject) =>
    {
      var safety = setTimeout(reject, 1000);

      COMM._onReceive = data => {
        requestBuffer += data;
        requestNibbles -= data.length;

//console.log(requestNibbles);
        if (requestNibbles < 0)
          throw "wrong count";

        if (requestNibbles == 0)
        {
          clearTimeout(safety);
          resolve(requestBuffer);
        }
      }
    });

    return BIOS.rpcCall('OSC::Transfer('+begin+','+len+');')
    .then((json) => 
    {
      requestNibbles = BIOS.safeeval(json).hex;
      return requestData()
        .then((data) =>
        {
          return new Promise((resolve, reject) =>
          {
            COMM._onReceive = () => resolve(data);
          });
//          COMM._onReceive = () => 0; // ignore trailing {ret:xxx}
//          return data;
        })
    });
  }               
  
};


var GEN =
{
  SetFrequency: (freq) => BIOS.rpcCall('GEN::SetFrequency('+freq+');'),
  GetFrequency: () => BIOS.rpcCall('GEN::GetFrequency();').then( json => BIOS.retval(json) ),
  SetDuty: (duty) => BIOS.rpcCall('GEN::SetDuty('+duty+');'),
  SetWave: (ptr, len) => BIOS.rpcCall('GEN::SetWave('+ptr+','+len+');'),
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
      COMM._onReceive = resolve;
      COMM._send(command);
    });
  }
};

