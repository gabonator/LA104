class Controls
{
  constructor()
  {
    this.elem = document.createElement("div");
    this.elem.style = "border:1px gray solid; width:1200px; height:360px; position:absolute; top:420px;";
    this.elem.innerHTML = `
<style>
.controlsContainer {
width:1200px;
height:160px;
padding-top:8px;
}

.controlsBlock 
{
border-radius: 25px;
border: 2px solid #73AD21;

color:#ffffff;
background:#202020;
width:180px;                  
height:120px;
padding:10px;
float:left;
}

.controlsSpacer
{
  float:left;
  width:35px;
  height:20px;
}

.controlsSpacerHalf
{
  float:left;
  width:21px;
  height:20px;
}

.blockCh1 { border: 2px solid #ffff00; }
.blockCh2 { border: 2px solid #00ffff; }
.blockTimeTrig { border: 2px solid #ffffff; }
.blockGen { border: 2px solid #ff00ff; }
.blockMeas { border: 2px solid #b0b0b0; }
.hidden {display:none;}
.blockConn { border: 2px solid #b0b0b0; }
.blockCalib { border: 2px solid #b0b0b0; }
.blockMemory { border: 2px solid #b0b0b0; }
</style>

<div class="controlsContainer">
  <div class="controlsSpacerHalf"></div>
  <div class="controlsBlock blockCh1">
    CH1:<br><br>
    Coupling: 
    <select id="ch1coupling">
     <option>AC</option>
     <option selected>DC</option>
<!--     <option>GND</option> -->
    </select><br>

    Range:
    <select id="ch1res">
     <option>50mV</option>
     <option>100mV</option>
     <option>200mV</option>
     <option selected>500mV</option>
     <option>1V</option>
     <option>2V</option>
     <option>5V</option>
     <option>10V</option>
    </select>&sol;div<br>
    <div class="hidden">Offset:
    <input id="ch1offset" size=5 type="text" value="20"></div>
  </div><div class="controlsSpacer"></div>
  <div class="controlsBlock blockCh2">
    CH2:<br><br>
    Coupling: 
    <select id="ch2coupling">
     <option>AC</option>
     <option selected>DC</option>
     <option>GND</option>
    </select><br>

    Range:
    <select id="ch2res">
     <option>50mV</option>
     <option>100mV</option>
     <option>200mV</option>
     <option selected>500mV</option>
     <option>1V</option>
     <option>2V</option>
     <option>5V</option>
     <option>10V</option>
    </select>&sol;div<br>
    <div class="hidden">Offset:
    <input id="ch2offset" size=5 type="text" value="20"></div>
  </div><div class="controlsSpacer"></div>
  <div class="controlsBlock blockTimeTrig">
    Timebase/Trigger:<br><br>
    T:
    <select id="timebase">
<!--      <option>100ns</option> -->
      <option>200ns</option>
      <option>500ns</option>
      <option>1us</option>
      <option>2us</option>
      <option>5us</option>
      <option>10us</option>
      <option>20us</option>
      <option>50us</option>
      <option>100us</option>
      <option>200us</option>
      <option>500us</option>
      <option selected>1ms</option>
      <option>2ms</option>
      <option>5ms</option>
      <option>10ms</option>
      <option>20ms</option>
      <option>50ms</option>
      <option>100ms</option>
      <option>200ms</option>
      <option>500ms</option>
      <option>1s</option>
    </select>&sol;div<br>
    Trigger:
    <select id="triggerMode">
      <option value="EdgeHL">Edge &#x21A7;</option>
      <option value="EdgeLH">Edge &#x21A5;</option>
      <option>LevelLow</option>
      <option>LevelHigh</option>
      <option>LowerDTLow</option>
      <option>GreaterDTLow</option>
      <option>LowerDTHigh</option>
      <option>GreaterDTHigh</option>
      <option value="None">Scan</option>
    </select><br>
    TrigSource:
    <select id="triggerSource">
      <option>CH1</option>
      <option>CH2</option>
    </select><br>
    <div class="hidden">Threshold:<input size=5 type="text" value="128" id="triggerThreshold"></div> 
    <span id="genTrigTime">Time:<input size=5 type="text" value="128" id="triggerTime"></span>

  </div><div class="controlsSpacer"></div>
  <div class="controlsBlock blockGen">
    Generator:<br><br>
    Flavour:<select id="genFlavour">
      <option>Sine</option>
      <option>Triangle</option>
      <option>Sawtooth</option>
      <option>Square</option>
      <option>DC</option>
      <option value ="Equation">Expression</option>
    </select><br>
    <span id="genCondEquation"><textarea cols=25 rows=3 id="genEquation"></textarea><br></span>
    <span id="genCondFreq">Frequency:<input size=8 type="text" value="1000" id="genFrequency"><br></span>
    <span id="genCondDuty">Duty:<input size=8 type="text" value="50" id="genDuty">%<br></span>
    <span id="genCondVolt">DC:<input size=8 type="text" value="1.000" id="genDc">V<br></span>
    <br>
  </div>
  <div class="controlsSpacer"></div>
  <div class="controlsBlock blockMeas">
    Measure:<br><br>
    Source:<select id="measSource">
      <option>CH1</option>
      <option>CH2</option>
    </select><br>
    Frequency:<input size=8 type="text" value="" id="measFrequency" disabled><br>
    Amplitude pk-pk:<input size=8 type="text" value="" id="measAmplitude" disabled><br>
    Average:<input size=8 type="text" value="" id="measAverage" disabled><br>
    <br>
  </div>
</div>
<div class="controlsContainer">
  <div class="controlsSpacerHalf"></div>
  <div class="controlsBlock blockCalib">
    Calibration:<input type="button" value="?" id="calibHelp"><br>
    <input type="button" value="Zero offset CH1" id="calibZeroCh1"><br>
    <input type="button" value="Zero offset CH2" id="calibZeroCh2"><br>
    Scale constants:<br>
    CH1: K[<span id="calibCh1Range"></span>]=<input size=6 type="text" value="" id="calibCh1K"><br>
    CH2: K[<span id="calibCh2Range"></span>]=<input size=6 type="text" value="" id="calibCh2K"><br>
    <br>
  </div>
  <div class="controlsSpacer"></div>
  <div class="controlsBlock blockConn">
    Analysis:<br>
    <br>
    <select id="analyserMode">
      <option>Off</option>
      <option>UART</option>
    </select><br>
  </div>
  <div class="controlsSpacer"></div>
  <div class="controlsBlock blockMemory">
    Memory:<br><br>
    <canvas id="memPreview" width="180" height="60" style="background:#404040;"></canvas>
    <select id="memSlot">
      <option value="0">Slot1</option>
      <option value="1">Slot2</option>
      <option value="2">Slot3</option>
      <option value="3">Slot4</option>
      <option value="4">Slot5</option>
      <option value="5">Slot6</option>
      <option value="6">Slot7</option>
      <option value="7">Slot8</option>
    </select>
    <button id="memLoad">Load</button>
    <button id="memSave">Save</button>
<!--    <br><button id="memExport">Export</button> -->
  </div>
  <div class="controlsSpacer"></div>
  <div class="controlsBlock blockConn">
    Connection:<br>
    <br>
    <button id="connect">Connect</button><br>
    <div id="status"></div
  </div>
</div>
`;
    document.documentElement.appendChild(this.elem);

    document.querySelector("#ch1res").addEventListener('change', 
      (o) => INTERFACE.setChannel1Range(o.target.value));

    document.querySelector("#ch1coupling").addEventListener('change', 
      (o) => INTERFACE.setChannel1Coupling(o.target.value));

    document.querySelector("#ch1offset").addEventListener('change', 
      (o) => INTERFACE.setChannel1Offset(o.target.value));

    document.querySelector("#ch2res").addEventListener('change', 
      (o) => INTERFACE.setChannel2Range(o.target.value));

    document.querySelector("#ch2coupling").addEventListener('change', 
      (o) => INTERFACE.setChannel2Coupling(o.target.value));

    document.querySelector("#ch2offset").addEventListener('change', 
      (o) => INTERFACE.setChannel2Offset(o.target.value));

    document.querySelector("#timebase").addEventListener('change', 
      (o) => INTERFACE.setTimebase(o.target.value));

    document.querySelector("#triggerMode").addEventListener('change', 
      (o) => {
        INTERFACE.setTriggerMode(o.target.value);
        document.querySelector("#genTrigTime").style.display = o.target.value.indexOf("DT") != -1 ? "block" : "none";
      });

    document.querySelector("#triggerSource").addEventListener('change', 
      (o) => INTERFACE.setTriggerSource(o.target.value));

    document.querySelector("#triggerThreshold").addEventListener('change', 
      (o) => INTERFACE.setTriggerThreshold(o.target.value));

    document.querySelector("#triggerTime").addEventListener('change', 
      (o) => INTERFACE.setTriggerTime(o.target.value));

    document.querySelector("#genFlavour").addEventListener('change', 
      (o) => {
        INTERFACE.setGeneratorFlavour(o.target.value)
        document.querySelector("#genCondDuty").style.display = o.target.value == "Square" ? "block" : "none";
        document.querySelector("#genCondFreq").style.display = o.target.value != "DC" ? "block" : "none";
        document.querySelector("#genCondVolt").style.display = o.target.value == "DC" ? "block" : "none";
        document.querySelector("#genCondEquation").style.display = o.target.value == "Equation" ? "block" : "none";
      });

    document.querySelector("#genFrequency").addEventListener('change', 
      (o) => INTERFACE.setGeneratorFrequency(o.target.value));

    document.querySelector("#genDuty").addEventListener('change', 
      (o) => INTERFACE.setGeneratorDuty(o.target.value));

    document.querySelector("#genDc").addEventListener('change', 
      (o) => INTERFACE.setGeneratorDc(o.target.value));

    document.querySelector("#measSource").addEventListener('change', 
      (o) => INTERFACE.setMeasSource(o.target.value));

    document.querySelector("#calibZeroCh1").addEventListener('click', 
      (o) => calibration.startOffsetCalibration("CH1"));

    document.querySelector("#calibZeroCh2").addEventListener('click', 
      (o) => calibration.startOffsetCalibration("CH2"));

    document.querySelector("#calibCh1K").addEventListener('change', 
      (o) => CALIBRATION.setCh1K(parseFloat(o.target.value)));

    document.querySelector("#calibCh2K").addEventListener('change', 
      (o) => CALIBRATION.setCh2K(parseFloat(o.target.value)));

    document.querySelector("#calibHelp").addEventListener('click', 
      (o) => alert("Connect CH1 to ground and run Zero offset calibration\nConnect known voltage to CH1 and finetune the CH1 K constant to get accurate reading in Measure section\nYou can use Generator in DC mode for calibration (check CALIBRATION.gen1V/gen2V values to get precise voltage at output)"));

    document.querySelector("#genEquation").addEventListener('change', 
      (o) => INTERFACE.setGeneratorEquation(o.target.value));

    document.querySelector("#genEquation").addEventListener('keypress', 
      (o) => 
      {
        if (o.key == "Enter") 
        {
          o.preventDefault();
          INTERFACE.setGeneratorEquation(o.target.value);
        }
      });

    document.querySelector("#analyserMode").addEventListener('change', 
      (o) => INTERFACE.setAnalyserMode(o.target.value));

    document.querySelector("#memSlot").addEventListener('change', 
      (o) => INTERFACE.memoryPreview(o.target.value));
    document.querySelector("#memLoad").addEventListener('click', 
      (o) => INTERFACE.memoryLoad());
    document.querySelector("#memSave").addEventListener('click', 
      (o) => INTERFACE.memorySave());
//    document.querySelector("#memExport").addEventListener('click', 
//      (o) => INTERFACE.memoryExport());
  }

  setChannel1Offset(v)
  {
    document.querySelector("#ch1offset").value = v;
  }

  setChannel2Offset(v)
  {
    document.querySelector("#ch2offset").value = v;
  }

  setTriggerThreshold(v)
  {
    document.querySelector("#triggerThreshold").value = v;
  }

  setMeasData(data)
  {
    document.querySelector("#measFrequency").value = data.frequency;
    document.querySelector("#measAmplitude").value = data.amplitude;
    document.querySelector("#measAverage").value = data.average;
  }

  setCh1Range()
  {
    document.querySelector("#calibCh1Range").innerHTML = INTERFACE.ch1range;
    document.querySelector("#calibCh1K").value = CALIBRATION.ch1Scale[INTERFACE.ch1range];
  }
  
  setCh2Range()
  {
    document.querySelector("#calibCh2Range").innerHTML = INTERFACE.ch2range;
    document.querySelector("#calibCh2K").value = CALIBRATION.ch2Scale[INTERFACE.ch2range];
  }

  load()
  {
    document.querySelector("#ch1res").value = INTERFACE.ch1range;
    document.querySelector("#ch1coupling").value = INTERFACE.ch1coupling;
    document.querySelector("#ch1offset").value = INTERFACE.ch1offset;
    document.querySelector("#ch2res").value = INTERFACE.ch2range;
    document.querySelector("#ch2coupling").value = INTERFACE.ch2coupling;
    document.querySelector("#ch2offset").value = INTERFACE.ch2offset;
    document.querySelector("#timebase").value = INTERFACE.timebase;
    document.querySelector("#triggerMode").value = INTERFACE.trigMode;
    document.querySelector("#triggerSource").value = INTERFACE.trigSource;
    document.querySelector("#triggerThreshold").value = INTERFACE.trigThreshold; 
    document.querySelector("#triggerTime").value = INTERFACE.trigTime;
    document.querySelector("#genTrigTime").style.display = INTERFACE.trigMode.indexOf("DT") != -1 ? "block" : "none";

    document.querySelector("#genFlavour").value = INTERFACE.genFlavour;
    document.querySelector("#genFrequency").value = INTERFACE.genFrequency;
    document.querySelector("#genDuty").value = INTERFACE.genDuty;
    document.querySelector("#genDc").value = INTERFACE.genDc;

    document.querySelector("#genCondDuty").style.display = INTERFACE.genFlavour == "Square" ? "block" : "none";
    document.querySelector("#genCondFreq").style.display = INTERFACE.genFlavour != "DC" ? "block" : "none";
    document.querySelector("#genCondVolt").style.display = INTERFACE.genFlavour == "DC" ? "block" : "none";
    document.querySelector("#genCondEquation").style.display = INTERFACE.genFlavour == "Equation" ? "block" : "none";

    document.querySelector("#genEquation").value = INTERFACE.genEquation;
    document.querySelector("#analyserMode").value = INTERFACE.analyse ? "UART" : "Off";
  }

  memPreview(rawdata)
  {
    var canvas = document.querySelector("#memPreview");
    var ctx = canvas.getContext("2d");

    var ypos = (v) => canvas.height-v*(canvas.height/256);

    var data1 = [], data2 = [];
    for (var i =0, x=0; i<rawdata.length && x < canvas.width; i+=5, x+=0.1)
    {
      var s = parseInt("0x" + rawdata.substr(i, 5));
      data1.push({x:x, y:ypos(s&255)});
      data2.push({x:x, y:ypos((s>>8)&255)});
    }

    var Poly = (p, c) =>
    {
      ctx.strokeStyle =c;
      ctx.lineJoin="round";
      ctx.lineWidth = 1;
      ctx.beginPath();
      ctx.moveTo(p[0].x, p[0].y);
      for (var i=0; i<p.length; i++)
        ctx.lineTo(p[i].x, p[i].y);
      ctx.stroke();
    }

    canvas.width = canvas.width;
    if (data1.length)
      Poly(data1, "#ffff00")
    if (data2.length)
      Poly(data2, "#00ffff");
  }
}
