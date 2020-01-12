class Canvas
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
    this.elem.width = 4096;
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
<button class="button button1" id="mode">&#x25cf;</button>
<button class="button button2" id="single">&#9658;&#10073;</button>
`;

    document.querySelector("#mode").addEventListener('click', 
      ((o) => 
    {
    }).bind(this));

    document.querySelector("#single").addEventListener('click', 
      ((o) => 
    {
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

  RenderData(data)
  {
    var ypos = (v) => this.height-v*(this.height/256);

    var colors = ["#ffff00", "#00ffff", "#ff00ff", "#ff0000"];
    var basey = [350, 280, 210, 140];
    var stepy = -50;
    var scalex = 0.003; // 10 div -> 1ms, 100us/div
    var scalex = 0.03; // 10us/div

    var allSeries = [];
    for (var s=0; s<4; s++)
    {
      var series = [];
      var t = 0;
      for (var i=0; i<data.length-1; i++)
      {
        series.push({x:t, y:basey[s]+stepy*data[i].ch[s]});
        if (data[i].ch[s] != data[i+1].ch[s])
          series.push({x:t, y:basey[s]+stepy*data[i+1].ch[s]});

        t += data[i].t * scalex;
      }
      allSeries.push(series);
    }
    this.Clear();

    for (var x=0; x<this.width; x+=30)
      this.Poly([{x:x, y:0}, {x:x, y:this.height}], "rgba(0, 0, 0, 0.8)", 1);

    for (var y=0; y<9; y++)
      this.Poly([{x:0, y:this.height*y/8}, {x:this.width, y:this.height*y/8}], "rgba(0, 0, 0, 0.8)", 1);

    for (var s=0; s<4; s++)
      canvas.Poly(allSeries[s], colors[s]);
  }

}

