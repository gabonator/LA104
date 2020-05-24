class CWeather
{
  Frequency()
  {
    return 433876000;
  }
	
  Identify(pulse)
  {
    var IsSpacer = l => l >= 18 && l <= 25;
    var IsShort = l => l >= 4 && l <= 5;
    var IsLong = l => l >= 8 && l <= 10;
    
    if (this.PulseLen(pulse[0]) == 1 && 
        IsSpacer(this.PulseLen(pulse[1])) &&
        PulseLen(pulse[2]) == 1 &&
        (IsShort(this.PulseLen(pulse[3])) ||
        IsLong(this.PulseLen(pulse[3]))))
    {
      return true;
    }
    return false;
  }

  Example()
  {
    var attributes = {};
    // -512 (-51.2 C) ... 999 (+99.9 C)
    attributes["temperature10"] = 171; // 17.1 C
    attributes["humidity"] = 99; // 99 %
    attributes["id"] = 38;
	attributes["battery_low"] = 0;
    attributes["channel"] = 1;
    attributes["junk"] = 2;

    return attributes;
  }

  Demodulate(pulse)
  {
    var b = this.PulseToNibbles(pulse);
    if (!b || b.length != 9)
      return false;

    var Reverse2 = x => ((x & 1) ? 2 : 0) + ((x & 2) ? 1 : 0);

    var sum_nibbles = b[0] + b[1] + b[2] + b[3] + b[4] + b[5] + b[6] + b[7];

    /* IIIICCII B???TTTT TTTTTSSS HHHHHHH1 XXXX */
    var negative_sign = (b[5] & 7);
    var temp          = b[3] | (b[4]<<4) | ((b[5]>>3)<<8);
    var humidity      = ((b[6] | (b[7]<<4)) & 0x7f) - 28;
    var sensor_id     = b[0] | ((b[1] >> 2) << 4);
    var battery_low   = b[2] & 1;
    var channel       = Reverse2(b[1] >> 2);
    var unk           = b[2] >> 1;
    var crcOk         = b[8] == (sum_nibbles & 0xF);

    var tempC10 = negative_sign ? -( (1<<9) - temp ) : temp;

    attributes = {};
    attributes["temperature10"] = tempC10;
    attributes["humidity"] = humidity;
    attributes["id"] = sensor_id;
    attributes["battery_low"] = battery_low;
    attributes["channel"] = channel;
    attributes["junk"] = unk;
    attributes["valid"] = crcOk;

    return attributes;
  }

  Modulate(attr)
  {
    var Reverse2 = x => ((x & 1) ? 2 : 0) + ((x & 2) ? 1 : 0);

    var temp = attr["temperature10"];
    var hum = (attr["humidity"] + 28) | 128;

    var nibbles = [];

    nibbles.push(attr["id"] & 0xf);
    nibbles.push(Reverse2(attr["channel"]) | (attr["id"] >> 4) << 2);
    nibbles.push(attr["battery_low"] | (attr["junk"] << 1));
    nibbles.push(temp & 15);
    nibbles.push((temp >> 4) & 15);
    nibbles.push((temp >> 8) & 15);
    nibbles.push(hum & 15);
    nibbles.push((hum >> 4) & 15);
    
    nibbles.push(nibbles.reduce((a,b)=>a+b, 0) & 15);

    return this.NibblesToPulse(nibbles);
  }

  PulseLen(microseconds)
  {
    return (microseconds+250)/500;
  }

  PulseDuration(ticks)
  {
    return ticks*500;
  }

  PulseToNibbles(pulse)
  {
    var nibbles = [];

    if (pulse.length < 9*4*2+3)
      return false;

    var nibble = 0;
    for (var i=0; i<9*4; i++)
    {
      var spacer = this.PulseLen(pulse[2+i*2]);
      var data = this.PulseLen(pulse[2+i*2+1]);
      if (spacer != 1)
        return false; //throw "wrong spacer value";
      if (data != 4 && data != 8)
        return false; //throw "wrong data value";

      var bit = data == 4 ? 0 : 1;
      var bitpos = i & 3;
     
      nibble |= bit << bitpos;
      if (bitpos == 3)
      {
        nibbles.push(nibble);
        nibble = 0;
      }
    }
    return nibbles;
  }

  NibblesToPulse(nibbles)
  {
    if (nibbles.length != 9)
      return false;

    var pulse = [];
    pulse.push(this.PulseDuration(1));
    pulse.push(this.PulseDuration(18));
    pulse.push(this.PulseDuration(1));

    for (var i=0; i<nibbles.length; i++)
    {
      for (var j=0; j<4; j++)
      {
        pulse.push(this.PulseDuration((nibbles[i] & (1<<j)) ? 8 : 4));
        pulse.push(this.PulseDuration(1));
      }
    }

    return pulse;
  }
}

