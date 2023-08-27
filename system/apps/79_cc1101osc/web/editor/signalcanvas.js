class SignalCanvas
{
  alignPos(pos)
  {
    var divs = document.querySelectorAll("div[role=SignalCanvas]")
    for (var d of divs)
      if (d != pos.e)
      {
        var x1 = pos.nx;
        var y1 = pos.ny;
        var x2 = d.getBoundingClientRect().left;
        var y2 = d.getBoundingClientRect().top + 103;
        if (Math.abs(x2-x1) < 5 && Math.abs(y2-y1) < 5)
        {
          pos.nx = x2;
          pos.ny = y2;
          pos.lock = d;
          //console.log("locking1");
          return;
        }
        var y2 = d.getBoundingClientRect().top - 103;
        if (Math.abs(x2-x1) < 5 && Math.abs(y2-y1) < 5)
        {
          pos.nx = x2;
          pos.ny = y2;
          pos.lock = d;
          //console.log("locking2");
          return;
        }
      }
    pos.lock = null;
  }
  attach(obj)
  {
    if (!obj)
    {
      if (this.attached)
      {
        //console.log("Detach!", this.attached); // TODO:
        this.attached.obj.removeEventListener('scroll', this.attached.handler, false);
        this.attached = null;
      }
      return;
    }
    var targetDiv = obj.querySelector("div:nth-child(2)");
    var handler = (e) => {
      this.scrollContainer.scrollLeft = e.currentTarget.scrollLeft + this.attached.offset;
    }
    //console.log("attach", targetDiv.scrollLeft, this.scrollContainer.scrollLeft);
    this.attached = {obj: targetDiv, handler: handler, offset:-targetDiv.scrollLeft+this.scrollContainer.scrollLeft};
    targetDiv.addEventListener('scroll', handler, false);
  }
  constructor(w, h)
  {
    this.width = w;
    this.height = h;
    this.container = document.createElement("div");
    this.container.role = "SignalCanvas";
    this.container.style = "position:absolute; top:50px; left:800px; width:800px; display:block; " +
      "background:#f8f8f8; border:3px #04b solid; border-radius:10px; ";
    this.attached = null;

    this.container.addEventListener('mousedown', (e) => {
      var drag = {x: e.clientX, y: e.clientY, e:e.currentTarget}
      var mouseMoveHandler = o => {
        this.attach(null);
        const dx = o.clientX - drag.x;
        const dy = o.clientY - drag.y;
        //var pos = {oldx:o.clientX, oldy:o.clientY, x:drag.e.offsetLeft + dx, y:drag.e.offsetTop + dy}
        drag.nx = drag.e.offsetLeft + dx;
        drag.ny = drag.e.offsetTop + dy;
        this.alignPos(drag)
        drag.e.style.top = `${drag.ny}px`;
        drag.e.style.left = `${drag.nx}px`;
        drag.x = o.clientX;
        drag.y = o.clientY;
      }
      var mouseUpHandler = e => {
        if (drag.lock)
          this.attach(drag.lock);
        document.removeEventListener('mousemove', mouseMoveHandler);
        document.removeEventListener('mouseup', mouseUpHandler);
      }
      document.addEventListener('mousemove', mouseMoveHandler);
      document.addEventListener('mouseup', mouseUpHandler);
    });

    this.controlsContainer = document.createElement("div");
    this.controlsContainer.style = "position:inline-block; float:left; border: 0px solid #ddd; width:50px; height:100px;";
    this.controlsContainer.innerHTML = `<i id="zoomin" class="fa fa-plus-circle fa-2x" style="padding-left:10px; padding-top:10px; color:rgba(110, 136, 255, 0.8)"></i>
<span id="zoom" style="padding-left:2px;">100&micro;s</span>
<i id="zoomout" class="fa fa-minus-circle fa-2x" style="padding-left:10px; color:rgba(110, 136, 255, 0.8)"></i>`

    this.controlsContainer.querySelector("#zoomin").addEventListener('mousedown', (e) => {
      this.zoomIn();
    });
    this.controlsContainer.querySelector("#zoomout").addEventListener('mousedown', (e) => {
      this.zoomOut();
    });
    this.zoomStatus = this.controlsContainer.querySelector("#zoom");
 
    this.controlsContainer.width = w;
    this.controlsContainer.height = h;
    this.container.appendChild(this.controlsContainer);

//    this.container.innerHTML = `<div style="position:inline-block; border: 1px solid #ddd; width:40px; height:100px">ahoj</div>`
    document.documentElement.appendChild(this.container);

    this.scrollContainer = document.createElement("div");
    this.scrollContainer.style = "overflow:auto; overflow-y: scroll; white-space:nowrap";
    this.scrollContainer.width = w;
    this.scrollContainer.height = h;
    this.scrollContainer.role = "Scroll";
    this.container.appendChild(this.scrollContainer);

    this.elems = [];
    for (var i=0; i<10; i++)
    {
      var elem = document.createElement("canvas");
      //elem.style = " background:" + (i==1 ? "red": "blue");
      elem.width = 1024;
      elem.height = h;
      this.scrollContainer.appendChild(elem);
      this.elems.push(elem);
    }

    this.pulse = [];
    this.level = [];
  }
  remove()
  {
    for (var i=0; i<10; i++)
      this.scrollContainer.removeChild(this.elems[i]);
    this.container.removeChild(this.scrollContainer);
    document.documentElement.removeChild(this.container);
  }
  Clear(elem)
  {
    elem.getContext("2d").clearRect(0, 0, elem.width, elem.height);
    elem.style = "display:inline-block";
  }

  Line(elem, x1, y1, x2, y2)
  {
    var ctx = elem.getContext("2d");
    ctx.beginPath();
    ctx.moveTo(x1, y1);
    ctx.lineTo(x2, y2);
    ctx.stroke();
  }

  Text(elem, x, y, text)
  {
    var ctx = elem.getContext("2d");
    ctx.font = "12px Arial";
    ctx.fillStyle = "#000000";
    ctx.textAlign = "center";
    ctx.fillText(text, x, y+10);
  }

  hide(elem)
  {
    if (!elem)
      this.container.style.display = "none";
    else
      elem.style="display: none;";
  }
  zoomIn()
  {
    this.zoom *= 2;
    this.render();
  }
  zoomOut()
  {
    this.zoom /= 2;
    this.render();
  }

  show(pulse, zoom)
  {
    this.container.style.display = "none";
    if (!pulse || pulse.type != "signal")
      throw "wrong type"
    this.container.style.display = "block";

    this.pulse = [...pulse.data];
    this.level = [...pulse.level];
    this.zoom = zoom;
    this.render();
  }

  render()
  {
    var sum = this.pulse.reduce((a,b) => a+b, 0)*this.zoom;
    for (var e of this.elems)
      this.hide(e);

    var scroll = 0;
    for (var i=0; i<this.elems.length; i++)
    {
      var curWidth = Math.min(2048, sum-scroll);
      if (curWidth <= 0)
        break;
      this.redraw(this.elems[i], curWidth, scroll, this.zoom);
      scroll += curWidth;
    }
    var dispUs = Math.floor(this.width/this.zoom);
    if (dispUs < 1000)
      this.zoomStatus.innerHTML = `${dispUs}&micro;s`
    else
      this.zoomStatus.innerHTML = `${(dispUs/1000).toFixed(1)}ms`
  }

  redraw(elem, w, ofs, zoom)
  {
    this.Clear(elem);

    elem.width = w;

    var x = -ofs;
    var y = 80;
    var c = "rgba(0, 0, 0, 0.5)";
    var ctx = elem.getContext("2d");
    ctx.strokeStyle = c;
    var sum = 0;

    for (var i=0; i<this.pulse.length; i++)
    {
      if (i==0 && this.pulse[i] == 0)
        continue;

      sum += this.pulse[i];

      var nx = x + this.pulse[i] * zoom;
      var ny = (i&1) ? 80 : 20;
      var flag = 0;
      if (this.level.length)
      {
        switch (this.level[i])
        {
          case "h":
            ny = 20;
            flag = 1;
            break;
          case "l":
            ny = 80;
            flag = 2;
            break;
          default:
            throw "bad flag"
        }
      }

      ctx.strokeStyle = c;
      ctx.lineWidth = 1;
      this.Line(elem, x, y, x, ny);

      if (flag)
      {
        ctx.strokeStyle = ["rgba(0, 0, 0, 0.5)", "#ff0000", "#0000ff", "#00ff00", "#ff00ff", "#ffff00", "#00ffff"][flag];
        ctx.lineWidth = 3;
      }
      else
        ctx.strokeStyle = c;

      this.Line(elem, x, ny, nx, ny);

      // draw text?
      if (nx-x > 30)
        this.Text(elem, (x+nx)/2, 50-4, this.pulse[i] & 0xffffff);
      x = nx;
      y = ny;
    }

    if (y != 80)
    {
      ny = 80;
      this.Line(elem, x, ny, x, ny);
    }

    ctx.strokeStyle = "rgba(0, 0, 0, 0.9)";
  }
}
