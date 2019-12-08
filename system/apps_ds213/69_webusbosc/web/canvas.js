class Renderer
{
  constructor(w, h)
  {
    this.elem = document.createElement("canvas");
    this.elem.style = "border:1px gray solid;";
    this.elem.width = w;
    this.elem.height = h;
    document.documentElement.style.background = "#404040";
    document.documentElement.appendChild(this.elem);
    this.ctx = this.elem.getContext("2d");
  }
  Clear()
  {
    this.data = [];
    this.elem.width = this.elem.width;
  }
  Poly(p, c)
  {
    this.ctx.strokeStyle =c;
    this.ctx.lineJoin="round";
    this.ctx.lineWidth = 5;
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
}
