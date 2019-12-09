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
    this.elem.width = 4096-25;
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

</style>
<div class="label" id="ch1pos">CH1</div>
<div class="label" id="ch2pos">CH2</div>
<div class="label" id="trigpos">TRIG</div>
`;

    dragElement(document.querySelector("#ch1pos"), this.updateCh1Pos.bind(this) );
    dragElement(document.querySelector("#ch2pos"), this.updateCh2Pos.bind(this));
    dragElement(document.querySelector("#trigpos"), this.updateTrigPos.bind(this));
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
    p = p/255*(255-28+5)+28;
    document.querySelector("#ch1pos").style.top = ((255-p)/255*this.height-10) + "px";
  }
  setCh2Pos(p)
  {
    p = p/255*(255-28+10)+20;
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
    var p0 = (p-28)*255/(255-28+5);
    INTERFACE.setChannel1Offset(Math.floor(p0));
    controls.setChannel1Offset(Math.floor(p0))
  }

  updateCh2Pos(y)
  {
    var y = parseInt(y);
    var p = 255-(y+10)*255/this.height;
    var p0 = (p-20)*255/(255-28+10);
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