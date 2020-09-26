var canvas = new PreviewCanvas(900, 100);
var detail = new DetailCanvas(900, 100);
var gui = new RemoteGui();
var aaa;

class Memory
{
  constructor()
  {
    this.buffers = [[], []];
    this.sizes = [0, 0];
    this.current = 0;
    this.maxSize = 2000*1000; // 2 seconds max
    this.counter = 0;
  }

  push(pulse)
  {
    this.counter += pulse.length;
    this.buffers[0] = this.buffers[0].concat(pulse);
    this.sizes[0] += pulse.reduce((a,b) => a+(b & 0xffff), 0);
    if (this.sizes[0] > this.maxSize)
    {
//      this.counter -= this.buffers[1].length;
      this.buffers[1] = this.buffers[0];
      this.sizes[1] = this.sizes[0];
      this.buffers[0] = [];
      this.sizes[0] = 0;
    }
  }

  getRange(first, last)
  {
    if (first > last)
      throw "wrong order";

    var pos = 0;
    var buf = this.buffers[1].concat(this.buffers[0]);
    var aux = [];
    var recording = false;
    var counter = this.counter;
    for (var i=buf.length-1; i>=0; i--)
    {
      var cur = buf[i] & 0xffffff;
      if (!recording && cur + pos >= first)
      {
        // toggle!
        recording = true;
      }

      if (recording)
        aux.push(buf[i]);

      if (cur + pos >= last)
      {
        // polarity!
        if ((counter & 1) == 0)
          aux.push(0);
        break;
      }
      counter--; 
      pos += cur;
    }
    aux.reverse();
    for (var i=2; i<aux.length; i++)
    {
      if ((aux[i] & 0xffffff)==0)
      {
        aux[i-1] += aux[i+1] & 0xffffff;
        aux.splice(i, 2);
        i--;
      }
    }
    return aux;
  }
};

var memory = new Memory();

function dumpRange(first, last)
{ 
  var buf = memory.getRange(-last, -first);
  if ((buf[0] & 0xffffff) == 0 && (buf[1] & 0xffffff) > 2000)
    buf[1] = 2000;

  var origbuf = [...buf];
  var ofsstart = 0;
  var ofsend = origbuf.length;

//  console.log(buf);
  if (buf[0] == 0 && buf.length > 1)
  {
    buf.splice(0, 2);
    ofsstart += 2;
  }
  if ((buf.length % 2) == 0)
  {
    buf.pop();
    ofsend--;
  }
  detail.show(origbuf, ofsstart, ofsend);
  buf = buf.map(x => x & 0xffffff);
  aaa = buf.map(x=>x/20);
  console.log("analyse: " + JSON.stringify(buf));
  analyse(buf);
}

var scanEnabled = true;
let startButton = document.querySelector("#start");
startButton.addEventListener('click', function() {
    APP.Info()
      .then( console.log )
      .catch( () => 0 )
      .then( () => APP.Info() )
      .then( console.log )
      .then( onMain )
      .catch( () => console.log("Device not ready") )
});

function timestamp()
{
  return (new Date()).getTime();
}

document.querySelector("#sstart").addEventListener('click', onStart);
document.querySelector("#sstop").addEventListener('click', onStop);
document.querySelector("#zoomin").addEventListener('click', () => canvas.zoomIn());
document.querySelector("#zoomout").addEventListener('click', () => canvas.zoomOut());
document.querySelector("#dzoomin").addEventListener('click', () => detail.zoomIn());
document.querySelector("#dzoomout").addEventListener('click', () => detail.zoomOut());
document.querySelector("#dquantize").addEventListener('click', () => { analyse(detail.quantize()); });
document.querySelector("#dreconstruct").addEventListener('click', () => { 
  reconstruct({protocol:"keyfob", data:{payload:[0x02, 0x70, 0x3f, 0x20, 0xd4, 0x35, 0xb3, 0xe7, 0x01], command:"open"}});
//  reconstruct(analyse(detail.quantize())); 
});
document.querySelector("#dendminus").addEventListener('click', () => { detail.trim(-1); });
document.querySelector("#dendplus").addEventListener('click', () => { detail.trim(+1); });
document.querySelector("#dsend").addEventListener('click', () => { sendPulse(detail.trim(0)); });
//document.querySelector("#dexample").addEventListener('click', () => { example(); });
//document.querySelector("#aenable").addEventListener('click', () => { attackEnable(); });
//document.querySelector("#adisable").addEventListener('click', () => { attackDisable(); });
//document.querySelector("#calibrate").addEventListener('click', () => { calibrate(); });


var started = false;
let toggleButton = document.querySelector("#toggle");
toggleButton.addEventListener('click', function() {
  scanEnabled = 1-scanEnabled;
});

function document_write(msg)
{
}

function onStart()
{
  if (simulator) {started = true; return; }
  if (COMM.onReceive)
  {
    console.log("Try later");
    return;
  }
  MODEM.Start().then( () => syncInfo()).then( () => started = true)
  .then(() => INTERFACE.Update()).then(() => INTERFACE.Finish());
}

function onStop()
{
  if (simulator) {started = false; return; }
  if (COMM.onReceive)
  {
    console.log("Try later");
    return;
  }
  MODEM.Stop().then( () => started = false)
  .then(() => INTERFACE.Update()).then(() => INTERFACE.Finish());
}

function onMain()
{
    var level = 1;
    console.log("-- main --");
    memory.push([0]);
    MODEM.Init()
        .then( (x) => { if (!x) throw "error"; })
//        .then( () => MODEM.Start() )
       .then( () => MODEM.SetFrequency(433876000)) //conrad
//      .then( () => MODEM.SetFrequency(434424000)) // keyfob
//      .then( () => MODEM.SetFrequency(433918000)) // garage
//      .then( () => MODEM.SetFrequency(433856000)) // conrad, OS/rain
//      .then( () => MODEM.SetFrequency(433942000)) // conrad, OS/temp
//      .then( () => MODEM.SetFrequency(433900000)) // conrad, OS/temp
       .then( () => MODEM.Calibrate()) //conrad
//       .then( () => syncInfo()) //conrad
      .then( () =>
      {
        setInterval(() =>
        {
          if (COMM.onReceive)
            return;
/*
          if (!scanEnabled)
          {
            MODEM.Transfer().then( (data) => 
            {
              if (data.length&1)
                level = 1-level;
            });
            return;
          }
*/
          if (!started)
            return;

          MODEM.Transfer().then( (data) => 
          {
            if (data && data.length) 
            {
//              var flags = data.map(x=>x>>12);
//console.log(data);
              data = data.map(x=>{ var val = (x & 0x0fff)*20; var flag = x>>12; return (flag << 24) | val; });
              memory.push(data);
              for (var i=0; i<data.length; i++)
              {
                canvas.drawPulse(data[i], level);
                pulseMachine(data[i], level = 1-level);
              }
              canvas.drawPulseFinish();
            }
          })
          .then( () => MODEM.Status())
          .then( status => { if (status) console.log("Buffer overflow!"); })
          .catch(console.log);
        }, 200);

        console.log("Modem running");
        document_write("<pre>Modem running\n");
      });
}

 

var decoder = new Decoder();
document_write("<pre>started\n");
var buf = [];
var last;
function pulseMachinePush(i)
{
  if (i==-1)
  {
/*
    if (buf.length > 20 && buf.length < 1000)
    {
        k = 20;
        l = 0;
      var pulse = buf.map(x => x*k);
      var arr = JSON.stringify(pulse);
      console.log("l="+buf.length + " d=" + buf.reduce((a, b) => a+b, 0) + " " + arr);
      document_write("<a href=\"javascript:send("+arr+")\">"+arr.substr(0, 100)+"..."+"</a>\n");

      test360(pulse);

      var is500 = x => x >= 400 && x <= 540;
      var is1000 = x => x >= 920 && x <= 1020;
      var is750 = x => x >= 700 && x <= 750;
      if (is500(pulse[0]) && is500(pulse[1]) && is500(pulse[2]) && is500(pulse[3]) && is500(pulse[4]))
      {

        // quantize
        var quantize = [];
        for (var i=0; i<pulse.length; i++)
        {
          if (is500(pulse[i]))
            quantize.push(500);
          else if (is1000(pulse[i]))
            quantize.push(1000);
          else if (is750(pulse[i]))
            quantize.push(7500);
          else
            quantize.push("unknown_"+pulse[i]);
        }
        console.log(JSON.stringify(quantize));
      }
      //keyfobTest(buf);

      var good = false;
//      for (var k=14; k<60; k++)
      {
//        for (l=-6; l<=6; l++)
        {
          var d = decoder.decode(buf.map((x, i) => (x + ((i&1)*2-1)*l) * k ));
//          var d = decoder.decode(buf.map(x => x*k ));
          if (d.length)
          {
            document_write(k + "," + l + ": " + JSON.stringify(d)+"\n");
//            console.log(JSON.stringify(buf));
            console.log(d); 
            good = true;
//		break;
          }
        }
//        if (good) break;
      }
    }
*/
    buf = [];
    return;
  }
  buf.push(i);
}

var enableFraming = true;
var interval1 = 0, interval2 = 0, leading = true, terminated = false;
function pulseMachine(interval, level)
{
  if (interval == -1)
  {
    interval1 = 0;
    interval2 = 0;
    leading = true;
    pulseMachinePush(-1);
    return;
  }

  if (interval1 == 0)
  {
    interval += interval2;
  }
  
  if (interval > 1600 && enableFraming)
  {
    pulseMachinePush(-1);
    leading = true;     
  }

  if (interval1 != 0 && interval2 != 0)
  {
    if (interval2 > 1600 && enableFraming)
    {
      pulseMachinePush(-1);
      leading = true;     
    }

    if (leading)
    {
      if (level == 0)   
        leading = false;
    }
    else
    {
      pulseMachinePush(interval2);
    }
  }

  interval2 = interval1;
  interval1 = interval;
}

function analyse(buf)
{
/*
  var resp;
  if ((resp = analysekeyfob(buf)))
  {
    return resp;
  }        
*/
  var d = decoder.decode(buf);
  if (d.length)
  {
    console.log(JSON.stringify(buf));
    console.log(JSON.stringify(d));
    logadd(JSON.stringify(d));
  }

/*
//  var k = 20;
//  var l = 0;
//      for (var k=14; k<60; k++)
      {
//        for (l=-6; l<=6; l++)
        {
//          var d = decoder.decode(buf.map((x, i) => (x + ((i&1)*2-1)*l) * k ));
          var d = decoder.decode(buf);
          if (d.length)
          {
            console.log(JSON.stringify(buf));
            console.log(JSON.stringify(d));
//console.log(buf.map(x => x*k));
//            document_write(k + "," + l + ": " + JSON.stringify(d)+"\n");
//            console.log(JSON.stringify(buf));
//console.log("k="+k);
//            console.log(d); 
//            good = true;
//		break;
          }
        }
//        if (good) break;
      }
*/
}

Array.prototype.contains = function(v) {
  for (var i = 0; i < this.length; i++) {
    if (this[i] === v) return true;
  }
  return false;
};

Array.prototype.unique = function() {
  var arr = [];
  for (var i = 0; i < this.length; i++) {
    if (!arr.contains(this[i])) {
      arr.push(this[i]);
    }
  }
  return arr;
}

function sendPulse(pulse)
{
  if (COMM.onReceive)
  {
    console.log("Try later");
    return;
  }
  console.log([... pulse].sort((a,b)=>a-b).unique());

  // find gcd
  var quantum = pulse.reduce((a,b) => Math.min(a,b), pulse[0]);
  console.log("Quantum: " + quantum);

  var maxRem = pulse.reduce((a, b) => Math.max(a, b % quantum), 0); 
  if (maxRem != 0)
  {
    quantum /= 2;
    maxRem = pulse.reduce((a, b) => Math.max(a, b % quantum), 0); 
  }
  if (maxRem != 0)
    throw "cannot find common divisor";

  // in microseconds
  var buf8 = [];

  for (var i=0; i<pulse.length; i++)
  {
    buf8.push(pulse[i]&255);
    buf8.push(pulse[i]>>8);
  }

  var bufferPtr = 0;

  enableFraming = false;
  COMM.debug = true;
  BIOS.biosMemGetBufferPtr()
  .then( (x) => bufferPtr = x )
  .then( () => BIOS.biosMemBulkWrite(bufferPtr, buf8) )
//  .then( () => MODEM.SetFrequency(frequency) )
  .then( () => MODEM.SetDataRate(Math.floor(1000000/quantum)) )
  .then( () => MODEM.SetOutputPower(0xb0) )
  .then( () => MODEM.Send(bufferPtr, buf8.length/2, quantum) )
  .then( () => { enableFraming = true; COMM.debug = false; } )
  .then( () => MODEM.SetDataRate(4000) )
}

var lastTemp = 33.3;
function example()
{
  var proto = new CWeather()
  var attributes = proto.Example();
  var newt = prompt("Set temperature", lastTemp);
  if (newt)
    lastTemp = newt;

  attributes["temperature10"] = Math.floor(lastTemp*10); // 17.1 C

  var pulse = proto.Modulate(attributes);
  var aligned = [0, 2000, ... pulse]; 
  detail.show(aligned, 2, aligned.length+1);
}

/*
function analysekeyfob(nsig)
{
  var q = nsig.map(x => Math.floor((x+120)/240)).join("");
  var preamble = "22222222222222222222222";
  var pos = q.indexOf(preamble + "423333332"); //224");// + "22222222224");
  if (pos == -1)
  {
    console.log("keyfob: Preamble not found");
    return false; // 
  }
  q = q.substr(pos+9+preamble.length);
//  console.log(q);
  var r = "";
  for (var i =0; i<q.length; i++)
  {  
    if (q.substr(i, 2) == "22")
    {
      r += "A";
      i++;
    }
    else if (q.substr(i, 1) == "4")
    { 
      r += "B";
    }
    else 
    {
      r += "?";
      console.log("keyfob: Wrong data");
      return false;
    }
  }

  var cmd = "";
  if (r.substr(-7) == "AABAABB") // open 
  {
    cmd = "open";
  } else
  if (r.substr(-7) == "ABBBBBA") // close
  {
    cmd = "close";
  } else
  if (r.substr(-7) == "BBAABAA") // trunk
  {
    cmd = "trunk";
  } else
  {
    console.log("keyfob: No matching suffix - " + r.substr(-7) );
    return false;
  } 

  r = r.substr(0, r.length-7);
  if (r.length != 64 + 8)
  {
    console.log("keyfob: Wrong code length");
    return false;
  } 

  r = r.split("").map(c => c.charCodeAt(0) - 65);
//console.log(r);
  r = bitstreamToBytes(r);
  var rs = r.map(x=>("0"+x.toString(16)).substr(-2)).join(" ")
  console.log(rs + " " + cmd); // 77
  return {protocol:"keyfob", data:{payload:r, command:cmd}};
}

function reconstruct(p)
{
  if (!p || p.protocol != "keyfob")
    return false;
  var pulses = "222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222"
  pulses += "423333332";

  var codeToPulse = code => code.split("").map(x => ((x=="A") ? "22" : "4")).join("");
 
  for (var bits=0; bits<72; bits++)
  {
    var bit = p.data.payload[Math.floor(bits/8)] & (1 << (bits%8));
    if (bit == 0)
      pulses += "22";
    else
      pulses += "4";
  }

  if (p.data.command == "open")
    pulses += codeToPulse("AABAABB");
  else
  if (p.data.command == "close")
    pulses += codeToPulse("ABBBBBA");
  else
  if (p.data.command == "trunk")
    pulses += codeToPulse("BBAABAA");

  var k = 250;
  var intervals = pulses.split("").map(x=>parseInt(x)*k);
  intervals = [0, 2000, ...intervals];

  analysekeyfob(intervals);
  detail.show(intervals, 2, intervals.length);
}

function bitstreamToBytes(str)
{
  var bytes = [];
  for (var i=0; i<str.length; i++)
  {
    if ((i%8) == 0)
      bytes.push(0);
    bytes[Math.floor(i/8)] >>= 1;
    if (str[i])
      bytes[Math.floor(i/8)] |= 128;
  }
  return bytes;
}
*/

var configStruct, configPtr;
function configInit()
{
  return APP.GetConfigPtr()
    .then( _configPtr => { configPtr = _configPtr; return BIOS.memRead32(configPtr); } )
    .then( structurePtr => BIOS.GetString(structurePtr) )
    .then( json => configStruct = JSON.parse(json) )
}

function configGet(attr)
{
  var index = configStruct.indexOf(attr);
  return BIOS.memRead32(configPtr + index*4);
}

function configSet(attr, val)
{
  var index = configStruct.indexOf(attr);
  return BIOS.biosMemWrite(configPtr + index*4, [val & 0xff, (val >> 8) & 0xff, (val >> 16) & 0xff, (val >> 24) & 0xff]);
}

function attackEnable()
{
  if (COMM.onReceive)
  {
    console.log("Try later");
    return;
  }

  APP.GetConfigPtr().then( configPtr => BIOS.biosMemWrite(configPtr+4, [1]) );
}

function attackDisable()
{
  if (COMM.onReceive)
  {
    console.log("Try later");
    return;
  }

  APP.GetConfigPtr().then( configPtr => BIOS.biosMemWrite(configPtr+4, [0]) );
}

function calibrate()
{
  if (COMM.onReceive)
  {
    console.log("Try later");
    return;
  }

  MODEM.Calibrate();
}


function mupdate(o)
{
  if (o.id == "mfrequency") 
  {
    MODEM.SetFrequency(Math.floor(parseFloat(o.value)*1e6))
    .then( () => MODEM.GetFrequency() ) 
    .then( (freq) =>
    {
      INTERFACE.SetFrequency(freq);
      o.value = INTERFACE.GetFreqText();
      INTERFACE.Update();
      return INTERFACE.Finish();
    });
  }

  if (o.id == "mbandwidth")
  {
    MODEM.SetBandwidth(Math.floor(parseFloat(o.value)*1e3))
    .then( () => MODEM.GetBandwidth() ) 
    .then( (bw) =>
    {
      INTERFACE.SetBandwidth(bw);
      o.value = INTERFACE.GetBandText();
      INTERFACE.Update();
      return INTERFACE.Finish();
    });
  }

  if (o.id == "mgain")
  {
    MODEM.SetGain(Math.floor(parseFloat(o.value)))
    .then( () => MODEM.GetGain() ) 
    .then( (gain) =>
    {
      INTERFACE.SetGain(gain);
      o.value = INTERFACE.GetGainText();
      INTERFACE.Update();
      return INTERFACE.Finish();
    });
  }
}

class Interface {
  constructor()
  {
    this.tasks = [];
    this.frequency = 100e6;
    this.bandwidth = 100e3;
    this.gain = -10;
  }
  process(x)
  {
    this.tasks.push(x);
  }
  Finish()
  {
    if (this.tasks.length > 0)
      return this.tasks.shift()().then( () => INTERFACE.Finish() );
    return Promise.resolve();
  }
  GetFrequency()
  {
    return INTERFACE.frequency;  
  }
  SetFrequency(f)
  {
    INTERFACE.frequency = f
  }

  GetBandwidth()
  {
    return INTERFACE.bandwidth;  
  }
  SetBandwidth(f)
  {
    INTERFACE.bandwidth = f
  }

  GetGain()
  {
    return INTERFACE.gain;  
  }
  SetGain(f)
  {
    INTERFACE.gain = f
  }

  GetFreqText()
  {
    return (INTERFACE.frequency / 1e6).toFixed(2);
  }
  GetBandText()
  {
    return (INTERFACE.bandwidth / 1e3).toFixed(1);
  }
  GetGainText()
  {
    return INTERFACE.gain;
  }
  Update()
  {
    gui.drawModem();
    gui.drawStreaming();
  }
  GetStreaming()
  {
    return started;
  }
}

var INTERFACE = new Interface();
 
function syncInfo()
{
  var q = e => document.querySelector("#"+e);

  return MODEM.GetFrequency()
    .then(freq => { INTERFACE.SetFrequency(freq); q("mfrequency").value = (freq / 1e6).toFixed(2) })
    .then(() => MODEM.GetBandwidth())
    .then(bw => { INTERFACE.SetBandwidth(bw); q("mbandwidth").value = (bw / 1e3).toFixed(1) })
    .then(() => MODEM.GetGain())
    .then(gain => { INTERFACE.SetGain(gain); q("mgain").value = gain } )
    .then(() => gui.redraw());
//    .then(() => INTERFACE.Finish());
}

    var loginfo = document.createElement("textarea");
    loginfo.style = "width:90%; height:200px";
    document.documentElement.appendChild(loginfo)

function logadd(msg)
{
  loginfo.value = msg + "\n" + loginfo.value;
}