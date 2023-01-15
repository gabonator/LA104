 window.addEventListener('DOMContentLoaded',function(){
    var sheet = document.createElement('style'); 
    sheet.type="text/css"; 
    sheet.innerHTML = `
#head {font-weight:bold; display: inline-block; width:40px; border:2px solid rgba(0, 0, 0, 0); border-style: hidden solid hidden hidden; margin-left:10px;}
#sig00 {padding-left:4px; background:#ffffff; display: inline-block; width:30px; border:1px solid transparent; border-left-color:#000; margin-left:10px;}
#sig01 {padding-left:4px; background:#d0d0d0; display: inline-block; width:30px; border:1px solid transparent; border-right-color:#000; border-top-color:#000; margin-left:10px;}
#sig10 {padding-left:4px; background:#ffffff; display: inline-block; width:30px; border:1px solid transparent; border-left-color:#000; border-top-color:#000; margin-left:10px;}
#sig11 {padding-left:4px; background:#d0d0d0; display: inline-block; width:30px; border:1px solid transparent; border-right-color:#000; margin-left:10px;}

#sigzz {padding-left:4px; background:#ffffff; display: inline-block; width:30px; border:1px solid transparent; margin-left:10px;}
#sigz0 {padding-left:4px; background:#ffffff; display: inline-block; width:30px; border:1px solid transparent; margin-left:10px;}
#sig0z {padding-left:4px; background:#ffffff; display: inline-block; width:30px; border:1px solid transparent; margin-left:10px;}
#sigz1 {padding-left:4px; background:#ffffff; display: inline-block; width:30px; border:1px solid transparent; margin-left:10px;}
#sig1z {padding-left:4px; background:#ffffff; display: inline-block; width:30px; border:1px solid transparent; margin-left:10px;}

#comment {display: inline-block; margin-left:10px;
  background: #c0c0f0;
  border-radius: 6px;
  padding-left:6px;
  padding-right:6px;
  font-weight:bold;
  color:#202020;
}

.global { padding-left:14px;}
.group {border:2px solid #d0d0d0; border-radius:6px; padding-left:12px;}
`; 
    document.body.appendChild(sheet);
    document.body.innerHTML += `
<div style="height:400px">
<div style="position:absolute; width:100%; height:400px; overflow:auto">
<canvas id="canvasBottom" style="width:4000px; height:400px;">
</canvas>
</div>
<canvas id="canvasTop" style="pointer-events: none; position:absolute; width:100%; height:400px;">
</canvas>
</div>
<div id="detail">
</div>
`
    main();

  }, false);

  var waveforms;
  var c;
  var ctx;
  var markerTimer = null;
  var markerPos = 0, markerScroll = 0;
  var markerSelectedBegin = -1;
  var markerSelectedEnd = -1;

function main()
{
  var c = document.getElementById("canvasBottom");
  var ctx = c.getContext('2d');
  ctx.fillStyle = "#008";
  ctx.font = "bold 30px Arial";
  c.width = 4000;
  c.height = 400;
  ctx.width = 4000;
  ctx.height = 400;

  var index=0;
  var gindex=0, gactive=0;
  ctx.fillStyle = "#b0b0b0";
  var groups = testbench_target.groups;
  waveforms = testbench_target.waveforms;

  for(var i in waveforms)
  {
    gindex = 0;
    gactive = 0;
    var wave = waveforms[i];
    ctx.beginPath();
    var lasty = -1;
    ctx.moveTo(0, 50*index+60-0);
    for (var j=0; j<wave.length*8; j++)
    {
      var i = j >> 3;
      var x = j;
      if (groups.length > gindex)
      {
        if (!gactive && j/8 >= groups[gindex].begin)
        {
            gactive = 1; //mGroups[gindex].color;
  //          ctx.fillStyle = groups[gindex].color;
            ctx.lineTo(x, 50*index+60-0);
            ctx.closePath();
            ctx.fill();
            ctx.beginPath();
            ctx.moveTo(x, 50*index+60-0);
        }
        if (gactive && j/8 >= groups[gindex].end)
        {
            gactive = 0;
            ctx.lineTo(x, 50*index+60-0);
            ctx.closePath();
            ctx.fillStyle = groups[gindex].color;
            ctx.fill();
            ctx.beginPath();
            ctx.moveTo(x, 50*index+60-0);
            ctx.fillStyle = "#b0b0b0";
            gindex++;
        }
      }

  /*
              if (mGroups.GetSize() > curGroup)
              {
                  if (!activeGroup && mWaveformShift+ii >= mGroups[curGroup].begin)
                  {
                      activeGroup = mGroups[curGroup].color;
                  }
                  if (activeGroup && mWaveformShift+ii >= mGroups[curGroup].end)
                  {
                      activeGroup = 0;
                      curGroup++;
                  }
              }

  */
      var sample = wave[i];
      var level = 0.5;
      switch (sample)
      {
        case '0': level = 0; break;
        case '1': level = 1; break;
        case 'x': level = j&1; break;
        case 'z': level = 0.5 + Math.cos(j/10*Math.PI)*0.1; break;
      }
      var y = 50*index+60-level*40;

        if (y != lasty)
          ctx.lineTo(x, lasty);
        ctx.lineTo(x, y);
      lasty = y;
    }
    ctx.lineTo(wave.length*8, 50*index+60-0);
  //  ctx.endPath();
  //  ctx.stroke();
    ctx.fill();
    index++;
  }

  var index=0;
  for(var i in waveforms)
  {
    var wave = waveforms[i];
    ctx.beginPath();
    var lasty = -1;
    ctx.moveTo(0, 50*index+60-0);
    for (var j=0; j<wave.length*8; j++)
    {
      var i = j >> 3;
      var x = j;
      var sample = wave[i];
      var level = 0.5;
      switch (sample)
      {
        case '0': level = 1/40; break;
        case '1': level = 1; break;
        case 'x': level = j&1; break;
        case 'z': level = 0.5 + Math.cos(j/10*Math.PI)*0.1; break;
      }
      var y = 50*index+60-level*40;

        if (y != lasty)
          ctx.lineTo(x, lasty);
        ctx.lineTo(x, y);
      lasty = y;
    }
    ctx.lineTo(wave.length*8, 50*index+60-0);
  //  ctx.endPath();
    ctx.stroke();
    index++;
  }


  var lineMax = [0, 0, 0];
  for (var i in testbench_target.labels)
  {
    var label = testbench_target.labels[i];
  //console.log(label);
    var x = label.pos*8;
    var y = 330+40-20;
    var line = 0;
    if (label.type==3)
      line = 1;
    if (label.type==5)
      line = 2;
    y += line*25;

    ctx.font = "bold 20px Arial";
    var width = ctx.measureText(label.text).width;
    if (x < lineMax[line])
      ctx.fillText("...", x, y);
    else {  
      ctx.fillText(label.text, x, y);
  //    ctx.fillStyle = "rgba(0, 0, 0, 0.3)"
  //    ctx.fillRect(x, y-20, width, 20);
  //    ctx.fillStyle = "#b0b0b0";
      lineMax[line] = x + width+30;
    }
    
  }

  c = document.getElementById("canvasTop");
  ctx = c.getContext('2d');
  c.width = c.clientWidth;
  c.height = 400;
  ctx.width = c.clientWidth;
  ctx.height = 400;
        let canvasElem = document.querySelector("#canvasBottom");
          
        canvasElem.addEventListener("mousemove", function(e)
        {
            getMousePosition(canvasElem, e);
        });
        canvasElem.addEventListener("mousedown", function(e)
        {
            markerSelectBegin();
        });
        canvasElem.addEventListener("mouseup", function(e)
        {
            markerSelectEnd();
        });

}

function markerUpdate()
{
  var c = document.getElementById("canvasTop");
  var ctx = c.getContext('2d');
  ctx.clearRect(0, 0, c.width, c.height);
  var grd = ctx.createLinearGradient(0, 0, 100, 0);
  grd.addColorStop(0, "rgba(255, 255, 255, 1)");
  grd.addColorStop(0.1, "rgba(255, 255, 255, 0.8)");
  grd.addColorStop(0.5, "rgba(255, 255, 255, 0.8)");
  grd.addColorStop(1, "rgba(255, 255, 255, 0");

  // Fill with gradient
  ctx.fillStyle = grd;
  ctx.fillRect(0, 0, 100, 350);

  var index = 0;
  for (var i in waveforms)
  {
    var label = i;
    var x = 10;
    var y = 50*(index++) +60-5;
    ctx.font = "bold 20px Arial";
    ctx.fillStyle = "#000000"
    ctx.fillText(label, x, y);
    
  }
 // if (markerPos < 0)
 //   markerPos = 0;

  var x = Math.floor(markerPos/8+0.5)*8;

//  markerPos -= 8;
  ctx.fillStyle = "#ff0000";
  ctx.fillRect(markerPos-markerScroll, 0, 1, 400);
  if (markerSelectedBegin != -1)
  {
    var end = markerSelectedEnd == -1 ? markerPos : markerSelectedEnd;
    ctx.fillStyle = "rgba(255, 0, 255, 0.2)";
    var xBegin = markerSelectedBegin-markerScroll;
    var xEnd = end-markerScroll;
    ctx.fillRect(xBegin, 0, xEnd - xBegin, 400);
  }
  var index=0;
  for(var i in waveforms)
  {
    var wave = waveforms[i];
    ctx.moveTo(0, 50*index+60-0);

      var sample = wave[x>>3];
      var level = 0.5;
      switch (sample)
      {
        case '0': level = 1/40; break;
        case '1': level = 1; break;
        case 'x': level = j&1; break;
        case 'z': level = 0.5 + Math.cos(0/10*Math.PI)*0.1; break;
      }
      var y = 50*index+60-level*40;
      var fofs = markerScroll;
      ctx.fillRect(x-2 - fofs, y-2, 5, 5);
    index++;
  }

}

function markerSelectBegin()
{
//  markerSelected = markerPos;
  var index = Math.floor(markerPos/8+0.5);
  markerSelectedEnd = -1;
  markerSelectedBegin = index*8;
  markerUpdate();
}

function getGroup(p)
{
  for (var i=0; i<testbench_target.groups.length; i++)
  {
    if (testbench_target.groups[i].begin <= p && p < testbench_target.groups[i].end)
      return testbench_target.groups[i];
  }
  return null;
}

function getGroupLabelBegin(g)
{
  return testbench_target.labels.find(x=>x.type == 2 && x.pos == g.begin)?.text
}

function getGroupLabelEnd(g)
{
  return testbench_target.labels.find(x=>x.type == 3 && x.pos == g.end)?.text
}

function getGroupLabel(g)
{
  return getGroupLabelBegin(g) + " / " + getGroupLabelEnd(g);
}

function markerSelectEnd()
{
//  markerSelected = markerPos;
  var index = Math.floor(markerPos/8+0.5);
  markerSelectedEnd = index*8;
  markerUpdate();
  var detail = "";
  for (var i in waveforms)
    detail += `<div id='head'>${i}</div>`;

  var curGroup = null; //getGroup(markerSelectedBegin/8);
//  if (curGroup)
//    detail += "<h1>" + getGroupLabel(curGroup).text + "</h2>";
/*
  groups: [
    {begin: 48, end: 176, color: "#f8b0b0"},
*/
    //gabo
  var prevlevels = "";
  for (var i=markerSelectedBegin/8; i<markerSelectedEnd/8; i++)
  {
    if (curGroup && curGroup.end == i)
    {
      detail += "</div></div>";
      curGroup = null;
    }
    if (!curGroup && (curGroup = getGroup(i)) != null)
    {
      var msg = getGroupLabel(curGroup);
      detail += (`<div class='group'><input type='button' value='Collapse: ${msg}' onClick='_.collapse(this);'><br><div class='group_contents'>`); 
    }
    if (i==index)
      detail += "<b>";

    var detailline = "";
    var levels = "";
    for (var j in waveforms)
    {
      var l0 = waveforms[j][i-1];
      var l1 = waveforms[j][i];
      detailline += `<div id='sig${l0}${l1}'>${l1}</div>`;
      levels += l1;
    }
    if (prevlevels != levels)
    {
      detail += "<br>";
      detail += detailline;
      prevlevels = levels;
    }
    
    var labels = testbench_target.labels.filter(x=>x.pos == i);
    for (var j of labels)
    {
      detail += `<div id='comment'>${j.text}</div>`;
    }

    if (i==index)
      detail += "</b>";
  }
  if (curGroup)
  {
      detail += "</div></div>";
  }
  document.getElementById("detail").innerHTML = detail;

}

function getMousePosition(canvas, event) {
            if (markerTimer)
              return;

            let rect = canvas.getBoundingClientRect();
            let x = event.clientX - rect.left;
            let y = event.clientY - rect.top;
            markerPos = x;
            markerScroll = -rect.left;
//            clearTimeout(markerTimer);
            markerUpdate();
            markerTimer = setTimeout(() => {markerTimer = null}, 100);
//            console.log("Coordinate x: " + x, 
//                        "Coordinate y: " + y, rect);
        }
      
