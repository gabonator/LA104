class Controls
{
  constructor()
  {
    this.elem = document.createElement("div");
    this.elem.style = "border:1px gray solid; width:1200px; height:160px; position:absolute; top:420px;";
    this.elem.innerHTML = `
<style>
.controlsContainer {
width:1200px;
height:180px;
}

.controlsBlock 
{
border-radius: 25px;
border: 2px solid #73AD21;

color:#ffffff;
background:#202020;
width:220px;                  
height:120px;
padding:10px;
float:left;
}

.controlsSpacer
{
  float:left;
  width:50px;
  height:50px;
}

.blockCh1 { border: 2px solid #ffff00; }
.blockCh2 { border: 2px solid #00ffff; }
.blockTimeTrig { border: 2px solid #ffffff; }
.blockGen { border: 2px solid #ff00ff; }
</style>

<div class="controlsContainer">
  <div class="controlsBlock blockCh1">
    CH1:<br><br>
    Coupling: 
    <select id="ch1coupling">
     <option>AC</option>
     <option selected>DC</option>
     <option>GND</option>
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
    Offset:
    <input id="ch1offset" size=5 type="text" value="20">
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
    Offset:
    <input id="ch2offset" size=5 type="text" value="20">
  </div><div class="controlsSpacer"></div>
  <div class="controlsBlock blockTimeTrig">
    Timebase:<br><br>
    T:
    <select id="timebase">
      <option>100ns</option>
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
      <option>EdgeHL</option>
      <option>EdgeLH</option>
      <option>LevelLow</option>
      <option>LevelHigh</option>
      <option>LowerDTLow</option>
      <option>GreaterDTLow</option>
      <option>LowerDTHigh</option>
      <option>GreaterDTHigh</option>
      <option>Scan</option>
    </select><br>
    TrigSource:
    <select id="triggerSource">
      <option>CH1</option>
      <option>CH2</option>
    </select><br>
    Threshold:<input size=5 type="text" value="128" id="triggerThreshold">, 
    Time:<input size=5 type="text" value="128" id="triggerTime">

  </div><div class="controlsSpacer"></div>
  <div class="controlsBlock blockGen">
    Generator:<br><br>
    Flavour:<select id="genFlavour">
      <option>Sine</option>
      <option>Square</option>
    </select><br>
    Frequency:<input size=8 type="text" value="1000" id="genFrequency"><br>
    Duty:<input size=8 type="text" value="50" disabled id="genDuty">%<br>
    <br>
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
      (o) => INTERFACE.setTriggerMode(o.target.value));

    document.querySelector("#triggerSource").addEventListener('change', 
      (o) => INTERFACE.setTriggerSource(o.target.value));

    document.querySelector("#triggerThreshold").addEventListener('change', 
      (o) => INTERFACE.setTriggerThreshold(o.target.value));

    document.querySelector("#triggerTime").addEventListener('change', 
      (o) => INTERFACE.setTriggerTime(o.target.value));

    document.querySelector("#genFlavour").addEventListener('change', 
      (o) => {
        INTERFACE.setGeneratorFlavour(o.target.value)
        document.querySelector("#genDuty").disabled = o.target.value != "Square";
      });

    document.querySelector("#genFrequency").addEventListener('change', 
      (o) => INTERFACE.setGeneratorFrequency(o.target.value));

    document.querySelector("#genDuty").addEventListener('change', 
      (o) => INTERFACE.setGeneratorDuty(o.target.value));
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
}
