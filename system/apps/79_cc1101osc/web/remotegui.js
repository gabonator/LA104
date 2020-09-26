class RemoteGui
{
  constructor()
  {
    // 0x2765d9
    this.sections = [
      {color:0x4040b0, name:"Modem", func:this.drawModem },
      {color:0x4040b0, name:"Streaming", func:this.drawStreaming },
/*
      {color:0xffffff, name:"Timebase", func:this.drawTimebase},
      {color:0xffffff, name:"Trigger", func:this.drawTrigger},
      {color:0xff00ff, name:"Generator", func:this.drawGenerator}
*/
    ];


    var paddingx = 20;
    var spacingx = 20;
    var paddingy = 30;
    var spacingy = 10;
    var width = 320;
    var height = 240;

    for (var y=0; y<2; y++)
      for (var x=0; x<1; x++)
      {
        var section = this.sections[y+x];
        if (!section)
          continue;

        section.rect = [
          paddingx+((width-2*paddingx+spacingx)/1)*x,
          paddingy+((height-2*paddingy+spacingy)/2)*y,
          paddingx+((width-2*paddingx+spacingx)/1)*(x+1)-spacingx,
          paddingy+((height-2*paddingy+spacingy)/2)*(y+1)-spacingy];
       }
  }

  redraw()
  {
    // 400x240
    this.Background([0, 14, 400, 240-14], 0x404040, 0x202020);
    this.Bar([0, 240-14, 400, 240], 0x404040);
    this.Print(8, 240-14, 0xb0b0b0, 0x404040, "Connected!");

    for (var y=0; y<2; y++)
      for (var x=0; x<1; x++)
      {
        var section = this.sections[y*1+x];
        if (!section)
          continue;

        var rect = section.rect;
        var recttop = [rect[0], rect[1], rect[2], rect[1]+16];
        var rectbtm = [rect[0], rect[1]+16, rect[2], rect[3]];

        //section.rect = rectbtm;
        this.Bar(recttop, section.color);
        this.Bar(rectbtm, 0x404040);
        this.Print(rect[0] + 4, rect[1], 0xffffff, LCD.transparent, section.name);

        section.func.bind(this)(section.rect);
      }
  }

  drawModem()
  {
    var rect = this.sections[0].rect;
    var items = [["Frequency: ", INTERFACE.GetFreqText() + " MHz"], ["Bandwidth: ", INTERFACE.GetBandText() + " kHz"], ["Gain: ", INTERFACE.GetGainText() + " dB"]];
    for (var i=0; i<items.length; i++)
    {
      var item = items[i]; 
      this.Print(rect[0] + 4, rect[1] + 16 + 4 + 16*i, 0xb0b0b0, 0x404040, item[0]);
      this.Print(rect[0] + 4 + item[0].length * 8, rect[1] + 4 + 16 + 16*i, 0xffffff, 0x404040, item[1] + "     ");
    }
  }                                                                                       

  drawStreaming()
  {
    var rect = this.sections[1].rect;
    var items = [["Sampling: ", "50 kHz"], [INTERFACE.GetStreaming() ? "Running..." : "Paused", ""]];
    for (var i=0; i<items.length; i++)
    {
      var item = items[i]; 
      this.Print(rect[0] + 4, rect[1] + 16 + 4 + 16*i, 0xb0b0b0, 0x404040, item[0]);
      this.Print(rect[0] + 4 + item[0].length * 8, rect[1] + 4 + 16 + 16*i, 0xffffff, 0x404040, item[1] + "     ");
    }
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