class RemoteGui
{
  constructor()
  {
    this.sections = [
      {color:0xffff00, name:"CH1", func:this.drawChannel1 },
      {color:0x00ffff, name:"CH2", func:this.drawChannel2 },
      {color:0xffffff, name:"Timebase", func:this.drawTimebase},
      {color:0xffffff, name:"Trigger", func:this.drawTrigger},
      {color:0xff00ff, name:"Generator", func:this.drawGenerator}
    ];
  }

  redraw()
  {
    // 400x240
    this.Background([0, 14, 400, 240-14], 0x404040, 0x202020);
    this.Bar([0, 240-14, 400, 240], 0x404040);
    this.Print(8, 240-14, 0xb0b0b0, 0x404040, "Connected!");

    var paddingx = 20;
    var spacingx = 20;
    var paddingy = 30;
    var spacingy = 20;
    var width = 400;
    var height = 240;
    for (var y=0; y<2; y++)
      for (var x=0; x<3; x++)
      {
        var section = this.sections[y*3+x];
        if (!section)
          continue;

        var rect = [
          paddingx+((width-2*paddingx+spacingx)/3)*x,
          paddingy+((height-2*paddingy+spacingy)/2)*y,
          paddingx+((width-2*paddingx+spacingx)/3)*(x+1)-spacingx,
          paddingy+((height-2*paddingy+spacingy)/2)*(y+1)-spacingy];

        var recttop = [rect[0], rect[1], rect[2], rect[1]+16];
        var rectbtm = [rect[0], rect[1]+16, rect[2], rect[3]];

        section.rect = rectbtm;
        this.Bar(recttop, section.color);
        this.Bar(rectbtm, 0x404040);
        this.Print(rect[0] + 4, rect[1], 0x000000, LCD.transparent, section.name);

        section.func.bind(this)(section.rect);
      }
  }

  drawChannel1()
  {
    var rect = this.sections[0].rect;
    this.Print(rect[0] + 4, rect[1] + 4 + 16*0, 0xffffff, 0x404040, INTERFACE.ch1coupling + "  ");
    this.Print(rect[0] + 4, rect[1] + 4 + 16*1, 0xffffff, 0x404040, (INTERFACE.ch1range + " /div     ").substr(0, 12));
  }                                                                                       

  drawChannel2()
  {
    var rect = this.sections[1].rect;
    this.Print(rect[0] + 4, rect[1] + 4 + 16*0, 0xffffff, 0x404040, INTERFACE.ch2coupling + "  ");
    this.Print(rect[0] + 4, rect[1] + 4 + 16*1, 0xffffff, 0x404040, (INTERFACE.ch2range + " /div   ").substr(0, 12));
  }

  drawTimebase()
  {
    var rect = this.sections[2].rect;
    this.Print(rect[0] + 4, rect[1] + 4 + 16*0, 0xffffff, 0x404040, INTERFACE.timebase + " /div   ");
  }

  drawTrigger()
  {
    var rect = this.sections[3].rect;
    this.Print(rect[0] + 4, rect[1] + 4 + 16*0, 0xffffff, 0x404040, (INTERFACE.trigState+"   ").substr(0, 12));
    this.Print(rect[0] + 4, rect[1] + 4 + 16*1, 0xffffff, 0x404040, (INTERFACE.trigMode+"     ").substr(0, 12));
    this.Print(rect[0] + 4, rect[1] + 4 + 16*2, 0xffffff, 0x404040, INTERFACE.trigSource+"   ");
  }

  drawGenerator()
  {
    var rect = this.sections[4].rect;
    this.Print(rect[0] + 4, rect[1] + 4 + 16*0, 0xffffff, 0x404040, (INTERFACE.genFlavour+"      ").substr(0, 12));
    if (INTERFACE.genFlavour != "DC")
      this.Print(rect[0] + 4, rect[1] + 4 + 16*1, 0xffffff, 0x404040, INTERFACE.genFrequency+" Hz   ");
    else
      this.Print(rect[0] + 4, rect[1] + 4 + 16*1, 0xffffff, 0x404040, "           ");

    if (INTERFACE.genFlavour == "DC")
      this.Print(rect[0] + 4, rect[1] + 4 + 16*2, 0xffffff, 0x404040, "DC: "+INTERFACE.genDc+"V  ");
    else if (INTERFACE.genFlavour == "Square")
      this.Print(rect[0] + 4, rect[1] + 4 + 16*2, 0xffffff, 0x404040, "Duty: "+INTERFACE.genDuty+"%  ");
    else
      this.Print(rect[0] + 4, rect[1] + 4 + 16*2, 0xffffff, 0x404040, "           ");
  }

  Background(rect, c0, c1)
  {
    INTERFACE.process(() => LCD.Background(rect, c0, c1));
  }

  Bar(rect, c)
  {
    INTERFACE.process(() => LCD.Bar(rect, c));
  }

  Print(x, y, cfront, cback, text)
  {
    INTERFACE.process(() => LCD.Print(x, y, cfront, cback, text));
  }
}