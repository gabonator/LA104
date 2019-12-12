
  //serial.ilovebill = true;  //20% cpu with free cycles, 44% with intensive communication

  var canvas = new Renderer(1200, 400);
  var controls = new Controls();
  var gui = new RemoteGui();
  var meas = new Measure();

  function Redraw(ofs, rawdata)
  {
    var ypos = (v) => canvas.height-v*(canvas.height/256);

    var data = [];
    for (var i =0; i<rawdata.length; i+=5)
    {
      var s = parseInt("0x" + rawdata.substr(i, 5));
      data.push([s&255, (s>>8)&255, (s>>16)&1, (s>>17)&1]);
    }

    var path1 = [];
    for (var i=0; i<data.length; i++)
      path1.push({x:ofs+i, y:ypos(data[i][0])});

    var path2 = [];
    for (var i=0; i<data.length; i++)
      path2.push({x:ofs+i, y:ypos(data[i][1])});

    canvas.Clear();

    var trig = ypos(INTERFACE.trigThreshold);
    var width = 4096;

    for (var x=0; x</*canvas.width*/ width; x+=30)
      canvas.Poly([{x:x, y:0}, {x:x, y:canvas.height}], "rgba(0, 0, 0, 0.8)", 1);

    for (var y=0; y<9; y++)
      canvas.Poly([{x:0, y:canvas.height*y/8}, {x:width, y:canvas.height*y/8}], "rgba(0, 0, 0, 0.8)", 1);

    canvas.Poly([{x:0, y:trig}, {x:width, y:trig}], "rgba(255, 255, 255, 0.6)", 1)
    canvas.Poly([{x:150-30, y:0}, {x:150-30, y:canvas.height}], "rgba(255, 255, 255, 0.6)", 1)


    canvas.Poly(path1, "#ffff00");
    canvas.Poly(path2, "#00ffff");
  }

  var init = false;
  var dummy = false;
  var promise = null;
  var promises = [];
  var bufferPtr = 0;
  var last = 0;
  var lastRequest = 0;
  var lastAsk = 0;

INTERFACE = {
  ch1range:"500mV",
  ch1coupling:"DC",
  ch1offset:20,

  ch2range:"1V",
  ch2coupling:"DC",
  ch2offset:100,

  timebase:"1ms",
  trigMode:"EdgeHL",
  trigTime:150,
  trigThreshold:128,
  trigSource:"CH1",
  trigState:"run",

  genFlavour: "Sine",
  genFrequency: 10000,
  genDuty: 50,

  // CH1          	
  save()
  {
    var data = {};
    for (var i in this)
    {
      if (typeof(this[i]) == "string" || typeof(this[i]) == "number")
        data[i] = this[i];
    }
    var settings = JSON.stringify(data);
    localStorage.setItem('settings', settings);
  },

  load()
  {
    var settings = localStorage.getItem('settings');
    var objs = JSON.parse(settings);
    for (var i in objs)
    {
      this[i] = objs[i];
    }
    this.trigState = "run";
  },

  setChannel1Range: (range) => 
  {
    INTERFACE.ch1range = range;
    console.log("ch1range: "+INTERFACE.ch1range);
    INTERFACE.configureInput("CH1");
    gui.drawChannel1();
  },

  setChannel1Coupling: (coupling) => 
  {
    INTERFACE.ch1coupling = coupling;
    console.log("ch1coupling: "+INTERFACE.ch1coupling);
    INTERFACE.configureInput("CH1");
    gui.drawChannel1();
  },

  setChannel1Offset: (offset) => 
  {
    if (typeof(offset) == "string")
      offset = parseInt(offset);
    if (!Number.isInteger(offset)) offset = 0;
    if (offset<0) offset = 0;
    if (offset>255) offset = 255;
    
    INTERFACE.ch1offset = offset;
    console.log("ch1offset: "+INTERFACE.ch1offset);
    INTERFACE.configureInput("CH1");
    canvas.setCh1Pos(offset);
  },

  // CH2
  setChannel2Range: (range) => 
  {
    INTERFACE.ch2range = range;
    console.log("ch2range: "+INTERFACE.ch2range);
    INTERFACE.configureInput("CH2");
    gui.drawChannel2();
  },

  setChannel2Coupling: (coupling) => 
  {
    INTERFACE.ch2coupling = coupling;
    console.log("ch2coupling: "+INTERFACE.ch2coupling);
    INTERFACE.configureInput("CH2");
    gui.drawChannel2();
  },

  setChannel2Offset: (offset) => 
  {
    if (typeof(offset) == "string")
      offset = parseInt(offset);
    if (!Number.isInteger(offset)) offset = 0;
    if (offset<0) offset = 0;
    if (offset>255) offset = 255;
    
    INTERFACE.ch2offset = offset;
    console.log("ch2offset: "+INTERFACE.ch2offset);
    INTERFACE.configureInput("CH2");
    canvas.setCh2Pos(offset);
  },

  configureInput: (ch) =>
  {                             
    if (ch=="CH1")
      INTERFACE.process( () => OSC.ConfigureInput(OSC.Enums["CH1"], OSC.Enums[INTERFACE.ch1coupling], OSC.Enums[INTERFACE.ch1range], INTERFACE.ch1offset) )
    if (ch=="CH2")
      INTERFACE.process( () => OSC.ConfigureInput(OSC.Enums["CH2"], OSC.Enums[INTERFACE.ch2coupling], OSC.Enums[INTERFACE.ch2range], INTERFACE.ch2offset) )

    INTERFACE.restart();
    INTERFACE.save();
  },

  // TIMEBASE
  setTimebase: (t) => 
  {
    INTERFACE.timebase = t;
    console.log("timebase: "+INTERFACE.timebase);
    INTERFACE.configureTimebase();
  },

  configureTimebase: () =>
  {    
    INTERFACE.process( () => OSC.ConfigureTimebase(OSC.Enums[INTERFACE.timebase]) )
    INTERFACE.process( () => OSC.ConfigureTimebase(OSC.Enums[INTERFACE.timebase]) )
    gui.drawTimebase();
    INTERFACE.save();
  },

  // TRIGGER
  setTriggerMode: (m) =>
  {
    INTERFACE.trigMode = m;
    console.log("triggerMode: "+INTERFACE.trigMode);
    INTERFACE.configureTrigger();
    gui.drawTrigger();
  },

  setTriggerThreshold: (thr) =>
  {
    INTERFACE.trigThreshold = thr;
    console.log("triggerThreshold: "+INTERFACE.trigThreshold);
    INTERFACE.configureTrigger();
    canvas.setTrigPos(thr);
  },

  setTriggerTime: (time) =>
  {
    INTERFACE.trigTime = time;
    console.log("triggerTime: "+INTERFACE.trigTime);
    INTERFACE.configureTrigger();
  },

  setTriggerSource: (source) =>
  {
    INTERFACE.trigSource = source;
    console.log("triggerSource: "+INTERFACE.trigSource);
    INTERFACE.configureTrigger();
    gui.drawTrigger();
  },

  toggleTriggerState()
  {
    if (INTERFACE.trigState == "run")
      INTERFACE.trigState = "paused"
    else
      INTERFACE.trigState = "run"
    gui.drawTrigger();
  },

  configureTrigger: () =>
  {
    INTERFACE.process( () => OSC.ConfigureTrigger(INTERFACE.trigTime, INTERFACE.trigThreshold, 
      OSC.Enums[INTERFACE.trigMode], OSC.Enums[INTERFACE.trigSource]) );
    INTERFACE.restart();
    INTERFACE.save();
  },

  // GENERATOR
  setGeneratorFlavour(flav)
  {
    INTERFACE.genFlavour = flav;
    INTERFACE.updateGenerator()
  },

  updateGenerator()
  {
    if (INTERFACE.genFlavour == "Sine")
    {
      setSineWave(INTERFACE.genFrequency);
    }
    if (INTERFACE.genFlavour == "Square")
    {
      setSquareWave(INTERFACE.genFrequency);
    }
    if (INTERFACE.genFlavour == "Triangle")
    {
      setTriangleWave(INTERFACE.genFrequency);
    }
    if (INTERFACE.genFlavour == "Sawtooth")
    {
      setSawtoothWave(INTERFACE.genFrequency);
    }
    gui.drawGenerator();
    INTERFACE.save();
  },

  setGeneratorFrequency(freq)
  {
    INTERFACE.genFrequency = freq;
    INTERFACE.updateGenerator();
  },

  setGeneratorDuty(duty)
  {
    INTERFACE.genDuty = duty;
    INTERFACE.process(() => GEN.SetDuty(INTERFACE.genDuty));
    gui.drawGenerator();
    INTERFACE.save();
  },
  // COMMON

  setAll: () =>
  {
    gui.redraw();
    canvas.setCh1Pos(INTERFACE.ch1offset);
    canvas.setCh2Pos(INTERFACE.ch2offset);
    canvas.setTrigPos(INTERFACE.trigThreshold);
    INTERFACE.configureInput("CH1");
    INTERFACE.configureInput("CH2");
    INTERFACE.configureTrigger();
    INTERFACE.configureTimebase()
    INTERFACE.restart();
    INTERFACE.updateGenerator();
  },

  onTrigger:()=> 
  {
    canvas.onStop();
  },
  process: (proc) => promises.push(proc),
  restart: () => promises.push(() => OSC.Restart())
};

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
    var n = 38*4;       
    var samples = [];
    for (var i=0; i<n; i++)
    {
      var sample = Math.sin(Math.PI*2*i/n);
      sample = (sample * (0x1000/2-20)) + 0x1000/2;
      samples.push(sample);
    }
    return transferWave(samples, freq);
  }
                                     	
  function setTriangleWave(freq)
  {
    var n = 38*4;       
    var samples = [];
    for (var i=0; i<n; i++)
    {
      var sample = 0;
      if (i<n/2)
        sample = i*0x1000/(n/2);
      else
        sample = (n-i)*0x1000/(n/2);
      samples.push(sample);
    }
    return transferWave(samples, freq);
  }

  function setSawtoothWave(freq)
  {
    var n = 38*4;       
    var samples = [];
    for (var i=0; i<n; i++)
    {
      var sample = i*0x1000/n;
      samples.push(sample);
    }
    return transferWave(samples, freq);
  }

  function transferWave(samples, freq)
  {
    var buf = [];
    for (var i=0; i<samples.length; i++)
    {
      sample = Math.floor(samples[i]);
      if (sample < 0) sample = 0;
      if (sample > 0xfff) sample = 0xfff;
      buf.push(sample & 255);
      buf.push(sample >> 8);
    }

    promises.push(() => 
      Promise.resolve()
      .then( ()=>BIOS.biosMemBulk(bufferPtr, buf) )
      .then( ()=>GEN.SetWave(bufferPtr, samples.length-1) ) // WTF??? bios error, wont configure DMA without changing buffer length
      .then( ()=>GEN.SetWave(bufferPtr, samples.length) )
      .then( ()=>GEN.SetFrequency(freq) )
      .then( ()=>GEN.GetFrequency() )
      .then( (f)=>console.log("Sample playing frequency="+f + ", sequence frequency="+Math.floor(f/samples.length)) )
    );
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
        promise = true;
        BIOS.biosMemGetBufferPtr();
        COMM._onReceive = () => 0;

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
          .then( () => OSC.Enable(1) )
          .then( () => INTERFACE.setAll() )
        );

        init = true;
        return;
      }

      if (promises.length > 0)
      {
        promise = promises.shift()().then( () => promise = false );
        return;
      }

      if (INTERFACE.trigState != "stop")
      {
        var dt = parseFloat(OSC.Enums[INTERFACE.timebase]);
        var dv = [50e-3, 100e-3, 200e-3, 500e-3, 1, 2, 5, 10][OSC.Enums[INTERFACE.ch1range]];

        var scroll = canvas.getScrollOffset();
        var now = (new Date()).getTime();
        var forceFreerunRedraw = INTERFACE.trigMode == "None" && now - last > 500;
        var forceAutoRedraw = INTERFACE.trigState == "run" && now - last > 2000;
        var forcePausedRedraw = INTERFACE.trigState == "paused" && lastRequest != scroll && now - last > 100;

        if (INTERFACE.trigState == "paused" && !forcePausedRedraw)
          return;
        if (now-lastAsk < 50)
          return;
        lastAsk = now;
        promise = Promise.resolve()
          .then( () => OSC.Ready() )
          .then( (ready) =>  
          { 
            var forceFinishedRedraw = INTERFACE.trigState == "run" && ready;

            if (INTERFACE.trigState == "stop") 
            {
            } else
            if (INTERFACE.trigState == "single") 
            {
                if (ready) 
                {
                  INTERFACE.onTrigger();
                  INTERFACE.trigState = "stop";

                  return Promise.resolve()
                  .then( () => OSC.Transfer(30, 4096-30) )
                  .then( data => { controls.setMeasData(meas.Calculate("CH1", dt, dv, data))	; return data;} )
                  .then( data => Redraw(0, data) )
                  .catch( () => { console.log("err"); return Promise.resolve()} );
                }
            } else
            if (forceFinishedRedraw || forceAutoRedraw || forceFreerunRedraw || forcePausedRedraw)
            {
              last = now;
              lastRequest = scroll;

              return Promise.resolve()
              .then( () => OSC.Transfer(150*0+30 + scroll, 256*4+180) )
              .then( data => { controls.setMeasData(meas.Calculate("CH1", dt, dv, data)); return Promise.resolve(data);} )
              .then( data => Redraw(scroll, data) )
              .then( () => 
              { 
                if ((ready && INTERFACE.trigState == "run") || forceAutoRedraw) 
                  return OSC.Restart(); 
              } )
              .then( () => Promise.resolve() )
              .catch( () => { console.log("err"); return Promise.resolve()} );
            }
            return Promise.resolve();
          })
          .then( () => promise = false );
      }
    }
  }, 10);


INTERFACE.load();
controls.load();




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
