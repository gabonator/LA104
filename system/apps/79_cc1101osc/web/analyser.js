var canvas = new PreviewCanvas(900, 100);
var detail = new DetailCanvas(900, 100);
var gui = new RemoteGui();
var aaa;
var extWindows = [];
var lastSignal;

window.document.addEventListener('EditorLoaded', (e) => { 
  console.log("!!! LOADED", e.detail.data);
}, false);

function remoteAdd(url)
{
    var w = window.open(url, "_blank", "width=1200,height=800,location=no,menubar=no");
    w.onload = () => {
/*
        var event = new CustomEvent("SetParent", {detail:{
          data: [],
          sender: window.document
        }});
        w.document.dispatchEvent(event);

        console.log("LOAD1");
*/
      setTimeout(() =>
      {
        if (lastSignal)
          remoteAnalyse(lastSignal);
      }, 500);
    }
    extWindows.push(w);
}
function remoteAnalyse(buf)
{
  if (extWindows.length <= 0)
    return;

  var event = new CustomEvent("SignalChanged", {detail:{
    data: buf,
    sender: window.document
  }});

  for (var w of extWindows)
    w.document.dispatchEvent(event);
}

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
// TODO!    var polarity = 0; // last
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
    for (var i=1; i<aux.length; i++)
    {
      if (aux[i]==0)
      {
        aux[i-1] += aux[i+1];
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
//  if (buf[0] == 0 && buf[1] > 2000)
//    buf[1] = 2000;
  buf.shift(); // probably starts with gap

  var origbuf = [...buf];
  var ofsstart = 0;
  var ofsend = origbuf.length;

//  console.log(buf);
/*
  if (buf[0] == 0 && buf.length > 1)
  {
    buf.splice(0, 2);
    ofsstart += 2;
  }
*/
  if ((buf.length % 2) == 0)
  {
    buf.pop();
    ofsend--;
  }
  detail.show(origbuf, ofsstart, ofsend);
  aaa = buf.map(x=>x/20);
  console.log("analyse: " + JSON.stringify(buf));

  lastSignal = [...buf];
  remoteAnalyse(buf);
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
//document.querySelector("#dsend").addEventListener('click', () => { sendPulse(detail.trim(0)); });
document.querySelector("#openeditor").addEventListener('click', () => { remoteAdd("editor/index.html") });
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
  console.log("onstart");   // flush buf!
//  if (simulator) {started = true; return; }
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
  console.log("onstart");
//  if (simulator) {started = false; return; }
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
//            console.log(data.length, data[0], data[1]);
            console.log(data);
            if (data && data.length) 
            {
              if (data.length % 2 != 0)
                throw "not pairs";
              data = data.map(x=>x*20); // sampler @ 50k smps -> microseconds
//              var flags = data.map(x=>x>>12);
//console.log(data);
//              data = data.map(x=>{ var val = (x & 0x0fff)*20; var flag = x>>12; return (flag << 24) | val; });
              memory.push(data);
              for (var i=0; i<data.length; i++)
              {
                canvas.drawPulse(data[i], (i%2==0) ? 1 : 0);
                pulseMachine(data[i], (i%2==0) ? 1 : 0)
                //pulseMachine(data[i], level = 1-level);
              }
              canvas.drawPulseFinish();
            }
          })
//          .then( () => MODEM.Status())
//          .then( status => { if (status) { console.log("Buffer overflow!"); level = 1;} })
          .catch(console.log);
        }, 200);

        console.log("Modem running");
        document_write("<pre>Modem running\n");
      });
}

 
var pmGap = 0;
var pmActive = false;
var pmDuration = 0;
var pmData = [];
function pulseMachine(len, level)
{
  if (!pmActive)
  {
    if (level == 0)
      pmGap += len;
    else if (len > 0)
    {
      if (pmGap > 100000) // 100ms gap
      {
        //console.log("PM: start");
        pmActive = true;
        pmDuration = 0;
        pmData = [];
      }
      pmGap = 0;
    }
  }
  if (pmActive)
  {
    if (!level)
      pmGap += len;
    else
      pmGap = 0;
    if (pmGap > 30000) // 30 ms
    {
      //console.log("PM: stop");
      for (var i=1; i<pmData.length; i++)
      {
        if (pmData[i]==0)
        {
          pmData[i-1] += pmData[i+1];
          pmData.splice(i, 2);
          i--;
        }
      }
      if (pmData.length < 20)
        console.log("PM: short");
      else
      {
        console.log("PM: good", pmData);
        lastSignal = [...pmData];
        remoteAnalyse(pmData);
        analyse(pmData);
        detail.show(pmData, 0, pmData.length);

      }
      pmActive = false;
      return;
    }
    if (pmDuration > 3*1000*1000)
    {
      console.log("PM: too long");
      pmActive = false;
      return;
    }
    if (pmData.length > 5000)
    {
      console.log("PM: too complex");
      pmActive = false;
      return;
    }
    pmData.push(len);
    pmDuration += len;
  }
}

var decoder = new WasmRtl433("editor/");
document_write("<pre>started\n");


function analyse(buf)
{
  var d = decoder.decode(buf);
  if (d && d.length)
  {
    console.log(JSON.stringify(buf));
    console.log(JSON.stringify(d));
    logadd(JSON.stringify(d));
  }

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

downloadConfigRegs = [];

function downloadConfig()
{
  var regs = "IOCFG2, IOCFG1, IOCFG0, FIFOTHR, SYNC1, SYNC0, PKTLEN, PKTCTRL1, PKTCTRL0, ADDR, CHANNR, FSCTRL1, FSCTRL0, FREQ2, FREQ1, FREQ0, MDMCFG4, MDMCFG3, MDMCFG2, MDMCFG1, MDMCFG0, DEVIATN, MCSM2, MCSM1, MCSM0, FOCCFG, BSCFG, AGCCTRL2, AGCCTRL1, AGCCTRL0, WOREVT1, WOREVT0, WORCTRL, FREND1, FREND0, FSCAL3, FSCAL2, FSCAL1, FSCAL0, RCCTRL1, RCCTRL0, FSTEST, PTEST, AGCTEST, TEST2, TEST1, TEST0";
  regs = regs.split(" ").join("").split(",");
  downloadConfigRegs = regs.map((r, i) => {var obj = {}; obj.name=r; obj.id=i; return obj})
  var downloadConfigWorker = () =>
  {
    var i = downloadConfigRegs.findIndex(r => !("value" in r))
    if (i == -1)
    {
      var aux = {};
      downloadConfigRegs.forEach(r=>aux[r.name] = r.value);
      //console.log(JSON.stringify(aux));
      console.log(Object.values(aux).map(x=>("0"+x.toString(16)).substr(-2)).join(","));
      return Promise.resolve(aux)
    }
    return MODEM.Read(downloadConfigRegs[i].id)
      .then(v => downloadConfigRegs[i].value = v)
      .then(downloadConfigWorker)
  }
  return downloadConfigWorker();
}

function checkConfig()
{
  var ref = {"IOCFG2":41,"IOCFG1":46,"IOCFG0":13,"FIFOTHR":7,"SYNC1":211,"SYNC0":145,"PKTLEN":4,"PKTCTRL1":6,"PKTCTRL0":48,"ADDR":0,"CHANNR":0,"FSCTRL1":6,"FSCTRL0":0,"FREQ2":16,"FREQ1":176,"FREQ0":2,"MDMCFG4":151,"MDMCFG3":67,"MDMCFG2":48,"MDMCFG1":34,"MDMCFG0":248,"DEVIATN":71,"MCSM2":7,"MCSM1":0,"MCSM0":24,"FOCCFG":118,"BSCFG":108,"AGCCTRL2":47,"AGCCTRL1":0,"AGCCTRL0":146,"WOREVT1":135,"WOREVT0":107,"WORCTRL":248,"FREND1":182,"FREND0":17,"FSCAL3":239,"FSCAL2":43,"FSCAL1":24,"FSCAL0":31,"RCCTRL1":65,"RCCTRL0":0,"FSTEST":89,"PTEST":127,"AGCTEST":63,"TEST2":129,"TEST1":53,"TEST0":9}
  downloadConfig().then(cfg =>
  {
    var diff = false;
    for (var i in ref)
      if (ref[i] != cfg[i])
      {
        diff = true;
        console.log(i, "should be", ref[i], "but is", cfg[i])
      }
    if (!diff)
      console.log("configs are equal")
  })
  // noisy signal: AGCTEST should be 63 but is 23
}
function setConfig(j)
{
  var regs = "IOCFG2, IOCFG1, IOCFG0, FIFOTHR, SYNC1, SYNC0, PKTLEN, PKTCTRL1, PKTCTRL0, ADDR, CHANNR, FSCTRL1, FSCTRL0, FREQ2, FREQ1, FREQ0, MDMCFG4, MDMCFG3, MDMCFG2, MDMCFG1, MDMCFG0, DEVIATN, MCSM2, MCSM1, MCSM0, FOCCFG, BSCFG, AGCCTRL2, AGCCTRL1, AGCCTRL0, WOREVT1, WOREVT0, WORCTRL, FREND1, FREND0, FSCAL3, FSCAL2, FSCAL1, FSCAL0, RCCTRL1, RCCTRL0, FSTEST, PTEST, AGCTEST, TEST2, TEST1, TEST0";
  regs = regs.split(" ").join("").split(",");
  regs = regs.map((r, i) => {var obj = {}; obj.name=r; obj.id=i; return obj})
  for (var i in j)
  {
    var r = regs.find(r=>r.name == i);
    if (r)
    {
      console.log("Setting reg", r.id, "to value", j[i]);
      MODEM.Write(r.id, j[i])
    }
  }
}

    var loginfo = document.createElement("textarea");
    loginfo.style = "width:90%; height:200px";
    document.documentElement.appendChild(loginfo)

function logadd(msg)
{
  loginfo.value = msg + "\n" + loginfo.value;
}