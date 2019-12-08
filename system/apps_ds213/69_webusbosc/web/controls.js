class Controls
{
  constructor()
  {
    this.elem = document.createElement("div");
    this.elem.style = "border:1px gray solid; width:1200px; height:160px;";
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
</style>

<div class="controlsContainer">
  <div class="controlsBlock">
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
  <div class="controlsBlock">
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
  <div class="controlsBlock">
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
    <select>
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
    TrigSource:<select>
      <option>CH1</option>
      <option>CH2</option>
    </select><br>
    Y-Offset:<input size=5 type="text" value="128">, 
    T-Value:<input size=5 type="text" value="128">

  </div><div class="controlsSpacer"></div>
  <div class="controlsBlock">
    Generator:<br><br>
    Flavour:<select>
      <option>Sine</option>
      <option>Square</option>
    </select><br>
    Frequency:<input size=8 type="text" value="1000"><br>
    Duty:<input size=8 type="text" value="50%" disabled><br>
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

    document.querySelector("#timebase").addEventListener('change', 
      (o) => INTERFACE.setTimebase(o.target.value));

  }
}
