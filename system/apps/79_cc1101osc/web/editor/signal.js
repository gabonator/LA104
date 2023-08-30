class Signal
{
  constructor(signal, type)
  {
    if (typeof(signal) == "string" && typeof(type) == "string")
    {
      this.message = signal;
      this.type = type;
      return;
    }
    if (!signal)
    {
      this.data = [];
      this.level = []
      this.type = "none";
      this.history = []
    }
    else if (Array.isArray(signal))
    {
      this.data = signal;
      this.type = type || "signal";
      if (this.type == "signal")
        this.level = this.data.map((s, i) => (i % 2 == 0) ? "h" : "l");
      this.history = ["constructor"]
    } else
    {
      if (signal.type == "multi" || signal.type == "multisymbols")
      {
        this.multi = [];
        for (var j=0; j<signal.multi.length; j++)
          this.multi.push({data:[...signal.multi[j].data], level:[...signal.multi[j].level]})
        this.type = signal.type;
        this.history = [...signal.history];
      } else
      {
        this.data = [...signal.data];
        if (signal.level)
          this.level = [...signal.level];
        this.type = signal.type;
        this.history = [...signal.history];
      }
    }
  }

  getInfo()
  {
    if (this.type == "none")
      return 'Nothing';
    if (this.type == "signal")
    {
      var duration = this.data.reduce((a,b)=>a+b, 0);
      return `Signal ${this.data.length}p / ${(duration/1000).toFixed(1)}ms`;
    }
    if (this.type == "pairs")
      return `Array of ${this.data.length} pairs`;
    if (this.type == "symbols")
      return `Array of ${this.data.length} symbols`;
    if (this.type.substr(0, 4) == "uint")
      return `${this.type}[${this.data.length}]`
    if (this.type == "multi")
      return `${this.multi.length} x Signal`;
    if (this.type == "multisymbols")
      return `${this.multi.length} x Symbols`;
    if (this.type == "bitstream")
      return `Array of ${this.data.length} bits`;
    if (this.type == "multibitstream")
      return `${this.multi.length} x Bitstream`;
    if (this.type.substr(0, 9) == "multiuint")
      return `${this.multi.length} x ` + this.type.substr(5) + "[]";
    if (this.type == "attributes")
      return `Array of ${this.data.length} attributes`;
    throw "wrong type"
    return this.type;
  }

  // exports
  makeQuantized(q)
  {
    var copy = this.makeDenoised(Math.floor(q/2));
    if (this.type == "multi")
    {
      for (var j=0; j<copy.multi.length; j++)
        for (var i=0; i<copy.multi[j].data.length; i++)
          copy.multi[j].data[i] = Math.floor((copy.multi[j].data[i]+q/2)/q)*q;
      // TODO: automatically merge duplicates?
      copy.flatten();
    } 
    else if (this.type == "signal")
    {
      for (var i=0; i<copy.data.length; i++)
        copy.data[i] = Math.floor((copy.data[i]+q/2)/q)*q;
    }
    else 
      throw "no signal to quantize";
    copy.history.push(["quantize", q])
    console.log(copy)
    return copy;
  }
  flatten()
  {
    if (this.type.substr(0, 5) != "multi")
      return;
    for (var i=1; i<this.multi.length; i++)
    {
      var duplicate = false;
      for (var j=0; j<i; j++)
        if (JSON.stringify(this.multi[i]) == JSON.stringify(this.multi[j]))
        {
          duplicate = true;
          break;
        }
      this.multi.splice(i--, 1);
    }
    if (this.multi.length == 1)
    {
      this.type = this.type.substr(5);
      if (this.type == "")
        this.type = "signal"
      for (var i in this.multi[0])
        this[i] = this.multi[0][i];
      delete this.multi;
    }
  }
  makeSlice(begin, len, tillEnd)
  {
    var copy = new Signal(this);
    var slice = (copy) =>
    {
      if (tillEnd)
      {
        copy.data = copy.data.slice(begin)
        copy.level = copy.level.slice(begin)
      } else {
        copy.data = copy.data.slice(begin, begin+len)
        copy.level = copy.level.slice(begin, begin+len)
      }
    }
    if (this.type == "multi")
    {
      for (var i=0; i<copy.multi.length; i++)
        slice(copy.multi[i])
    }
    else
      slice(copy);

    if (tillEnd)
      copy.history.push(["slice", begin])
    else
      copy.history.push(["slice", begin, len])
    return copy
  }
  makeRange(first, last)
  {
    var copy = new Signal(this);
    var slice = (copy) =>
    {
      copy.data = [];
      if (copy.level)
        copy.level = [];
      first = Math.max(0, Math.min(first, this.data.length-1));
      last = Math.max(0, Math.min(last, this.data.length-1));
      if (last >= first)
      {
        for (var i=first; i<=last; i++)
        {
          copy.data.push(this.data[i]);
          if (this.level)
            copy.level.push(this.level[i]);
        }
      } else {
        for (var i=first; i>=last; i--)
        {
          copy.data.push(this.data[i]);
          if (this.level)
            copy.level.push(this.level[i]);
        }
      }
    }
    if (this.type == "multi")
    {
      for (var i=0; i<copy.multi.length; i++)
        slice(copy.multi[i])
    }
    else
      slice(copy);

    copy.history.push(["range", first, last])
    return copy
  }
  makeFilterByCondition(cond, brk)
  {
    var copy = new Signal(this);
    var filterCode = eval(`(x, i) => ${cond}`);
    copy.data = [];
    copy.level = [];
    for (var i=0; i<this.data.length; i++)
    {
      if (filterCode(this.data[i], i))
      {
        copy.data.push(this.data[i]);
        copy.level.push(this.level[i]);
      }
      else if (brk)
        break;
    }
    copy.history.push(["filterbycond", cond, brk])
    return copy;
  }
  makeSplitByCondition(cond)
  {
    var filterCode = eval(`(x, i) => ${cond}`);
    var partial = {data:[], level:[]};
    var multi = [];
    for (var i=0; i<this.data.length; i++)
    {
      if (!filterCode(this.data[i], i))
      {
        partial.data.push(this.data[i]);
        partial.level.push(this.level[i]);
      } else {
        multi.push(partial);
        partial = {data:[], level:[]}
      }
    }
    if (partial.data.length > 0)
      multi.push(partial);

    var newSignal = new Signal();
    newSignal.type = "multi";
    newSignal.multi = multi;
//    newSignal.valid = new Array(multi.length).fill(0);
    return newSignal;
  }
  makePairs()
  {
    var copy = new Signal(this);
    var arrout = []
    for (var i=0; i<this.data.length; i+=2)
      arrout.push(`${this.data[i]} ${this.data[i+1]}`);
    copy.data = arrout;
    copy.level = [];
    copy.type = "pairs";
    copy.history.push("pairs");
    return copy;
  }
  makeLookupTable(assign, invalid)
  {
    var lookup = (src, out) =>
    {
      for (var i=0; i<src.data.length; i++)
      {
        var symbol = invalid;
        for (var j=0; j<assign.length; j++)
        {
          if (assign[j][0].includes(","))
          {
            var syms = assign[j][0].split(",").map(x=>x.trim())
            var m = true
            for (var k=0; k<syms.length; k++)
              if (src.data[i+k] != syms[k])
              {
                m = false;
                break;
              }
            if (m)
            {
              symbol = assign[j][1];
              i += syms.length-1;
              break;
            }
          }
          else if (assign[j][0].substr(-1) == "L" && src.level[i] == "l" && parseInt(assign[j][0]) == src.data[i])
            symbol = assign[j][1];
          else if (assign[j][0].substr(-1) == "H" && src.level[i] == "h" && parseInt(assign[j][0]) == src.data[i])
            symbol = assign[j][1];
          else if (src.data[i] == assign[j][0])
            symbol = assign[j][1];
        }
        if (symbol != "")
          symbol.split(",").forEach(s => out.data.push(s.trim()))
      }
    }
    var tosignal = (signal) =>
    {
      var n = {data:[], level:[]};
      for (var i=0; i<signal.data.length; i++)
      {
        var data = parseInt(signal.data[i]);
        var level = signal.data[i].substr(-1);
        if (n.level.length > 0 && n.level[n.level.length-1] == level)
        {
          n.data[n.data.length-1] += data;
        } else {
          n.data.push(data);
          n.level.push(level);
        }
      }
      signal.data = n.data;
      signal.level = n.level.map(l => l.toLowerCase());
    }

    var copy = new Signal(this);

    if (this.type == "multi")
    {
      copy.multi = [];
      var allbitstream = this.multi.length > 0;
      for (var i=0; i<this.multi.length; i++)
      {
        var out = {data:[], level:[]};
        lookup(this.multi[i], out);
        var bitstream = out.data.length > 0 && out.data.findIndex(x => x !== "0" && x !== 0 && x !== "1" && x !== 1) == -1;
        allbitstream &= bitstream;
        copy.multi.push(out);
      }
      copy.type = allbitstream ? "multibitstream" : "multisymbols";
    } else {
      copy.data = [];
      copy.level = [];
      copy.history.push(["lookuptable", assign, invalid])
      lookup(this, copy)
      var bitstream = copy.data.length > 0 && copy.data.findIndex(x => x !== "0" && x !== 0 && x !== "1" && x !== 1) == -1;
      var signal = copy.data.length > 0 && 
        copy.data.findIndex(x => !["H", "L"].includes(x.substr(-1))) == -1 &&
        copy.data.findIndex(x => isNaN(parseInt(x))) == -1;
      if (signal)
        tosignal(copy);
      copy.type = bitstream ? "bitstream" : (signal ? "signal" : "symbols");
    }
    return copy;
  }
  checkAssertion(cond, index)
  {
    var check = (arr) =>
    {
      var result = false;
      try {
        result = eval(`${cond}`);
      } catch (e) {
        result = false;
      }
      return result;
    }
    if (this.type == "attributes")
    {
      var arr = {};
      for (var d of this.data)
        arr[d[0]] = d[1];
      return check(arr);
    }
    else if (this.type.substr(0, 5) == "multi")
    {
      if (typeof(index) == "number")
        return check(this.multi[index].data);

      var ok = 0;
      for (var i=0; i<this.multi.length; i++)
        if (check(this.multi[i].data))
          ok++;
      return ok;
    } else
      return check(this.data);
  }
  getCountElements(cond, breakOn)
  {
    var filterCode = eval(`(x, i) => ${cond}`);
    var count = 0;
    for (var i=0; i<this.data.length; i++)
    {
      if (filterCode(this.data[i], i))
        count++;
      else if (breakOn)
        break;
    }
    return count;
  }
  makeDenoised(q)
  {
    var copy = new Signal(this);
    var denoise = (copy) =>
    {
      for (var i=1; i<copy.data.length-1; i++)
        if (copy.data[i] < q)
        {
          copy.data[i-1] += copy.data[i] + copy.data[i+1]
          copy.data.splice(i, 2)
          copy.level.splice(i, 2)
        }
    }

    if (this.type == "multi")
    {
      for (var j=0; j<copy.multi.length; j++)
        denoise(copy.multi[j]);
    } else
    if (this.type == "signal")
    {
      denoise(copy);
    } else
      throw "no signal to denoise";
    copy.history.push(["denoise", q])
    return copy;
  }
  makeSelect(ind)
  {
    var copy = new Signal(this);
    if (this.type.substr(0, 5) == "multi")
    {
      copy.type = this.type.substr(5);
      if (copy.type == "")
        copy.type = "signal";
      copy.data = [];
      copy.level = [];

      if (ind >= 0 && ind < this.multi.length)
      {
        var src = this.multi[ind];
        for (var i in src)
          copy[i] = Array.isArray(src[i]) ? [...src[i]] : src[i];
      } else
        copy.type = "none"
    } 
    delete copy.multi;
    return copy;
  }

  getBitstreamToHex(msb, databits)
  {
    if (this.type != "bitstream")
      throw "wrong type"

    var format = (bits, val) => {
      var digits = (bits+3)>>2;
      return "0x"+(("00000000"+val.toString(16)).substr(-digits));
    };
    var uint = new Uint32Array(4);
    var arr = this.data;
    var bits = 0;
    var result = [];
    for (var i=0; i<arr.length; i++)
    {
      var bit = arr[i] === 1 || arr[i] === "1";
      if (arr[i] !== 0 && arr[i] !== "0" && arr[i] !== 1 && arr[i] !== "1")
      {
        return false;
      }
      
      if (msb)
      {
        uint[0] <<= 1;
        uint[0] |= bit;
      } else {
        uint[0] >>>= 1;
        uint[0] |= bit<<(databits-1);
      }
      if (++bits % databits == 0)
      {
        result.push(format(databits, uint[0]));
        uint[0] = 0;
      }
    }
    if (bits % databits != 0)
    {
      if (!msb)
        uint[0] >>>= databits - bits;
      result.push(format(bits % databits, uint[0]));
    }
    return {bits:bits, values:result};
  }
}
