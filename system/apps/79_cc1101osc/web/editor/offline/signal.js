class Signal
{
  constructor(signal)
  {
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
      this.level = this.data.map((s, i) => (i % 2 == 0) ? "h" : "l")
      this.type = "signal";
      this.history = ["constructor"]
    } else
    {
      this.data = [...signal.data];
      this.level = [...signal.level];
      this.type = signal.type;
      this.history = [...signal.history];
    }
  }

  getInfo()
  {
    if (this.type == "signal")
    {
      var duration = this.data.reduce((a,b)=>a+b, 0);
      return `Signal ${this.data.length}p / ${(duration/1000).toFixed(1)}ms`;
    }
    if (this.type == "pairs")
      return `Array of ${this.data.length} pairs`;
    if (this.type == "symbols")
      return `Array of ${this.data.length} symbols`;

    throw "wrong type"
    return this.type;
  }

  // exports
  makeQuantized(q)
  {
    var copy = new Signal(this);
    for (var i=0; i<copy.data.length; i++)
      copy.data[i] = Math.floor((copy.data[i]+q/2)/q)*q;
    copy.history.push(["quantize", q])
    return copy;
  }
  makeSlice(begin, len, tillEnd)
  {
    var copy = new Signal(this);
    if (tillEnd)
    {
      copy.data = copy.data.slice(begin)
      copy.level = copy.level.slice(begin)
    } else {
      copy.data = copy.data.slice(begin, begin+len)
      copy.level = copy.level.slice(begin, begin+len)
    }
    if (tillEnd)
      copy.history.push(["slice", begin])
    else
      copy.history.push(["slice", begin, len])
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
    var copy = new Signal(this);
    var filterCode = eval(`(x, i) => ${cond}`);
    copy.data = [];
    copy.level = [];
    for (var i=0; i<this.data.length; i++)
    {
      if (!filterCode(this.data[i], i))
      {
        copy.data.push(this.data[i]);
        copy.level.push(this.level[i]);
      }
      else
        break;
    }
    copy.history.push(["split", cond])
    return copy;
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
    var copy = new Signal(this);
    copy.data = [];
    copy.level = [];
    copy.type = "symbols";
    copy.history.push(["lookuptable", assign, invalid])

    for (var i=0; i<this.data.length; i++)
    {
      var symbol = invalid;
      for (var j=0; j<assign.length; j++)
      {

        if (assign[j][0].substr(-1) == "L" && this.level[j] == "L" && parseInt(assign[j][0].substr(0, -1)) == this.data[i])
          symbol = assign[j][1];
        else if (assign[j][0].substr(-1) == "H" && this.level[j] == "H" && parseInt(assign[j][0].substr(0, -1)) == this.data[i])
          symbol = assign[j][1];
        else if (this.data[i] == assign[j][0])
          symbol = assign[j][1];
      }
      copy.data.push(symbol);
    }
    return copy;
  }
  checkAssertion(cond)
  {
    var result = false;
    try {
      var arr = this.data;
      result = eval(`${cond}`);
    } catch (e) {
      result = false;
    }
    return result;
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
}
