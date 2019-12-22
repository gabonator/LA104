
  //serial.ilovebill = true;  //20% cpu with free cycles, 44% with intensive communication

  var canvas = new Renderer(1200, 400);
  var controls = new Controls();
  var gui = new RemoteGui();
  var meas = new Measure();



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
  genEquation: "Math.cos(x)",

  measSource: "CH1",

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

                  return Promise.resolve()
                  .then( () => OSC.Transfer(30, 4096-30) )
                  .then( data => { INTERFACE.measure = meas.Calculate(INTERFACE.measSource, dt, dv, data); controls.setMeasData(INTERFACE.measure); return data;} )
                  .then( data => canvas.OscilloscopeRedraw(0, data) )
                  .catch( () => { console.log("err"); return Promise.resolve()} );
                }
            } else
            if (forceFinishedRedraw || forceAutoRedraw || forceFreerunRedraw || forcePausedRedraw)
            {
              last = now;
              lastRequest = scroll;

              var maxSafe = 4096-30-15;
              var reqStart = canvas.OscilloscopeResampleNum(scroll)+30;
              var reqLen = 256*4+180*1; // screen width, ignore resampling, always take 1k
              if (reqStart > maxSafe - 128)
                reqStart = maxSafe - 128;
              if (reqLen+reqStart > maxSafe)
                reqLen = maxSafe - reqStart;

              return Promise.resolve()
              .then( () => OSC.Transfer(reqStart, reqLen) )
              .then( data => { INTERFACE.measure = meas.Calculate(INTERFACE.measSource, dt, dv, data); controls.setMeasData(INTERFACE.measure); return Promise.resolve(data);} )
              .then( data => canvas.OscilloscopeRedraw(scroll, data) )
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

