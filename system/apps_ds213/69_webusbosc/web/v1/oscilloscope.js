
  //serial.ilovebill = true;  //20% cpu with free cycles, 44% with intensive communication

  var canvas = new Renderer(1200, 400);
  var controls = new Controls();
  var gui = new RemoteGui();
  var meas = new Measure();
  var analyser = new Analyser();



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
  genDc: 1.0,
  genEquation: "(sin(x)+abs(sin(x*2))*0.5)*0.6",
  // i/n*2-1
  // (sin(x)+sin(x*2)/2+sin(x*3)/3+sin(x*4)/4+sin(x*5)/5)*0.5
  // (0b111100001100110010101010 >> Math.floor(i/n*24))&1
  // (0b000000000001111000011001 >> Math.floor(24-i/n*24))&1
  // (0b111111110000000010101010 >> Math.floor(24-i/n*24))&1      // GreaterDTHigh, Time=100
  // (0b111100001111000011100010 >> Math.floor(24-i/n*24))&1      // LowerDTLow, Time=50
  // (0b11101110001011111111111111111111 >>> Math.floor(32-i/n*32))&1   // 'G' UART 0b01000111
  // (0b11101110001010011010101111111111 >>> Math.floor(32-i/n*32))&1   // 'GV' UART 0b01000111, 0b01010110
  // "11110111000101001101010111111111"[floor(i/n*32)]

  measSource: "CH1",

  analyse:false,

  memSlot:"0",

  // CH1          	
  save()
  {
    var data = {};
    for (var i in this)
    {
      if (i.substr(0, 1) == "_")
        continue;

      if (typeof(this[i]) == "string" || typeof(this[i]) == "number" || typeof(this[i]) == "boolean")
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
    controls.setCh1Range();
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
    offset = Math.max(CALIBRATION.getCh1ZeroMin(), offset);
    offset = Math.min(CALIBRATION.getCh1ZeroMax(), offset);
    
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
    controls.setCh2Range();
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
      INTERFACE.process( () => OSC.ConfigureInput(OSC.Enums["CH1"], OSC.Enums[INTERFACE.ch1coupling], OSC.Enums[INTERFACE.ch1range], CALIBRATION.getCh1Zero(INTERFACE.ch1offset)) )
    if (ch=="CH2")
      INTERFACE.process( () => OSC.ConfigureInput(OSC.Enums["CH2"], OSC.Enums[INTERFACE.ch2coupling], OSC.Enums[INTERFACE.ch2range], CALIBRATION.getCh2Zero(INTERFACE.ch2offset)) )

    INTERFACE.restart();
    INTERFACE.save();
  },

  // TIMEBASE
  setTimebase: (t) => 
  {
    INTERFACE.timebase = t;
    console.log("timebase: "+INTERFACE.timebase);
    INTERFACE.configureTimebase();
    console.log("resample="+OSC.ResampleTable[INTERFACE.timebase]);

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
    var thresh = INTERFACE.trigThreshold;

    if (INTERFACE.trigSource == "CH1")
      thresh = Math.floor(CALIBRATION.getCh1Inv(thresh));
    if (INTERFACE.trigSource == "CH2")
      thresh = Math.floor(CALIBRATION.getCh2Inv(thresh));

    INTERFACE.process( () => OSC.ConfigureTrigger(INTERFACE.trigTime, thresh, 
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
      INTERFACE.setGeneratorDuty(INTERFACE.genDuty);
    }
    if (INTERFACE.genFlavour == "Triangle")
    {
      setTriangleWave(INTERFACE.genFrequency);
    }
    if (INTERFACE.genFlavour == "Sawtooth")
    {
      setSawtoothWave(INTERFACE.genFrequency);
    }
    if (INTERFACE.genFlavour == "DC")
    {
      setDcWave(INTERFACE.genDc);
    }
    if (INTERFACE.genFlavour == "Equation")
    {
      setEquationWave(INTERFACE.genEquation, INTERFACE.genFrequency);
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

  setGeneratorDc(dc)
  {
    INTERFACE.genDc = dc;
    setDcWave(INTERFACE.genDc);
    gui.drawGenerator();
    INTERFACE.save();
  },

  setGeneratorEquation(eq)
  {
    INTERFACE.genEquation = eq;
    setEquationWave(INTERFACE.genEquation, INTERFACE.genFrequency);
    INTERFACE.save();
  },

  // MEASURE
  setMeasSource(src)
  {
    INTERFACE.measSource = src;
  },

  // ANALYSER
  setAnalyserMode(mode)
  {
    INTERFACE.analyse = mode == "UART";
  },

  // MEMORY/FILE
  memorySave()
  {
    INTERFACE._wave.oscSettings = {
      ch1range:INTERFACE.ch1range,
      ch1coupling:INTERFACE.ch1coupling,
      ch1offset:INTERFACE.ch1offset,
      ch2range:INTERFACE.ch2range,
      ch2coupling:INTERFACE.ch2coupling,
      ch2offset:INTERFACE.ch2offset,
      timebase:INTERFACE.timebase,
      trigMode:INTERFACE.trigMode,
      trigTime:INTERFACE.trigTime,
      trigThreshold:INTERFACE.trigThreshold,
      trigSource:INTERFACE.trigSource,

      genFlavour: INTERFACE.genFlavour,
      genFrequency: INTERFACE.genFrequency,
      genDuty: INTERFACE.genDuty,
      genDc: INTERFACE.genDc,
      genEquation: INTERFACE.genEquation
    };

    localStorage.setItem('wave' + INTERFACE.memSlot, JSON.stringify(INTERFACE._wave));

    INTERFACE.memoryPreview(INTERFACE.memSlot);
  },

  memoryLoad()
  {
    var _wave = localStorage.getItem('wave' + INTERFACE.memSlot);
    if (_wave)
    {
      INTERFACE.trigState = "stop";
      gui.drawTrigger();
      canvas.onStop();

      _wave = JSON.parse(_wave);
      for (var i in _wave.oscSettings)
        INTERFACE[i] = _wave.oscSettings[i];

      controls.load();
      INTERFACE.configureInput("CH1");
      INTERFACE.configureInput("CH2");
      INTERFACE.configureTimebase();
      INTERFACE.configureTrigger();
      INTERFACE.updateGenerator();

      delete _wave.oscSettings;

      var dt = parseFloat(OSC.Enums[INTERFACE.timebase]);
      var dv = [50e-3, 100e-3, 200e-3, 500e-3, 1, 2, 5, 10][OSC.Enums[INTERFACE.ch1range]];

      INTERFACE.process( () =>
        Promise.resolve()
//          .then( () => new Promise( (resolve) => setTimeout(resolve, 50) ) )  // finish previous transfer
          .then( () => INTERFACE._wave = _wave )
          .then( () => meas.Calculate(INTERFACE.measSource, dt, dv, INTERFACE._wave.data) )
          .then( (measure) => controls.setMeasData(measure) ) 
          .then( () => canvas.OscilloscopeRedraw(INTERFACE._wave.scroll, INTERFACE._wave.data) )
          .then( () => { if (INTERFACE.analyse) analyser.analyse(INTERFACE._wave.data); } )
          .then( () => OSC.Restart() )
      );

    }
  },

  memoryPreview(id)
  {
    INTERFACE.memSlot = id;
    var _wave = localStorage.getItem('wave' + INTERFACE.memSlot);
    if (_wave)
    {
      _wave = JSON.parse(_wave);
      controls.memPreview(_wave.data);
    } else
    {
      controls.memPreview("");
    }
  },

  memoryExport()
  {
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
    controls.setCh1Range();
    controls.setCh2Range();
  },

  onTrigger:()=> 
  {
    canvas.onStop();
  },
  process: (proc) => promises.push(proc),
  restart: () => promises.push(() => OSC.Restart())
};


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
                  gui.drawTrigger();

                  return Promise.resolve()
                  .then( () => OSC.Transfer(30, 4096-30) )
//!!!!                  .then( (data) => {if (INTERFACE.trigState == "stop") throw "cancelled"; return data; } )
                  .then( (data) => INTERFACE._wave = {scroll:0, start:reqStart, len:reqLen, data:data} )

                  .then( () => meas.Calculate(INTERFACE.measSource, dt, dv, INTERFACE._wave.data) )
                  .then( (measure) => controls.setMeasData(measure) ) 
                  .then( () => canvas.OscilloscopeRedraw(INTERFACE._wave.scroll, INTERFACE._wave.data) )
                  .catch( (e) => { if (e=="cancelled") return; console.log("err"); return Promise.resolve()} );
                }
            } else
            if (forceFinishedRedraw || forceAutoRedraw || forceFreerunRedraw || forcePausedRedraw)
            {
              last = now;
              lastRequest = scroll;

              var maxSafe = 4096-30-15;
              var reqStart = canvas.OscilloscopeResampleNum(scroll)+30;

              if (INTERFACE.trigMode == "None")
              {
                reqStart = Math.max(150, reqStart);
                scroll = Math.max(120, scroll);
              }

              var reqLen = 256*4+180*1; // screen width, ignore resampling, always take 1k
              if (reqStart > maxSafe - 128)
                reqStart = maxSafe - 128;
              if (reqLen+reqStart > maxSafe)
                reqLen = maxSafe - reqStart;

              return Promise.resolve()
              .then( () => OSC.Transfer(reqStart, reqLen) )
              .then( (data) => INTERFACE._wave = {scroll:scroll, start:reqStart, len:reqLen, data:data} )

              .then( () => meas.Calculate(INTERFACE.measSource, dt, dv, INTERFACE._wave.data) )
              .then( (measure) => controls.setMeasData(measure) ) 
              .then( () => canvas.OscilloscopeRedraw(INTERFACE._wave.scroll, INTERFACE._wave.data) )
              .then( () => { if (INTERFACE.analyse) analyser.analyse(INTERFACE._wave.data); } )

              .then( () => 
              { 
                if ((ready && INTERFACE.trigState == "run") || forceAutoRedraw) 
                  return OSC.Restart(); 
              } )
              .then( () => Promise.resolve() )
//              .catch( () => { console.log("err"); return Promise.resolve()} );
            }
            return Promise.resolve();
          })
          .then( () => promise = false );
      }
    }
  }, 10);


INTERFACE.load();
controls.load();
CALIBRATION.load();
INTERFACE.memoryPreview(INTERFACE.memSlot);