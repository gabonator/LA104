class Analyser
{
  analyse(rawdata)
  {
    var data = this.getData(rawdata, "CH1");
    if (!this.threshold(data))
      return;

    this.idlePolarity = 1;

    // find smallest
    var minPeriod = -1;
    this.OnEdge(data, (count) =>
    {
      if (count <= 2)
         return true;
      if (minPeriod == -1)
        minPeriod = count;
      minPeriod = Math.min(minPeriod, count);
      return true;
    });

    // sum
    var totalBits = 0, totalTime = 0;
    var ok = this.OnEdge(data, (count, risingEdge) =>
    {
      if (count <= 2)
        return false;

      var bits = 0;
//      var xx = (count)/minPeriod;
//      console.log([count, Math.floor(xx), (xx - Math.floor(xx)).toFixed(2) ]);

      if (count >= minPeriod*9 && risingEdge) 
      {
        return false;
      }
      else if (count >= minPeriod*8) bits = 8;
      else if (count >= minPeriod*7) bits = 7;
      else if (count >= minPeriod*6) bits = 6;
      else if (count >= minPeriod*5) bits = 5;
      else if (count >= minPeriod*4) bits = 4;
      else if (count >= minPeriod*3) bits = 3;
      else if (count >= minPeriod*2) bits = 2;
      else if (count >= minPeriod*1) bits = 1;
      else {
        return false;
      }

      totalBits += bits;
      totalTime += count;
      return true;
    });

    if (!ok)
      return;

    var decodeByte = 0, decodeIndex = -1, decoded = [], decodePosition = 0;
    var annotate = (text, dx, dy) =>
    {
      if (!dx) dx = 0;
      if (!dy) dy = 0;
      canvas.annotate(decodePosition + minPeriod/2 + dx, INTERFACE.ch1offset + dy, text);
    }

    var decodeBit = (bit) =>
    {
      if ( decodeIndex == -2 )
      {
        if (bit == 1) // stop bit
        {
          decodeIndex = -1;
          annotate(">");
        }
        else
          return false;
      }

      if ( decodeIndex == -1 )
      {
        if ( bit == 0 )
        {
          decodeIndex = 0;
          annotate("<");
        }
        return true;
      }

      annotate(bit.toString());
      decodeByte >>= 1;
      if ( bit )
        decodeByte |= 128;

      if (++decodeIndex >= 8)
      {
        annotate("0x"+decodeByte.toString(16) + " = " + decodeByte + " = '"+String.fromCharCode(decodeByte) + "'" , -minPeriod*4, 16);
        decoded.push(decodeByte);
        decodeIndex = -2; // stop bit
        decodeByte = 0;
      }
      return true;
    };

    var ok = this.OnEdge(data, (count, risingEdge, position) =>
    {
      if (count <= 2)
        return false;
//      var bits = Math.floor((count-minPeriod*0.9+1)/(minPeriod)+1);

      var bits = Math.floor(count/minPeriod + 0.2);
      if (bits > 10) 
        bits = 10;

      decodePosition = position;

      for (var i=0; i<bits; i++, decodePosition += minPeriod)
        if (!decodeBit(this.idlePolarity ? 1-risingEdge : risingEdge))
          return false;
      return true;
    });
    
    decodeBit(this.idlePolarity);

    if (!ok)
      return;

    if (decoded.length > 0)
      console.log(decoded.map(x=>String.fromCharCode(x)).join(""));
  }
 
  getData(rawdata, ch)
  {
    var data = new Array(rawdata.length/5);
    for (var i=0, j=0; i<rawdata.length; i+=5, j++)
    {
      var s = parseInt("0x" + rawdata.substr(i, 5));
      var bundle = [s&255, (s>>8)&255, (s>>16)&1, (s>>17)&1];
      if (ch == "CH1")
        data[j] = bundle[0];
      if (ch == "CH2")
        data[j] = bundle[1];
    }
    return data;
  }

  threshold(data)
  {            
    var dmin = data[0], dmax = data[0];
    for (var i=0; i<data.length; i++)
    {
      dmin = Math.min(dmin, data[i]);
      dmax = Math.max(dmax, data[i]);
    }
    
    var range = dmax - dmin;
    if (range < 30)
      return;

    var trigMin = dmin + range/4;
    var trigMax = dmax - range/4;

    var oldState = data[0] > (dmin+dmax)/2;
    var middle = 0;
    for (var i=0; i<data.length; i++)
    {
      var newState = oldState;
      if (data[i] > trigMax)
      {
        newState = 1;
        middle = 0;
      }
      else if (data[i] < trigMin)
      {
        newState = 0;
        middle = 0;
      }
      else
      {
        if (middle++ > 10)
          return;
      }
      data[i] = newState;
      oldState = newState;
    }
    return data;
  }

  OnEdge(data, h)
  {
    var old = data[0];
    var last = -1;
    for (var i = 0; i < data.length; i++)
    {
      var cur = data[i];
      if (cur != old)
      {
        if (last != -1)
          if (!h(i-last, cur, last))
            return false;
        last = i;
        old = cur;
      }
    }
    return true;
  }


}