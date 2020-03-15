class Renderer
{
  constructor(w, h)
  {
    this.width = w;
    this.height = h;
    this.container = document.createElement("div");
    this.container.style = "position:relative; width:1200px; display:block;  border:1px gray solid;";
    this.container.width = w;
    this.container.height = h;

    document.documentElement.style.background = "#404040";
    document.documentElement.appendChild(this.container);


    this.scrollContainer = document.createElement("div");
    this.scrollContainer.style = "overflow:auto; overflow-y: scroll;";
    this.scrollContainer.width = w;
    this.scrollContainer.height = h;
    this.container.appendChild(this.scrollContainer);

    this.elem = document.createElement("canvas");
    this.elem.width = 4096-30-15;
    this.elem.height = h;
    this.ctx = this.elem.getContext("2d");
    this.scrollContainer.appendChild(this.elem);

    this.controls = document.createElement("div");
    this.controls.width = w;
    this.controls.height = h;
    this.container.appendChild(this.controls);

    this.controls.innerHTML =
`
<style>

.label
{
  border-radius: 5px;
  border: 2px solid #000000;
  padding-left:10px;
  height:20px;
  width:80px;
  color:#000000;
}

#ch1pos
{
  position:absolute;
  left:5px;
  top:50px;
  background:rgba(255, 255, 0, 0.8);
}

#ch2pos
{
  position:absolute;
  left:5px;
  top:150px;
  background:rgba(0, 255, 255, 0.8);
}

#trigpos
{
  position:absolute;
  left:5px;
  top:250px;
  background:rgba(255, 255, 255, 0.8);
}

.button {
  position:absolute;
  top:20px;
  right:20px;
  width:100px;
  height:80px;

  background-color: #202020; /* Green */
  border-radius: 10px;
  border: none;
  color: white;
  text-decoration: none;
  display: inline-block;
  font-size: 60px;
  -webkit-transition-duration: 0.4s; /* Safari */
  transition-duration: 0.4s;
  cursor: pointer;
  opacity: 0.8;
}

.button1 {
  background-color: white; 
  color: red; 
  border: 2px solid #202020;
}

.button1:hover {
  background-color: #202020;
  color: white;
}

.button2 {
  right: 140px;
  background-color: white; 
  color: black; 
  border: 2px solid #202020;
}

.button2:hover {
  background-color: #202020;
  color: white;
}

</style>
<div class="label" id="ch1pos">CH1</div>
<div class="label" id="ch2pos">CH2</div>
<div class="label" id="trigpos">TRIG</div>
<button class="button button1" id="mode">&#x25cf;</button>
<button class="button button2" id="single">&#9658;&#10073;</button>
`;

    dragElement(document.querySelector("#ch1pos"), this.updateCh1Pos.bind(this));
    dragElement(document.querySelector("#ch2pos"), this.updateCh2Pos.bind(this));
    dragElement(document.querySelector("#trigpos"), this.updateTrigPos.bind(this));

    document.querySelector("#mode").addEventListener('click', 
      ((o) => 
    {
      INTERFACE.toggleTriggerState();
      this.onStop();
      o.target.innerHTML = INTERFACE.trigState == "run" ? "&#x25CF;" : "&#9724;" 
      document.querySelector("#mode").style.color = INTERFACE.trigState == "run" ? "rgb(255, 0, 0)" : "rgb(0, 0, 0)";
    }).bind(this));

    document.querySelector("#single").addEventListener('click', 
      ((o) => 
    {
      this.onStop();
      if (INTERFACE.trigState != "single")
      {
        INTERFACE.restart();
        INTERFACE.trigState = "single";

        this.waitingTimer = setInterval(() => 
        {
          o.target.style.color = (o.target.style.color == "rgb(255, 0, 0)") ? "rgb(0, 0, 0)" : "rgb(255, 0, 0)"; //innerHTML = INTERFACE.trigState == "run" ? "&#9899;" : "&#9724;" 
        }, 500);
      } else
      {
        INTERFACE.trigState = "stop";
      }
      gui.drawTrigger();
    }).bind(this));
  }
  Clear()
  {
    this.data = [];
    this.elem.width = this.elem.width;
  }
  Poly(p, c, w)
  {
    this.ctx.strokeStyle =c;
    this.ctx.lineJoin="round";
    this.ctx.lineWidth = w ? w : 5;
    this.ctx.beginPath();

    this.ctx.moveTo(p[0].x, p[0].y);
    for (var i=0; i<p.length; i++)
      this.ctx.lineTo(p[i].x, p[i].y);
    this.ctx.stroke();
  }
  Finish()
  {
  }
  sortOrder()
  {
    return 1;
  }
  GetRenderData()
  {
    return this.data;
  }

  setCh1Pos(p)
  {
//    p = p*CALIBRATION.ch1ZeroK+CALIBRATION.ch1ZeroQ;
    document.querySelector("#ch1pos").style.top = ((255-p)/255*this.height-10) + "px";
  }
  setCh2Pos(p)
  {
//    p = p*CALIBRATION.ch2ZeroK+CALIBRATION.ch2ZeroQ;
    document.querySelector("#ch2pos").style.top = ((255-p)/255*this.height-10) + "px";
  }
  setTrigPos(p)
  {
    document.querySelector("#trigpos").style.top = ((255-p)/255*this.height-10) + "px";
  }


  updateCh1Pos(y)
  {
    var y = parseInt(y);
    var p = 255-(y+10)*255/this.height;
//    var p0 = (p-28)*255/(255-28+5);
    var p0 = p;
    INTERFACE.setChannel1Offset(Math.floor(p0));
    controls.setChannel1Offset(Math.floor(p0))
  }

  updateCh2Pos(y)
  {
    var y = parseInt(y);
    var p = 255-(y+10)*255/this.height;
//    var p0 = (p-20)*255/(255-28+10);
    var p0 = p;
    INTERFACE.setChannel2Offset(Math.floor(p0));
    controls.setChannel2Offset(Math.floor(p0))
  }

  updateTrigPos(y)
  {
    var y = parseInt(y);
    var p = 255-(y+10)*255/this.height;
    var p0 = p;
    INTERFACE.setTriggerThreshold(Math.floor(p0));
    controls.setTriggerThreshold(Math.floor(p0))
  }

  getScrollOffset()
  { 
    return this.scrollContainer.scrollLeft;
  }

  onStop()
  {
    document.querySelector("#mode").innerHTML = "&#9724;";
    document.querySelector("#mode").style.color = "rgb(0, 0, 0)";
    document.querySelector("#single").style.color = "rgb(0, 0, 0)";
    if (this.waitingTimer)                 	
    {
      clearInterval(this.waitingTimer);
      this.waitingTimer = null;
    }
  }


  // osc graph
  OscilloscopeResampleNum(ofs)
  {
    var k = OSC.ResampleTable[INTERFACE.timebase];
    if (k==1)
      return ofs;
    return Math.floor(ofs*k);
  }

  OscilloscopeResample(data)
  {
    var k = OSC.ResampleTable[INTERFACE.timebase];
    if (k==1)
      return data;

    var Interpolate = (i) =>
    {
      var base = Math.floor(i);
      var part = i - base;
      var s0 = data[base];
      var s1 = data[base+1];
      var s = s0 + (s1-s0)*part;
      return s;
    };

    var aux = [];
    for (var i = 0; i<Math.floor(data.length*k); i++)
      aux.push(Interpolate(i*k));
    return aux;
  }

  OscilloscopeRedraw(ofs, rawdata)
  {
    var ypos = (v) => canvas.height-v*(canvas.height/256);

    var data1 = [], data2 = [], data3 = [], data4 = [];
    for (var i =0; i<rawdata.length; i+=3)
    {
      var bundle = [rawdata[i+0], rawdata[i+1], rawdata[i+2] & 1, rawdata[i+2] & 2];
      data1.push(bundle[0]);
      data2.push(bundle[1]);
      data3.push(bundle[2]&1);
      data4.push(bundle[2]&2);
    }

    var trigx = 120/OSC.ResampleTable[INTERFACE.timebase];
    var ofsx = 120 - trigx;
    ofs += ofsx;
    trigx = 120;

    this.lastOfs = ofs;

    data1 = this.OscilloscopeResample(data1);
    data2 = this.OscilloscopeResample(data2);

    for (var i=0; i<data1.length; i++)
      data1[i] = CALIBRATION.getCh1(data1[i]);

    for (var i=0; i<data2.length; i++)
      data2[i] = CALIBRATION.getCh2(data2[i]);

    var path1 = [];
    for (var i=0; i<data1.length; i++)
      path1.push({x:ofs+i, y:ypos(data1[i])});

    var path2 = [];
    for (var i=0; i<data2.length; i++)
      path2.push({x:ofs+i, y:ypos(data2[i])});

    this.Clear();

    var trig = ypos(INTERFACE.trigThreshold);
    var width = 4096;

    for (var x=0; x</*canvas.width*/ width; x+=30)
      canvas.Poly([{x:x, y:0}, {x:x, y:canvas.height}], "rgba(0, 0, 0, 0.8)", 1);

    for (var y=0; y<9; y++)
      canvas.Poly([{x:0, y:canvas.height*y/8}, {x:width, y:canvas.height*y/8}], "rgba(0, 0, 0, 0.8)", 1);

    this.Poly([{x:0, y:trig}, {x:width, y:trig}], "rgba(255, 255, 255, 0.6)", 1)
    this.Poly([{x:trigx, y:0}, {x:trigx, y:canvas.height}], "rgba(255, 255, 255, 0.6)", 1)

    this.Poly(path1, "#ffff00");
    this.Poly(path2, "#00ffff");
  }

  OscilloscopeRedrawGraphPart(ofs, rawdata, begin, end, channel, color)
  {
    var ypos = (v) => canvas.height-v*(canvas.height/256);

    var data = new Array(rawdata.length/3);
    for (var i=0, j=0; i<rawdata.length; i+=3, j++)
    {
      var bundle = [rawdata[i+0], rawdata[i+1], rawdata[i+2] & 1, rawdata[i+2] & 2];
      if (channel == "CH1")
        data[j] = bundle[0];
      if (channel == "CH2")
        data[j] = bundle[1];
    }

    var trigx = 120/OSC.ResampleTable[INTERFACE.timebase];
    var ofsx = 120 - trigx;
    ofs += ofsx;
    trigx = 120;
    begin = Math.floor(begin / OSC.ResampleTable[INTERFACE.timebase]);
    end = Math.floor(end / OSC.ResampleTable[INTERFACE.timebase]);

    data = this.OscilloscopeResample(data);

    for (var i=0; i<data.length; i++)
    {
      if (channel == "CH1")
        data[i] = CALIBRATION.getCh1(data[i]);
      else
      if (channel == "CH2")
        data[i] = CALIBRATION.getCh2(data[i]);
    }

    var path = [];
    for (var i=begin; i<=end; i++)
      path.push({x:ofs+i, y:ypos(data[i])});

    this.Poly(path, color, 3);
  }

  annotate(x, y, text, color)
  {
    var ypos = (v) => canvas.height-v*(canvas.height/256);
    var xpos = (x) => this.lastOfs + x / OSC.ResampleTable[INTERFACE.timebase]; // resampling!!!

    this.ctx.font = "20px Arial";
    this.ctx.fillStyle = color ? color : "white";
    this.ctx.strokeStyle = "#404040";
    this.ctx.textAlign = "center";
    this.ctx.strokeText(text, xpos(x), ypos(y)+32);
    this.ctx.fillText(text, xpos(x), ypos(y)+32);
  }
}

   	

function dragElement(elmnt, onFinish) {
  var pos1 = 0, pos2 = 0, pos3 = 0, pos4 = 0;
  var updTimer;
  var posLast = 0;

  elmnt.onmousedown = dragMouseDown;

  function dragMouseDown(e) {
    e = e || window.event;
    e.preventDefault();
    // get the mouse cursor position at startup:
//    pos3 = e.clientX;
    pos4 = e.clientY;
    document.onmouseup = closeDragElement;
    // call a function whenever the cursor moves:
    document.onmousemove = elementDrag;
    updTimer = setInterval(() => 
    {
      if (elmnt.style.top == posLast)
        return;
      posLast = elmnt.style.top;
      onFinish(elmnt.style.top);
    }, 800);
  }

  function elementDrag(e) {
    e = e || window.event;
    e.preventDefault();
    // calculate the new cursor position:
//    pos1 = pos3 - e.clientX;
    pos2 = pos4 - e.clientY;
//    pos3 = e.clientX;
    pos4 = e.clientY;
    // set the element's new position:
    elmnt.style.top = (elmnt.offsetTop - pos2) + "px";
//    elmnt.style.left = (elmnt.offsetLeft - pos1) + "px";
  }

  function closeDragElement() {
    // stop moving when mouse button is released:
    document.onmouseup = null;
    document.onmousemove = null;
    onFinish(elmnt.style.top);
    clearInterval(updTimer);
  }
}