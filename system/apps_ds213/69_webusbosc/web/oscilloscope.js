
  var canvas = new Renderer(1200, 400);
  var controls = new Controls();

  function Redraw(rawdata)
  {
    var data = [];
    for (var i =0; i<rawdata.length; i+=5)
    {
      var s = parseInt("0x" + rawdata.substr(i, 5));
      data.push([s&255, (s>>8)&255, (s>>16)&1, (s>>17)&1]);
    }

    var path1 = [];
    for (var i=0; i<data.length; i++)
      path1.push({x:i, y:400-data[i][0]*(400/256)});

    var path2 = [];
    for (var i=0; i<data.length; i++)
      path2.push({x:i, y:400-data[i][1]*(400/256)});

    canvas.Clear();
    canvas.Poly(path1, "#ffff00");
    canvas.Poly(path2, "#00ffff");
//    canvas.Poly(path1, "rgba(255, 255, 0, 0.8)");
//    canvas.Poly(path2, "rgba(0, 255, 255, 0.8)");
  }

  var init = false;
  var dummy = false;
  var promise = null;
  var promises = [];
  var bufferPtr = 0;
  var last = 0;

INTERFACE = {
  ch1range:"500mV",
  ch1coupling:"DC",
  ch1offset:20,

  ch2range:"1V",
  ch2coupling:"DC",
  ch2offset:100,

  timebase:"1ms",
  
  setChannel1Range: (range) => 
  {
    console.log("ch1range: "+range);
    INTERFACE.ch1range = range;
    INTERFACE.process( () => OSC.ConfigureInput(OSC.Enums["CH1"], OSC.Enums[INTERFACE.ch1coupling], OSC.Enums[INTERFACE.ch1range], INTERFACE.ch1offset) )
    INTERFACE.restart();
  },

  setChannel1Coupling: (coupling) => 
  {
    INTERFACE.ch1coupling = coupling;
    console.log("ch1coupling: "+INTERFACE.ch1coupling);
    INTERFACE.process( () => OSC.ConfigureInput(OSC.Enums["CH1"], OSC.Enums[INTERFACE.ch1coupling], OSC.Enums[INTERFACE.ch1range], INTERFACE.ch1offset) )
    INTERFACE.restart();
  },

  setChannel1Offset: (offset) => 
  {
    if (typeof(offset) == "string")
      offset = parseInt(offset);
    if (!Number.isInteger(offset)) offset = 0;
    if (offset<0) offset = 0;
    if (offset>255) offset = 255;
    console.log("ch1offset: "+offset);
    
    INTERFACE.ch1offset = offset;
    INTERFACE.process( () => OSC.ConfigureInput(OSC.Enums["CH1"], OSC.Enums[INTERFACE.ch1coupling], OSC.Enums[INTERFACE.ch1range], INTERFACE.ch1offset) )
    INTERFACE.restart();
  },

  setTimebase: (t) => 
  {
    timebase = t;
    console.log("timebase: "+timebase);
    INTERFACE.process( () => OSC.ConfigureTimebase(OSC.Enums[timebase]) )
    INTERFACE.process( () => OSC.ConfigureTimebase(OSC.Enums[timebase]) ) // TODO: fix
    INTERFACE.restart();
  },

  process: (proc) => promises.push(proc),
  restart: () => promises.push(() => OSC.Restart())
};


  function setScan()
  {
    promises.push(() => OSC.ConfigureTrigger(150, 100, OSC.Enums["None"], OSC.Enums["CH1"]) );

  }
  function setTimebase(t)
  {
    promises.push(() => Promise.resolve()
//            .then(() => OSC.Enable(1))
            .then(() => OSC.ConfigureTimebase(OSC.Enums[t]))
            .then(() => OSC.ConfigureTimebase(OSC.Enums[t]))
//            .then(() => OSC.Restart() )
    );
  }

  function setSquareWave(freq, duty)
  {
    promises.push(() => 
      Promise.resolve()
      .then( ()=>GEN.SetWave(0, 0) )
      .then( ()=>GEN.SetFrequency(freq) )
      .then( ()=>{ if (duty) return GEN.SetDuty(duty); } )
      .then( ()=>GEN.GetFrequency() )
      .then( (f)=>console.log("Frequency="+f) )
    );
  }

  function setSineWave(freq)
  {
/*
    var pSineWave = [0x000,0x027,0x08E,0x130,0x209,0x311,0x441,0x58F,0x6F0,    // 90
			0x85A,0x9C0,0xB19,0xC59,0xD76,0xE68,0xF26,0xFAB,0xFF3,    // 180
			0xFFF,0xFD7,0xF70,0xECE,0xDF5,0xCED,0xBBD,0xA6F,0x90E,    // 270
			0x7A4,0x63E,0x4E5,0x3A5,0x288,0x196,0x0D8,0x053,0x00B];   // 360   
    var buf = [];
    var freq = f;//1000;
    for (var i=0; i<pSineWave.length; i++)
    {
      var sample = pSineWave[i];
//      sample = Math.floor(0xfff * i / pSineWave.length);
      buf.push(sample & 255);
      buf.push(sample >> 8);
    }
*/
    var n = 38*4;       
    var buf = [];
    for (var i=0; i<n; i++)
    {
      var sample = Math.sin(Math.PI*2*i/n);
      sample = (sample * (0x1000/2-20)) + 0x1000/2;
      sample = Math.floor(sample);
      if (sample < 0) sample = 0;
      if (sample > 0xfff) sample = 0xfff;
      buf.push(sample & 255);
      buf.push(sample >> 8);
    }
    promises.push(() => 
      Promise.resolve()
      .then( ()=>BIOS.biosMemBulk(bufferPtr, buf) )
      .then( ()=>GEN.SetWave(bufferPtr, n-1) )
      .then( ()=>GEN.SetWave(bufferPtr, n) )
//      .then( ()=>GEN.SetWave(0, 0) )
      .then( ()=>GEN.SetFrequency(freq) )
//      .then( ()=>GEN.GetFrequency() )
//      .then( (f)=>console.log("Frequency="+f) )
    );
  }

  function setCh1(resol)
  {
    promises.push(() => OSC.ConfigureInput(OSC.Enums["CH1"], OSC.Enums["DC"], OSC.Enums[resol], 50) );
  }

  setInterval(() =>
  {
    if (COMM._open)
    {
      if (promise)
        return;

      if (!dummy)
      {
        /// first request is without response?
        promise = BIOS.biosMemGetBufferPtr();

        setTimeout(() => promise = false, 300);
        dummy = true;
        return;
      }

      if (!init)
      {
        promises.push(() => 
          Promise.resolve()
          .then( () => BIOS.biosMemGetBufferPtr() )
          .then( ptr => bufferPtr = ptr)

//          .then( () => OSC.ConfigureTrigger(0, 0, OSC.Enums["None"], OSC.Enums["CH1"]) )
          .then( () => OSC.Enable(1) )
          .then( () => OSC.ConfigureTimebase(OSC.Enums[INTERFACE.timebase]) )
          .then( () => OSC.ConfigureTimebase(OSC.Enums[INTERFACE.timebase]) ) // bug in bios
          .then( () => OSC.ConfigureTrigger(150, 100, OSC.Enums["EdgeHL"], OSC.Enums["CH1"]) )
          .then( () => OSC.ConfigureInput(OSC.Enums["CH1"], OSC.Enums["DC"], OSC.Enums["500mV"], 20) )
          .then( () => OSC.ConfigureInput(OSC.Enums["CH2"], OSC.Enums["DC"], OSC.Enums["100mV"], 50) )
          .then( () => OSC.Restart() )
        );

        setSineWave(10000);

        init = true;
        return;
      }

      if (promises.length > 0)
      {
        promise = promises.shift()().then( () => promise = false );
        return;
      }

      promise = Promise.resolve()
        .then( () => OSC.Ready() )
//        .then( console.log )
//        .then( () => true )
        .then( (ready) =>  
        { 
          var now = (new Date()).getTime();
          if (ready || now - last > 2000)
          {
            last = now;
            return Promise.resolve()
            .then( () => OSC.Transfer(150, 256*4) )
            .then( data => Redraw(data) )
            .then( () => { if (ready) return OSC.Restart(); else return Promise.resolve();} )
            .then( () => Promise.resolve() )
            .catch( () => { console.log("err"); return Promise.resolve()} );
          }
          return Promise.resolve();
        })
        .then( () => promise = false );
    }
  }, 100);








///////////


  document.addEventListener('DOMContentLoaded', event => {
    let connectButton = document.querySelector("#connect");
    let statusDisplay = document.querySelector('#status');
    let port;

    let ascii = (s => s.split('').map(c=>c.charCodeAt(0)) );
    
    function connect() {

COMM._send = function(msg)
{
  if (COMM.debug)
    console.log("> " + msg);
  if (msg.indexOf("undefined") != -1) 
  { 
    console.log(">>>> ignoring command >>>> " + msg);
    return;
  }
  port.send(Uint8Array.from(ascii(msg)));
}

      port.connect().then(() => {
        COMM._open = true;

        statusDisplay.textContent = '';
        connectButton.textContent = 'Disconnect';

        port.onReceive = data => {
          let textDecoder = new TextDecoder();
          let msg= textDecoder.decode(data)

          if (COMM.debug)
            console.log("< " + msg);

          COMM._onReceive(msg);
        }
        port.onReceiveError = error => {
          console.error(error);
        };
      }/*, error => {
        statusDisplay.textContent = error;
      }*/);
    }

    function onUpdateLed() {
      if (!port) {
        return;
      }
      toggle();     
    };

    connectButton.addEventListener('click', function() {
      if (port) {
        port.disconnect();
        COMM._open = false;
        connectButton.textContent = 'Connect';
        statusDisplay.textContent = '';
        port = null;
      } else {
        serial.requestPort().then(selectedPort => {
          port = selectedPort;
          connect();
        }).catch(error => {
          statusDisplay.textContent = error;
        });
      }
    });

    serial.getPorts().then(ports => {
      if (ports.length == 0) {
        statusDisplay.textContent = 'No device found.';
      } else {
        statusDisplay.textContent = 'Connecting...';
        port = ports[0];
        connect();
      }
    });
  });
