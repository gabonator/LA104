var COMM = {
  debug: false,
  _open: false,
  _send: () => 0,
  _onReceive: () => 0
};

var OSC = {
  Enums: {"CH1": 0, "CH2": 1, "CH3": 2, "CH4": 3,
    // coupling
    "DC": 0, "AC": 0, 
    // resolution
    "50mV":0, "100mV":1, "200mV":2, "500mV":3, "1V":4, "2V":5, "5V":6, "10V":7,
    // trigger
    "EdgeHL":0, "EdgeLH":1, "LevelLow":2, "LevelHigh":3, "LowerDTLow":4, "GreaterDTLow":5, "LowerDTHigh":6,
    "GreaterDTHigh":7, "None":8},
/*
		{100e-9f, 200e-9f, 500e-9f,
		1e-6f, 2e-6f, 5e-6f,
		10e-6f, 20e-6f, 50e-6f, 100e-6f, 200e-6f, 500e-6f,
		1e-3f, 2e-3f, 5e-3f,
		10e-3f, 20e-3f, 50e-3f, 100e-3f, 200e-3f, 500e-3f,
		1.0f};
*/

  Enable: (enable) => BIOS.rpcCall('OSC::Enable('+enable+');'),
  ConfigureTrigger: (time, value, type, source) => BIOS.rpcCall('OSC::ConfigureTrigger('+time+','+value+','+type+','+source+');'),
  ConfigureTimebase: (timebase) => BIOS.rpcCall('OSC::ConfigureTimebase('+timebase+');'),
  ConfigureInput: (input, couple, res, offset) => BIOS.rpcCall('OSC::ConfigureInput('+input+','+couple+','+res+','+offset+');'),
  Ready: () => BIOS.rpcCall('OSC::Ready();').then( json => BIOS.safeeval(json).ret ),
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
          COMM._onReceive = () => 0; // ignore trailing {ret:xxx}
          return data;
        })
    });
  }               
  
};

var BIOS =
{
  biosSpiBegin: (baud) => BIOS.rpcCall('SPI::begin('+baud+');'),
  biosSpiWrite: (addr, len) => BIOS.rpcCall('SPI::write('+addr+', '+len+');'),
  biosMemWrite: (addr, data) => BIOS.rpcCall('RPC::MemoryWrite(0x'+addr.toString(16)+', "'+data.map(i => ("0"+i.toString(16)).substr(-2) ).join("")+'");'),
  safeeval: (json) => eval("("+json+")"),
  biosMemGetBufferPtr: () => BIOS.rpcCall('RPC::GetBufferPtr();').then( json => BIOS.safeeval(json).ret ), 
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

