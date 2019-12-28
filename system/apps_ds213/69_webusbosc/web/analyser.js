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
//      count += 2;
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

    var code = "";
//    var lastLevel = 0;
    var decodeOffsets = [];
//    data = data.concat(new Array(minPeriod*7).fill(1-data[data.length-1])).concat(data[data.length-1]);
    var ok = this.OnEdgeTrailing(data, minPeriod*3, (count, risingEdge, position) =>
    {
      if (count <= 2)
        return false;
//      var bits = Math.floor((count-minPeriod*0.9+1)/(minPeriod)+1);

      var bits = Math.floor(count/minPeriod + 0.2);
      if (bits > 3) 
        return false;
//        bits = 10;

      decodePosition = position;

//      lastLevel = 1-risingEdge;
      for (var i=0; i<bits; i++, decodePosition += minPeriod)
      {
        decodeOffsets.push(decodePosition);
        code += this.idlePolarity ? 1-risingEdge : risingEdge;
      }
//        if (!decodeBit(this.idlePolarity ? 1-risingEdge : risingEdge))
//          return false;
      return true;
    });

var bites = "";
for (var i=0; i<code.length; i+=3)
{
  var seq = code.substr(i, 3);
  if (seq == "100")
    bites += "0";
  else
  if (seq == "110")
    bites += "1";
  else
  if (seq == "000")
    break;
  else
    bites += "?";
}

var ledWords = []
for (var i=0; i<bites.length; i+= 24)
{
  var ledWord = bites.substr(i, 24);
  if (ledWord.length < 24 || ledWord.indexOf("?") != -1)
    break;

  var ledCode = eval("0b"+ledWord);
  var red = ledCode & 0xff;
  var blue = (ledCode >> 8) & 0xff;
  var green = (ledCode >> 16) & 0xff;

  ledWords.push({r:red, g:green, b:blue, raw:ledCode});
  canvas.annotate(decodeOffsets[i*3 + 3*24/2] + minPeriod/2, INTERFACE.ch1offset, "LED-GBR: " + ("00000"+ledCode.toString(16)).substr(-6));
  var clr = x => ("00" + x.toString(16)).substr(-2);
  canvas.annotate(decodeOffsets[i*3 + 3*24/2] + minPeriod/2 + 120, INTERFACE.ch1offset, "\u2B24", "#" + clr(red) + clr(green) + clr(blue));

  canvas.annotate(decodeOffsets[i*3] + minPeriod/2, INTERFACE.ch1offset, "<");
  canvas.annotate(decodeOffsets[i*3+24*3-1] + minPeriod/2, INTERFACE.ch1offset, ">");

  canvas.OscilloscopeRedrawGraphPart(INTERFACE._wave.scroll, INTERFACE._wave.data, decodeOffsets[i*3+8*3*0], decodeOffsets[i*3+8*3*1], "CH1", "#00ff00");
  canvas.OscilloscopeRedrawGraphPart(INTERFACE._wave.scroll, INTERFACE._wave.data, decodeOffsets[i*3+8*3*1], decodeOffsets[i*3+8*3*2], "CH1", "#0000ff");
  canvas.OscilloscopeRedrawGraphPart(INTERFACE._wave.scroll, INTERFACE._wave.data, decodeOffsets[i*3+8*3*2], decodeOffsets[i*3+8*3*3], "CH1", "#ff0000");
}
if (ledWords.length>0)
  console.log(JSON.stringify(ledWords));

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

  OnEdgeTrailing(data, trailing, h)
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
    return h(trailing, 1-data[data.length-1], last);
  }


}