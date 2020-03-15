class Analyser
{
  constructor()
  {
    this.bitstreamAnal = new BitstreamAnalyser;
    this.uartDecoder = new UartDecoder;
    this.ledDecoder = new LedDecoder;
  }

  analyse(rawdata, ch, mode)
  {
    if (mode == "UART")
    {
      var bitstream = this.bitstreamAnal.analyse(rawdata, ch, 10, 1000, 10);
      if (bitstream)
      {
        var result = this.uartDecoder.decode(bitstream, ch);
        if (result)
          console.log("UART: " + result.map(x => String.fromCharCode(x)).join(""));
      }
    }
    if (mode == "LED-RGB")
    {
      var bitstream = this.bitstreamAnal.analyse(rawdata, ch, 3, 3, 3);
      if (bitstream)
      { 
        var result = this.ledDecoder.decode(bitstream, ch);
        if (result)
          console.log("LED-RGB: " + JSON.stringify(result));
      }
    }
  }
}

class BitstreamAnalyser
{
  analyse(rawdata, ch, maxBitsLow, maxBitsHigh, trailing)
  {
    var data = this.getData(rawdata, "CH1");
    if (!this.threshold(data))
      return;

    this.idlePolarity = 1;

    // find smallest
    var minPeriod = -1;
    this.OnEdgeTrailing(data, 0, (count) =>
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
    var ok = this.OnEdgeTrailing(data, 0, (count, risingEdge) =>
    {
      if (count <= 2)
        return false;

      var bits = 0;
      if (count >= minPeriod*9 && risingEdge)  // TODO: uart?
      {
        return false;
      }
      else if (count >= minPeriod*9 && !risingEdge)  // TODO: uart?
      {
        // ignore idle line
        return true;
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

    var decodeCode = [];
    var decodeOffsets = [];
    var ok = this.OnEdgeTrailing(data, minPeriod*trailing, (count, risingEdge, position) =>
    {
      if (count <= 2)
        return false;

      var bits = Math.floor(count/minPeriod + 0.2);
      if (risingEdge && bits > maxBitsLow) 
        return false;
      if (!risingEdge && bits > maxBitsHigh) 
        return false;

      if (!risingEdge && bits > 10) 
        bits = 10; // uart idle line

      var decodePosition = position;

      for (var i=0; i<bits; i++, decodePosition += minPeriod)
      {
        decodeOffsets.push(decodePosition);
        decodeCode.push(this.idlePolarity ? 1-risingEdge : risingEdge);
      }
      return true;
    });

    if (!ok)
      return;
    return {code:decodeCode, offsets:decodeOffsets, rate:totalBits/totalTime, channel:ch, minPeriod:minPeriod};
  }                                                                                                         
 
  getData(rawdata, ch)
  {
    var data = new Array(rawdata.length/3);
    for (var i=0, j=0; i<rawdata.length; i+=3, j++)
    {
      var bundle = [rawdata[i+0], rawdata[i+1], rawdata[i+2] & 1, rawdata[i+2] & 2];
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
    if (range < 30) // weak signal
      return;

    var trigMin = dmin + range/8;
    var trigMax = dmax - range/8;

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
        if (middle++ > 10) // slow transition
          return;
      }
      data[i] = newState;
      oldState = newState;
    }
    return data;
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
    if (trailing > 0)
      return h(trailing, 1-data[data.length-1], last); // TODO:
    else
      return true;
  }
}


class LedDecoder
{
  decode(bitstream)
  {
    var code = bitstream.code.join("");
    var offsets = bitstream.offsets;
    var minPeriod = bitstream.minPeriod;

    // led decoder
    var bits = "";
    for (var i=0; i<code.length; i+=3)
    {
      var seq = code.substr(i, 3);
      if (seq == "100")
        bits += "0";
      else
      if (seq == "110")
      {
        bits += "1";
      }
      else
      if (seq == "000")
        break;
      else
        bits += "?";
    }

    var ledWords = []
    for (var i=0; i<bits.length; i+= 24)
    {
      var ledWord = bits.substr(i, 24);
      if (ledWord.length < 24 || ledWord.indexOf("?") != -1)
        break;          	

      var ledCode = eval("0b"+ledWord);
      var blue = ledCode & 0xff;
      var red = (ledCode >> 8) & 0xff;
      var green = (ledCode >> 16) & 0xff;

      ledWords.push({r:red, g:green, b:blue});
      canvas.annotate(offsets[i*3 + 3*24/2] + minPeriod/2, INTERFACE.ch1offset, "LED-GRB: " + ("00000"+ledCode.toString(16)).substr(-6));
      var clr = x => ("00" + x.toString(16)).substr(-2);
      canvas.annotate(offsets[i*3 + 3*24/2] + minPeriod/2 + 120, INTERFACE.ch1offset, "\u2B24", "#" + clr(red) + clr(green) + clr(blue));

      canvas.annotate(offsets[i*3] + minPeriod/2, INTERFACE.ch1offset, "<");
      canvas.annotate(offsets[i*3+24*3-1] + minPeriod/2, INTERFACE.ch1offset, ">");

      canvas.OscilloscopeRedrawGraphPart(INTERFACE._wave.scroll, INTERFACE._wave.data, offsets[i*3+8*3*0], offsets[i*3+8*3*1], "CH1", "#00ff00");
      canvas.OscilloscopeRedrawGraphPart(INTERFACE._wave.scroll, INTERFACE._wave.data, offsets[i*3+8*3*1], offsets[i*3+8*3*2], "CH1", "#ff0000");
      canvas.OscilloscopeRedrawGraphPart(INTERFACE._wave.scroll, INTERFACE._wave.data, offsets[i*3+8*3*2], offsets[i*3+8*3*3], "CH1", "#0000ff");
    }

    if (ledWords.length>0)
      return ledWords;
    return;
  }
}

class UartDecoder
{
  decode(bitstream)
  {
    var decodeByte = 0, decodeIndex = -1, decoded = [], decodePosition = 0;

    var annotate = (text, dx, dy) =>
    {
      if (!dx) dx = 0;
      if (!dy) dy = 0;
      canvas.annotate(decodePosition + bitstream.minPeriod/2 + dx, INTERFACE.ch1offset + dy, text);
    }

    for (var i=0; i<bitstream.code.length; i++)
    {
      var bit = bitstream.code[i];
      decodePosition = bitstream.offsets[i];

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
      else if ( decodeIndex == -1 )
      {
        if ( bit == 0 )
        {
          decodeIndex = 0;
          annotate("<");
        }
      } 
      else
      {
        annotate(bit.toString());
        decodeByte >>= 1;
        if ( bit )
          decodeByte |= 128;

        if (++decodeIndex >= 8)
        {
          annotate("0x"+decodeByte.toString(16) + " = " + decodeByte + " = '"+String.fromCharCode(decodeByte) + "'" , -bitstream.minPeriod*4, 16);
          decoded.push(decodeByte);
          decodeIndex = -2; // stop bit
          decodeByte = 0;
        }
      }
    }
    return decoded;
  }
}
