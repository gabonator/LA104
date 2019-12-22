class Measure
{
  Calculate(ch, dt, dv, rawdata)
  {
    var data = [];
    for (var i =0; i<rawdata.length; i+=5)
    {
      var s = parseInt("0x" + rawdata.substr(i, 5));
      var bundle = [s&255, (s>>8)&255, (s>>16)&1, (s>>17)&1];
      if (ch == "CH1")
        data.push(bundle[0]);
      if (ch == "CH2")
        data.push(bundle[1]);
    }

    var smin, smax, savg = 0;
    smin = smax = data[0];
    for (var i=0; i<data.length; i++)
    {
      savg += data[i];
      smin = Math.min(smin, data[i]);
      smax = Math.max(smax, data[i]);
    }
    savg /= data.length;

    // per samplex
    dt /= 30;
    dv = dv/256*8;

    var range = smax - smin;
    var frequency = 0;

    if (range > 40)
    {
      var threshHigh = (smin+smax)/2 + range/4;
      var threshLow = (smin+smax)/2 - range/4;

      var prev = data[0] > threshLow;
      var transitions = 0;
      var first = 0;
      var last = 0;
      var lastLast = 0;
      for (var i=0; i<data.length; i++)
      {
         var level = prev;
         if (data[i] < threshLow) 
           level = 0;
         if (data[i] > threshHigh) 
           level = 1;
         if (level != prev)
         {
           if (first == 0)
             first = i;
           else
             transitions++;
           lastLast = last;
           last = i;
         }
         prev = level;
      }
//      console.log([data.length, transitions]);
         if (transitions >= 4)
         {
           if (transitions%2 == 1) // odd
           {
             transitions--;
             last = lastLast;
           }
           var halfPeriod = (last-first)/transitions;
           var period = 2*halfPeriod;
           frequency = 1/(period * dt);
         }

    }
    var rawavg = savg, rawmin = smin, rawmax = smax;
    savg = this.voltageCalib(ch, savg);
    smin = this.voltageCalib(ch, smin);
    smax = this.voltageCalib(ch, smax);

    range = smax - smin;

    return {frequency:this.frequency(frequency), 
      average:this.voltage(savg, dv), amplitude:this.voltage(range, dv), min:this.voltage(smin, dv), max:this.voltage(smax, dv), 
      rawAverage:rawavg, rawMin:rawmin, rawMax:rawmax};
  }

  voltageCalib(ch, v)
  {
    if (ch == "CH1")
      v = CALIBRATION.getCh1(v) - INTERFACE.ch1offset;
    if (ch == "CH2")
      v = CALIBRATION.getCh2(v) - INTERFACE.ch2offset;
    return v;
  }

  voltage(v, mul)
  {
    v = v * mul;
    if (v < 1.0)
      return Math.floor(v*1000) + " mV";
    return v.toFixed(2) + " V";
  }

  frequency(f)
  {
    if (f < 1000)
      return f.toFixed(3) + " Hz";
    if (f < 1000000)
      return (f/1000).toFixed(2) + " kHz";
    return (f/1000000).toFixed(2) + " MHz";
  }
}