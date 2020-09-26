class PreviewCanvas
{
  constructor(w, h)
  {
    this.width = w;
    this.height = h;
    this.container = document.createElement("div");
    this.container.style = "position:relative; display:block;  border:1px gray solid; overflow:hidden; width:"+w+"px; height:"+h+"px;";
    this.container.width = w+40;
    this.container.height = h;

//    document.documentElement.style.background = "#404040";
    document.documentElement.appendChild(this.container);

/*
    this.scrollContainer = document.createElement("div");
    this.scrollContainer.style = "overflow:auto; overflow-y: scroll;";
    this.scrollContainer.width = w;
    this.scrollContainer.height = h;
    this.container.appendChild(this.scrollContainer);
*/
    this.elem1 = document.createElement("canvas");
    this.elem1.style = "border:1px red solid; position:absolute;";
    this.elem1.width = w;
    this.elem1.height = h;
    this.ctx1 = this.elem1.getContext("2d");
    this.container.appendChild(this.elem1);
//    this.scrollContainer.appendChild(this.elem);

    this.elem2 = document.createElement("canvas");
    this.elem2.style = "border:1px blue solid; position:absolute;";
    this.elem2.width = w;
    this.elem2.height = h;
    this.ctx2 = this.elem2.getContext("2d");
    this.container.appendChild(this.elem2);

    this.elem3 = document.createElement("canvas");
    this.elem3.style = "border:1px green solid; position:absolute;";
    this.elem3.width = w;
    this.elem3.height = h;
    this.ctx3 = this.elem3.getContext("2d");
    this.container.appendChild(this.elem3);

    this.registerMouse(this.elem3);
/*
    this.elem3.addEventListener('click', function() {
    });
*/

    this.ctx1.strokeStyle = "rgba(0, 0, 0, 0.5)";
    this.ctx2.strokeStyle = "rgba(0, 0, 0, 0.5)";
    this.ctx3.strokeStyle = "rgba(0, 0, 0, 0.5)";
/*
    this.controls = document.createElement("div");
    this.controls.width = w;
    this.controls.height = h;
    this.container.appendChild(this.controls);
    this.controls.innerHTML = "";
*/

    this.animatorInit();
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

  Line(x1, y1, x2, y2)
  {
//    if (x1 <= this.width)
    {
      if (x1!=x2)
        this.ctx1.clearRect(x1+1, 0, x2-x1+20, this.height);
      this.ctx1.beginPath();
      this.ctx1.moveTo(x1, y1);
      this.ctx1.lineTo(x2, y2);
      this.ctx1.stroke();
      x1 -= this.width;
      x2 -= this.width;
    }

    if (x1!=x2)
      this.ctx2.clearRect(x1+1, 0, x2-x1+20, this.height);
    this.ctx2.beginPath();
    this.ctx2.moveTo(x1, y1);
    this.ctx2.lineTo(x2, y2);
    this.ctx2.stroke();

    x1 -= this.width;
    x2 -= this.width;
    if (x2 > 0)
    {
      this.ctx1.clearRect(x1+1, 0, x2-x1+20, this.height);
      this.ctx1.beginPath();
      this.ctx1.moveTo(x1, y1);
      this.ctx1.lineTo(x2, y2);
      this.ctx1.stroke();
    }
  }

  // animator
  animatorInit()
  {
    this.zoom = 1/200/20;

    this.drawX = 0;
    this.drawY = 20;
    this.animOffset = 0;
    this.animOffsetAt = 0;
    this.animLast = 0;
    this.animLastAt = 0;

    setInterval((() =>
    {
      this.animatorUpdate();
    }).bind(this), 20);
  }

  zoomIn()
  {
    this.zoom *= 2;
  }

  zoomOut()
  {
    this.zoom /= 2;
  }

  animatorUpdate()
  {
    if (this.animOffset != this.animLast)
      this.drawToOffset(this.calculateOffset());
  }

  drawPulse(len, level)
  {
    if (len == 0)
      return;         

    this.ctx1.strokeStyle = ["rgba(0, 0, 0, 0.5)", "#ff0000", "#00ff00", "#0000ff", "#ff00ff", "#ffff00", "#00ffff"][len>>24];
    this.ctx2.strokeStyle = ["rgba(0, 0, 0, 0.5)", "#ff0000", "#00ff00", "#0000ff", "#ff00ff", "#ffff00", "#00ffff"][len>>24];

    var x0 = this.drawX;
    this.drawX += (len&0xffffff)*this.zoom;
    var x1 = this.drawX;
    if (this.drawX>=this.width*2)
      this.drawX -= this.width*2;
    var y1 = level ? 80 : 20;
    if (this.drawY != y1)
      this.Line(x0, this.drawY, x0, y1);
    this.Line(x0, y1, x1, y1);
    this.drawY = y1;
  }

  drawPulseFinish()
  {
    var now = (new Date()).getTime();
    // converge from calculateOffset to shouldOffset in 300ms
    var last = this.calculateOffset();
    this.animOffset = this.drawX;
    this.animOffsetAt = now+300;
    this.animLast = last;
    this.animLastAt = now;

    this.Line(this.drawX+2, 0, this.drawX+2, 100);

  }

  calculateOffset()
  {
    if (this.animOffsetAt == this.animLastAt)
      return 0;

    var now = (new Date()).getTime();

    var t = (now - this.animLastAt) / (this.animOffsetAt - this.animLastAt); // 0..1
    if (t > 1.3)
      t = 1.3;
    var aux;
    if (this.animOffset < this.animLast)
      aux = this.animLast + ((this.animOffset+2*this.width)-this.animLast)*t - 2*this.width;
    else
      aux = this.animLast + (this.animOffset-this.animLast)*t;
    return aux;
  }

  drawToOffset(x)
  {
    this.elem1.style.left = Math.floor(this.width-x) + "px";

    if (x > this.width)
      x -= 2*this.width;
    this.elem2.style.left = Math.floor(-x) + "px";
  }

  // mouse
  registerMouse(element)
  {
    this.mouseDown = false;

    element.addEventListener('mousedown', (e => {
      var x = e.offsetX;
      var y = e.offsetY;
      this.mouseDown = true;
      this.mouseFirst = {x:x, y:y};
      this.mouseLast = {};
    }).bind(this));

    element.addEventListener('mousemove', (e => {
      if (this.mouseDown === true) {
//        drawLine(context, x, y, e.offsetX, e.offsetY);
        var x = e.offsetX;
        var y = e.offsetY;
        this.mouseLast = {x:x, y:y};
        this.highlightRange(this.mouseFirst.x, this.mouseLast.x);
      }
    }).bind(this));

    window.addEventListener('mouseup', (e => {
      if (this.mouseDown === true) {
        this.mouseDown = false;
//        console.log(this.coordToOffset(this.mouseFirst.x)-this.drawX, this.coordToOffset(this.mouseLast.x)-this.drawX);
        var times = [this.coordToTime(this.mouseFirst.x), this.coordToTime(this.mouseLast.x)];
        times = times.map(Math.floor);
        times.sort((a, b) => a-b);
//        times.reverse();
        dumpRange(times[0], times[1]);
      }
    }).bind(this));
  }

  coordToTime(x)
  {
    return (this.coordToOffset(x)-this.drawX)/this.zoom;
  }

  coordToOffset(x)
  {
    if (this.elem1.offsetLeft > this.elem2.offsetLeft)
      return x-this.elem1.offsetLeft;
    else
      return x-this.elem2.offsetLeft+this.width;
  }

  highlightRange(x1, x2)
  {
    this.ctx3.clearRect(0, 0, this.width, this.height);
    this.ctx3.fillStyle="rgba(10,60,250,0.1)";
    this.ctx3.fillRect(x1,0,x2-x1,this.height);

  }
}

class DetailCanvas
{
  constructor(w, h)
  {
    this.width = w;
    this.height = h;
    this.container = document.createElement("div");
    this.container.style = "position:relative; width:1200px; display:block;  border:1px gray solid;";
    this.container.width = w;
    this.container.height = h;

    document.documentElement.appendChild(this.container);

    this.scrollContainer = document.createElement("div");
    this.scrollContainer.style = "overflow:auto; overflow-y: scroll;";
    this.scrollContainer.width = w;
    this.scrollContainer.height = h;
    this.container.appendChild(this.scrollContainer);

    this.elem = document.createElement("canvas");
    this.elem.width = w*2;
    this.elem.height = h;
    this.ctx = this.elem.getContext("2d");
    this.scrollContainer.appendChild(this.elem);
    this.zoom = 1/10/20;

    this.pulse = [];
    this.range = [0, 0];
  }

  Clear()
  {
    this.ctx.clearRect(0, 0, this.elem.width, this.elem.height);
  }

  Line(x1, y1, x2, y2)
  {
    this.ctx.beginPath();
    this.ctx.moveTo(x1, y1);
    this.ctx.lineTo(x2, y2);
    this.ctx.stroke();
  }

  Text(x, y, text)
  {
    this.ctx.font = "12px Arial";
    this.ctx.fillStyle = "#000000";
//    this.ctx.strokeStyle = "#ffffff";
    this.ctx.textAlign = "center";
//    this.ctx.strokeText(text, x, y+10);
    this.ctx.fillText(text, x, y+10);
  }

  show(pulse, begin, end)
  {
    //console.log("show: " + JSON.stringify(pulse) + " " + begin + "-" + end);
    this.pulse = [...pulse];
    this.range = [begin, end];
    this.redraw();          	
  }

  redraw()
  {
    this.Clear();

    var sum = this.pulse.reduce((a,b) => a+(b&0xffffff), 0)*this.zoom;
    this.elem.width = sum;

    var x = 0;
    var y = 80;
    var c = "rgba(0, 0, 0, 0.3)";
    this.ctx.strokeStyle = c;
    var sum = 0;

    for (var i=0; i<this.pulse.length; i++)
    {
      if (i==0 && (this.pulse[i] & 0xffffff) == 0)
        continue;

      sum += this.pulse[i]  & 0xffffff;

      var nx = x + (this.pulse[i] & 0xffffff) *this.zoom;
      var ny = (i&1) ? 80 : 20;

      var flag = this.pulse[i] >> 24;
      if (flag)
        this.ctx.strokeStyle = ["rgba(0, 0, 0, 0.5)", "#ff0000", "#00ff00", "#0000ff", "#ff00ff", "#ffff00", "#00ffff"][flag];
      else
        this.ctx.strokeStyle = c;
                                                                                                                                                                	
      if (i == this.range[0])
        this.ctx.strokeStyle = c = "rgba(0, 0, 0, 1)";

      if (i == this.range[1] && ny != 80)
        this.ctx.strokeStyle = c = "rgba(0, 0, 0, 0.3)";

      this.Line(x, y, x, ny);

      if (i == this.range[1] && ny == 80)
        this.ctx.strokeStyle = c = "rgba(0, 0, 0, 0.3)";

      this.Line(x, ny, nx, ny);

      // draw text?
      if (nx-x > 30)
      {
        this.Text((x+nx)/2, 50-4, this.pulse[i] & 0xffffff);
//        this.Text((x+nx)/2, 50+8, (this.pulse[i] & 0xffffff)/20);
//        this.Text(x, 50+8+25, sum/20);
      }
      x = nx;
      y = ny;
    }

    if (y != 80)
    {
      ny = 80;
      this.Line(x, ny, x, ny);
    }

    this.ctx.strokeStyle = "rgba(0, 0, 0, 0.9)";
  }

  zoomIn()
  {
    this.zoom *= 2;
    this.redraw();
  }

  zoomOut()
  {
    this.zoom /= 2;
    this.redraw();
  }

  quantize()
  {
    var q = this.pulse;
    var k = 500;

    for (var i=this.range[0]; i<this.range[1]; i++)
    {
      var d = q[i];
      if (d < 100 && i > 0)
      {
        if (d==0) throw "error";
        q[i-1] += d;
        q[i-1] += q[i+1];
        q.splice(i,2);
        this.range[1]-=2;
        i--;
        continue;
      }
//      d = Math.floor(d/200)*200;
/*
      if (d >= 180 && d <= 480)
        d = 500;
      if (d >= 1700 && d <= 2000)
        d = 2000;
      if (d >= 3700 && d <= 4100)
        d = 4000;
      if (d >= 8200 && d <= 9000)
        d = 9000;
*/
      if (d >= 400 && d <= 560)
        d = k;
      if (d >= 900 && d <= 1100)
        d = k*2;
      if (d >= 580 && d <= 780)
        d = k*1.5;

      q[i] = d;
    }
//    this.pulse = q.map(x => Math.floor(x/20));
    return this.trim(0);
  }

  trim(delta)
  {
    this.range[1] += delta;
    this.redraw();  

    var quantized = [...this.pulse]; //.map(x => x*20);
    quantized = quantized.splice(this.range[0], this.range[1]-this.range[0]); //?
    console.log(quantized);
    return quantized;
  }
}

