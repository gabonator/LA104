CALIBRATION = {
  gen1V: 1455,   // generator set to 1.000V, test by calling "transferWave([1455], 1000);"
  gen2V: 2935,   // generator set to 2.000V

  ch1ZeroCurve: [[28,0],[40,16],[53,32],[68,48],[82,64],[97,80],[112,96],[127,112],[142,128],[157,144],[172,160],[188,176],[203,192],[218,208],[233,224],[248,240]], //[[0,0], [255,255]],
  ch1Scale: {"50mV":1, "100mV":1.3736, "200mV":1.3986, "500mV":1.360, "1V":1.388, "2V":1.4286, "5V":1.4286, "10V":1.3298},

  ch2ZeroCurve: [[19,0],[32,16],[45,32],[61,48],[75,64],[91,80],[105,96],[121,112],[136,128],[151,144],[167,160],[183,176],[197,192],[213,208],[229,224],[244,240]],
  ch2Scale: {"50mV":1, "100mV":1.3736, "200mV":1.3986, "500mV":1.360, "1V":1.388, "2V":1.4286, "5V":1.4286, "10V":1.3298},

  getCh1: (y) =>
  {
     return (y - INTERFACE.ch1offset) * CALIBRATION.ch1Scale[INTERFACE.ch1range] + INTERFACE.ch1offset;
  },

  getCh1Inv: (out) =>
  {
     return (out - INTERFACE.ch1offset)/CALIBRATION.ch1Scale[INTERFACE.ch1range] + INTERFACE.ch1offset;
  },

  getCh1Zero: (ypos) =>
  {
    var curve = CALIBRATION.ch1ZeroCurve;
    if (ypos < curve[0][0])
      return curve[0][1];
    if (ypos >= curve[curve.length-1][0])
      return curve[curve.length-1][1];

    for (var i=0; i<curve.length-1; i++)
    {
      if (ypos >= curve[i][0] && ypos < curve[i+1][0])
      {
        return Math.floor((ypos-curve[i][0])/(curve[i+1][0]-curve[i][0])
          * (curve[i+1][1]-curve[i][1]) + curve[i][1]);
      }
    }
    throw "calib failed"
  },

  getCh1ZeroMin: () => CALIBRATION.ch1ZeroCurve[0][0],
  getCh1ZeroMax: () => CALIBRATION.ch1ZeroCurve[CALIBRATION.ch1ZeroCurve.length-1][0],

  getCh2: (y) =>
  {
     return (y - INTERFACE.ch2offset) * CALIBRATION.ch2Scale[INTERFACE.ch2range] + INTERFACE.ch2offset;
  },

  getCh2Inv: (out) =>
  {
     return (out - INTERFACE.ch2offset)/CALIBRATION.ch2Scale[INTERFACE.ch2range] + INTERFACE.ch2offset;
  },

  getCh2Zero: (ypos) =>
  {
    var curve = CALIBRATION.ch2ZeroCurve;
    if (ypos < curve[0][0])
      return curve[0][1];
    if (ypos >= curve[curve.length-1][0])
      return curve[curve.length-1][1];

    for (var i=0; i<curve.length-1; i++)
    {
      if (ypos >= curve[i][0] && ypos < curve[i+1][0])
      {
        return Math.floor((ypos-curve[i][0])/(curve[i+1][0]-curve[i][0])
          * (curve[i+1][1]-curve[i][1]) + curve[i][1]);
      }
    }
    throw "calib failed"
  },

  getCh2ZeroMin: () => CALIBRATION.ch2ZeroCurve[0][0],
  getCh2ZeroMax: () => CALIBRATION.ch2ZeroCurve[CALIBRATION.ch2ZeroCurve.length-1][0],

  setCh1K: (k) =>
  {
    CALIBRATION.ch1Scale[INTERFACE.ch1range] = k;
    CALIBRATION.save();
  },

  setCh2K: (k) =>
  {
    CALIBRATION.ch2Scale[INTERFACE.ch1range] = k;
    CALIBRATION.save();
  },

  save()
  {
    var data = {};
    for (var i in this)
    {
      if (typeof(this[i]) != "function")
        data[i] = this[i];
    }
    var settings = JSON.stringify(data);
    localStorage.setItem('calibration', settings);
  },

  load()
  {
    var settings = localStorage.getItem('calibration');
    var objs = JSON.parse(settings);
    for (var i in objs)
    {
      this[i] = objs[i];
    }
  },
};

class Calibration
{ 
  constructor()
  {
    this.calibCurve = null;
  }

  startOffsetCalibration(ch)
  {
    this.calibCurve = [];

    INTERFACE.process(() => this.setStatisticMode());
    for (var pos=0; pos<256; pos+=16)
    {
      if (ch == "CH1")
      {
        (function(pos)
        {
           INTERFACE.process(() => Promise.resolve()
             .then(() => this.setCh1Offset(pos) )
             .then(() => this.getCh1Statistics())
             .then((stats) => {console.log([pos, stats]); this.calibCurve.push([pos, Math.floor(stats)])})
           );
        }).bind(this)(pos);
      }
      if (ch == "CH2")
      {
        (function(pos)
        {
           INTERFACE.process(() => Promise.resolve()
             .then(() => this.setCh2Offset(pos) )
             .then(() => this.getCh2Statistics())
             .then((stats) => {console.log([pos, stats]); this.calibCurve.push([pos, Math.floor(stats)])})
           );
        }).bind(this)(pos);
      }
    }
    INTERFACE.process(() => this.finishOffsetCalibration(ch));
  }

  finishOffsetCalibration(ch)
  {
    var curveValueToOffset = this.calibCurve;
    var curveOffsetToValue = [];
    for (var i = 0; i<curveValueToOffset.length; i++)
      curveOffsetToValue.push([curveValueToOffset[i][1], curveValueToOffset[i][0]]);

    if (ch == "CH1")
      CALIBRATION.ch1ZeroCurve = curveOffsetToValue;
    if (ch == "CH2")
      CALIBRATION.ch2ZeroCurve = curveOffsetToValue;

    CALIBRATION.save();

    return Promise.resolve(); // restore settings
  }

  setCh1Offset(ofs)
  {
    return Promise.resolve()
      .then( () => OSC.ConfigureInput(OSC.Enums["CH1"], OSC.Enums["DC"], OSC.Enums[INTERFACE.ch1range], ofs) )
      .then( () => OSC.Restart() )
      .then( () => canvas.setCh1Pos(ofs));
  }

  setCh2Offset(ofs)
  {
    return Promise.resolve()
      .then( () => OSC.ConfigureInput(OSC.Enums["CH2"], OSC.Enums["DC"], OSC.Enums[INTERFACE.ch2range], ofs) )
      .then( () => OSC.Restart() )
      .then( () => canvas.setCh2Pos(ofs));
  }
  
  setStatisticMode()
  {            
    return Promise.resolve()
      .then( () => INTERFACE.trigState = "run" )
      .then( () => OSC.ConfigureTimebase(OSC.Enums["1us"]) )
      .then( () => OSC.ConfigureTrigger(100, 128, OSC.Enums["None"], OSC.Enums["CH1"]) )
      .then( () => OSC.Restart() );
  }

  getCh1Statistics()
  {
    return Promise.resolve()
      .then( () => OSC.Restart() )
      .then( () => this.delay(100) )
      .then( () => this.transfer() )
      .then( () => this.delay(100) )
      .then( () => this.transfer() )
      .then( () => this.delay(100) )
      .then( () => this.transfer("CH1") )
      .then( (measure) => measure.rawAverage )
  }

  getCh2Statistics()
  {
    return Promise.resolve()
      .then( () => OSC.Restart() )
      .then( () => this.delay(100) )
      .then( () => this.transfer() )
      .then( () => this.delay(100) )
      .then( () => this.transfer() )
      .then( () => this.delay(100) )
      .then( () => this.transfer("CH2") )
      .then( (measure) => measure.rawAverage )
  }

  transfer(ch)
  {
    var measure; 
    return Promise.resolve()
    .then( () => OSC.Transfer(30+120, 1024) )
    .then( data => { if (ch) {measure = meas.Calculate(ch, 1, 1, data);} return data;} )
    .then( data => canvas.OscilloscopeRedraw(0, data) )
    .then( () => OSC.Restart() )
    .then( () => measure );
  }
 
  delay(interval)
  {
    return new Promise( (resolve, reject) => setTimeout(resolve, interval))
  }
};

var calibration = new Calibration();