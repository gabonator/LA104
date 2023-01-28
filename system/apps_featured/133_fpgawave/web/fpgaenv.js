testbenches = [];
if (typeof(testbench_target) != "undefined")
  testbenches.push(testbench_target);
if (typeof(testbench_verilator) != "undefined")
  testbenches.push(testbench_verilator);

var testbench;
calculateTestbench();

function checkChildren()
{
  if (self != top)
  {
    // if we are iframe embedded inside other window, post our data to parent
    if (typeof(testbench_target) != "undefined")
      parent.postMessage(JSON.stringify({testbench_child:testbench_target}), "*");
    if (typeof(testbench_verilator) != "undefined")
      parent.postMessage(JSON.stringify({testbench_child:testbench_verilator}), "*");
  }

  if (self == top)
  {
    // we are top window, be ready to process data from children
    window.addEventListener("message", function(event) {
      var edata = JSON.parse(event.data);
      if (typeof(edata.testbench_child) == "undefined")
        return false;
      //console.log("got data:" + edata.testbench_child.title);
      testbenches.push(edata.testbench_child);
      calculateTestbench();
      updateInfo();
      main(testbench);
    });

    var iframe = document.createElement('iframe');
    var url = document.location.href;
    if (url.includes("_target."))
      url = url.replace("_target.", "_verilated.");
    else if (url.includes("_verilated."))
      url = url.replace("_verilated.", "_target.");

    iframe.src = "allinput_verilated.html"
    iframe.style = "display:none"
    document.body.appendChild(iframe);
  }
}

function calculateTestbench()
{
  if (testbenches.length == 0)
    throw "no data";
  testbench = {waveforms:{}, groups:[], labels:[]}//{...testbenches[0]};
  for (var t=0; t<testbenches.length; t++)
  {
//    testbenches[t]._suppress = ["clk"];
    var waveforms = testbenches[t].waveforms;
    var used = [];
    for (var i in testbenches[t].groups)
    {
      var group = testbenches[t].groups[i];
      if (testbench.groups.find(x=>(x.begin == group.begin && x.end == group.end)) == null)
        testbench.groups.push(group);
    }
    for (var i in testbenches[t].labels)
    {
      var label = testbenches[t].labels[i];
//      var olabel = testbench.labels.find(x=>(x.pos == label.pos && x.type == label.type));
      if (testbench.labels.find(x=>(x.pos == label.pos && x.type == label.type)) == null)
        testbench.labels.push(label);
    }
    var suppress = [];
    for (var i in waveforms)
    {
      if (typeof(testbenches[t]._suppress) != "undefined" && testbenches[t]._suppress.indexOf(i) != -1)
        continue;

      if (typeof(testbench.waveforms[i]) == "undefined")
      {
        testbench.waveforms[i] = waveforms[i];
        used.push(i);
      }
      else if (testbench.waveforms[i] != waveforms[i])
      {
        testbench.waveforms[i+""+t] = waveforms[i];
        used.push(i);
      } else {
        suppress.push(i);
      }
    }
    testbenches[t]._used = used;
    if (typeof(testbenches[t]._suppress) == "undefined")
      testbenches[t]._suppress = suppress;
  }
}

function allowDrop(ev) {
  ev.preventDefault();
}

function drag(ev, tbid) {
  ev.dataTransfer.setData("text", JSON.stringify(testbenches[tbid]));
}

function drop(ev) {
  ev.preventDefault();
  var data = JSON.parse(ev.dataTransfer.getData("text"));
  testbenches.push(data);
  calculateTestbench();
  updateInfo();
  main(testbench);
}

function waveClick(tbid, w)
{
  var tb = testbenches[tbid];
  if (typeof(tb._suppress) == "undefined")
    tb._suppress = [w];
  else if (tb._suppress.indexOf(w) == -1)
    tb._suppress.push(w);
  else
    tb._suppress.splice(tb._suppress.indexOf(w), 1);

  calculateTestbench();
  updateInfo();
  main(testbench);
  markerUpdate();
}

function getInfo()
{
  var info = "";
  for (var j in testbenches)
  {
    var tb = testbenches[j];
    info += `
      <div class="infoBlock">
        <div class="infoName">${tb.title}</div>
        <div class="infoGroup">
          FPGA image:<br>
          ${tb.fpga.file}<br>
          ${tb.fpga.timestamp}<br>
        </div>
        <div class="infoGroup">
          Verification code:<br>
          ${tb.verify.file?tb.verify.file:""}<br>
          ${tb.verify.timestamp}<br>
        </div><br>
     `;
    for (var i in tb.waveforms)
    {
      var cls = tb._used.indexOf(i) != -1 ? "infoSignal" : "infoSignalDisabled";
      info += `
        <div class="${cls}" draggable="true" ondragstart="drag(event, ${j})" onclick="waveClick(${j}, '${i}')">
           &#x223F; ${i}
        </div>
    `;
    }
    info += `
      </div>
    `;
  }

  info += `<div class="infoBlock2" ondrop="drop(event)" ondragover="allowDrop(event)">
      <div class="infoGray">Drop here more data...</div></div>`;

  return `<div class="infos"><div class="heading">FPGA<br>Test bench</div>${info}</div>`;
}

function updateInfo()
{
  var infoelement = document.getElementById("info");
  if (!infoelement)
  {
    infoelement = document.createElement('div'); 
    infoelement.id = "info";
    document.body.appendChild(infoelement);
  }
  infoelement.innerHTML = getInfo();
}

window.addEventListener('DOMContentLoaded',function(){
  var sheet = document.createElement('style'); 
  sheet.type="text/css"; 
  sheet.innerHTML = `
  .heading { padding:8px; writing-mode: vertical-rl; font-size:40px; width:80px; height:180px; display:inline-block;} 
  .infos { }
  .infoGroup {padding:4px; margin:4px; width:200px; border: 2px #b0b0b0 solid; border-radius: 6px; display:inline-block;}
  .infoName {padding:8px; font-size:20px;}
  .infoGray {color:#b0b0b0; padding:8px; font-size:20px;}
  .infoBlock {width:450px; border: 2px #b0b0b0 solid; border-radius: 6px; padding:2px; display:inline-block; min-height:180px; vertical-align:top;}
  .infoBlock2 {width:150px; border: 2px #b0b0b0 solid; border-radius: 6px; padding:2px; display:inline-block; min-height:180px; vertical-align:top;}
  .infoSignal {box-shadow: rgba(0, 0, 0, 0.37) 1px 2px 2px, rgba(0, 0, 0, 0.09) 0px -1px 4px; font-weight:bold; margin:4px; padding:4px; min-width:60px; border-radius: 6px; background:#b0b0b0; display:inline-block;}
  .infoSignalDisabled {box-shadow: rgba(0, 0, 0, 0.37) 1px 2px 2px, rgba(0, 0, 0, 0.09) 0px -1px 4px; font-weight:bold; margin:4px; padding:4px; min-width:60px; border-radius: 6px; color:#808080; background:#b0b0b0; display:inline-block;}
  `;
  document.body.appendChild(sheet);
  updateInfo();
  checkChildren();
});
















 window.addEventListener('DOMContentLoaded',function(){
    var sheet = document.createElement('style'); 
    sheet.type="text/css"; 
    sheet.innerHTML = `
#head {font-weight:bold; display: inline-block; width:32px; padding-left:4px; margin-left:10px; border:1px;}
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
#detail {width:50%; margin-top:40px;}
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
<input type="button" value="zoom +" style="position:absolute; right:20px; top:170px; width:80px; height:40px" onClick="zoom(+1)">
<input type="button" value="zoom -" style="position:absolute; right:120px; top:170px; width:80px; height:40px" onClick="zoom(-1)">
</div>
<div id="detail">
</div>
`
//    if (typeof(testbench_target) != "undefined")
//      main(testbench_target);
 //   if (typeof(testbench_verilator) != "undefined")
  //    main(testbench_verilator);
     main(testbench);
     markerUpdate();
  }, false);

  var waveforms;
  var c;
  var ctx;
  var markerTimer = null;
  var markerPos = -1, markerScroll = 0;
  var markerSelectedBegin = -1;
  var markerSelectedEnd = -1;
  var scale = 8;

function zoom(v)
{
  scale += v;
  if (scale < 1)
    scale = 1;
  main(testbench);
  markerUpdate();
}

function main(tb)
{
  var wavelen = 0;
  for(var i in tb.waveforms)
    wavelen = Math.max(wavelen, tb.waveforms[i].length);
 
  var c = document.getElementById("canvasBottom");
  var ctx = c.getContext('2d');
  ctx.fillStyle = "#008";
  ctx.font = "bold 30px Arial";
  c.style.width = wavelen*scale;
  c.width = wavelen*scale;
  c.height = 400;
  ctx.width = wavelen*scale;
  ctx.height = 400;

  var index=0;
  var gindex=0, gactive=0;
  ctx.fillStyle = "#b0b0b0";
  var groups = tb.groups;
  waveforms = tb.waveforms;
  var zfreq = 10;
  if (scale < 4)
    zfreq = 5;
  if (scale < 2)
    zfreq = 3;
  for(var i in waveforms)
  {
    gindex = 0;
    gactive = 0;
    var wave = waveforms[i];
    ctx.beginPath();
    var lasty = -1;
    ctx.moveTo(0, 50*index+60-0);
    for (var j=0; j<wave.length*scale; j++)
    {
      var i = Math.floor(j / scale);
      var x = j;
      if (groups.length > gindex)
      {
        if (!gactive && j/scale >= groups[gindex].begin)
        {
            gactive = 1; //mGroups[gindex].color;
  //          ctx.fillStyle = groups[gindex].color;
            ctx.lineTo(x, 50*index+60-0);
            ctx.closePath();
            ctx.fill();
            ctx.beginPath();
            ctx.moveTo(x, 50*index+60-0);
        }
        if (gactive && j/scale >= groups[gindex].end)
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

      var sample = wave[i];
      var level = 0.5;
      switch (sample)
      {
        case '0': level = 0; break;
        case '1': level = 1; break;
        case 'x': level = j&1; break;
        case 'z': level = 0.5 + Math.cos(j/zfreq*Math.PI)*0.1; break;
      }
      var y = 50*index+60-level*40;

        if (y != lasty)
          ctx.lineTo(x, lasty);
        ctx.lineTo(x, y);
      lasty = y;
    }
    ctx.lineTo(wave.length*scale, 50*index+60-0);
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
    for (var j=0; j<wave.length*scale; j++)
    {
      var i = Math.floor(j / scale);
      var x = j;
      var sample = wave[i];
      var level = 0.5;
      switch (sample)
      {
        case '0': level = 1/40; break;
        case '1': level = 1; break;
        case 'x': level = j&1; break;
        case 'z': level = 0.5 + Math.cos(j/zfreq*Math.PI)*0.1; break;
      }
      var y = 50*index+60-level*40;

        if (y != lasty)
          ctx.lineTo(x, lasty);
        ctx.lineTo(x, y);
      lasty = y;
    }
    ctx.lineTo(wave.length*scale, 50*index+60-0);
  //  ctx.endPath();
    ctx.stroke();
    index++;
  }


  var lineMax = [0, 0, 0];
  for (var i in tb.labels)
  {
    var label = tb.labels[i];
  //console.log(label);
    var x = label.pos*scale;
    var y = 330+40-20-8;
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
            markerSelectBegin(tb);
        });
        canvasElem.addEventListener("mouseup", function(e)
        {                   
            markerSelectEnd(tb);
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
  if (markerPos < 0)
    return;
 //   markerPos = 0;

  var x = Math.floor(markerPos/scale+0.5)*scale;

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
    ctx.fillStyle = "#ff0000";
  }
	  var index=0;
  for(var i in waveforms)
  {
    var wave = waveforms[i];
    ctx.moveTo(0, 50*index+60-0);

      var sample = wave[Math.floor(x/scale)];
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
  var index = Math.floor(markerPos/scale+0.5);
  markerSelectedEnd = -1;
  markerSelectedBegin = index*scale;
  markerUpdate();
}

function getGroup(tb, p)
{
  for (var i=0; i<tb.groups.length; i++)
  {
    if (tb.groups[i].begin <= p && p < tb.groups[i].end)
      return tb.groups[i];
  }
  return null;
}

function getGroupLabelBegin(tb, g)
{
  return tb.labels.find(x=>x.type == 2 && x.pos == g.begin)?.text
}

function getGroupLabelEnd(tb, g)
{
  return tb.labels.find(x=>x.type == 3 && x.pos == g.end)?.text
}

function getGroupLabel(tb, g)
{
  return getGroupLabelBegin(tb, g) + " / " + getGroupLabelEnd(tb, g);
}

function markerSelectEnd(tb)
{
//  markerSelected = markerPos;
  var index = Math.floor(markerPos/scale+0.5);
  markerSelectedEnd = index*scale;
  markerUpdate();
  var detail = "";
  for (var i in waveforms)
    detail += `<div id='head'>${i}</div>`;

  var curGroup = null; //getGroup(markerSelectedBegin/8);
    //gabo
  var prevlevels = "00000000";
  for (var i=markerSelectedBegin/scale; i<markerSelectedEnd/scale; i++)
  {
    if (curGroup && curGroup.end == i)
    {
      detail += "</div></div>";
      curGroup = null;
    }
    if (!curGroup && (curGroup = getGroup(tb, i)) != null)
    {
      var msg = getGroupLabel(tb, curGroup);
      detail += (`<div class='group'><input type='button' value='Collapse: ${msg}' onClick='_.collapse(this);'><br><div class='group_contents'>`); 
    }
    if (i==index)
      detail += "<b>";

    var detailline = "";
    var levels = "";
    var ind = 0;
    for (var j in waveforms)
    {
      var l0 = prevlevels[ind++];//waveforms[j][i-1];
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
    
    var labels = tb.labels.filter(x=>x.pos == i);
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
            markerUpdate();
            markerTimer = setTimeout(() => {markerTimer = null}, 50);
        }
      
