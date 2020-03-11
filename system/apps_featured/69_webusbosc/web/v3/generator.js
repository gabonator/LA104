  function setSquareWave(freq, duty)
  {
    promises.push(() => 
      Promise.resolve()
      .then( ()=>GEN.SetMode(GEN.Enums["square"], 0, 0) )
      .then( ()=>GEN.SetFrequency(freq) )
      .then( ()=>{ if (duty) return GEN.SetDuty(duty); } )
      .then( ()=>GEN.GetFrequency() )
      .then( (f)=>console.log("Frequency="+f) )
    );
  }

  function setDcWave(v)
  {                       
    // 1455 - 1.0V
    // 2935 - 2.0V
    var X = CALIBRATION.gen1V+(v-1.0)*(CALIBRATION.gen2V-CALIBRATION.gen1V);
    if (X < 0 || X > 0xfff)
    {
      alert("Value out of range!");
      return;
    }
    var sample = X; //Math.floor(0xfff*(v-Umin)/(Umax-Umin));
    return transferWave([sample], 1000);
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

  function setEquationWave(eq, freq)
  {
    var fixMath = (func) => 
    {
      var i = 0, j = 0;
      while ((j = eq.indexOf(func, i)) != -1)
      {
        if (eq.substr(j-5, 5) != "Math.")
        {
          eq = eq.substr(0, j) + "Math." + eq.substr(j);
          i = j + 6;
        }
        else
          i = j + 1;
      }
    }

    ["abs", "sin", "cos", "floor"].forEach(f => fixMath(f));
    console.log(eq);

    var n = 38*4;       
    var samples = [];
    for (var i=0; i<n; i++)
    {
      var x = Math.PI*2*i/n;
      var sample = 0;
      try { sample = eval(eq); } catch (e) {console.log("Invalid formula: "+e); return;}
      
      sample = (sample * (0x1000/2-20)) + 0x1000/2;
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
//      .then( ()=>GEN.SetWave(bufferPtr, samples.length-1) ) // WTF??? bios error, wont configure DMA without changing buffer length
      .then( ()=>GEN.SetMode(GEN.Enums["buffer"], bufferPtr, samples.length) )
      .then( ()=>GEN.SetFrequency(freq*samples.length) )
      .then( ()=>GEN.GetFrequency() )
      .then( (f)=>console.log("Frequency="+Math.floor(f/samples.length)) )
    );
  }
