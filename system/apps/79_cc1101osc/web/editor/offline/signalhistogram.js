function SignalHistogram(signal)
{
  if (!signal || signal.data.length == 0)
  {
    document.querySelector("#chart").style.display = "none";
    return;
  } else {
    document.querySelector("#chart").style.display = "block";
  }
  
  var data1 = [];
  var data2 = [];
  for (var i=0; i<signal.data.length; i++)
  {
    if (signal.level[i] == "h")
      data1.push(signal.data[i]);
    else
      data2.push(signal.data[i]);
  }

  Highcharts.chart('chart', {
    plotOptions: {series: {animation: false} },
    title: { text: null },
    xAxis: [{ title: { text: 'Data' }, alignTicks: false }, { title: { text: 'Histogram' }, alignTicks: false, opposite: true }],
    yAxis: [{ title: { text: 'Data' } }, { title: { text: 'Histogram' }, opposite: true }],
    series: [
    { name: 'Histogram', type: 'histogram', xAxis: 1, yAxis: 1, baseSeries: "s1", zIndex: -1, color: "rgba(255, 0, 0, 0.5)", binsNumber:20 },
    { name: 'Histogram', type: 'histogram', xAxis: 1, yAxis: 1, baseSeries: "s2", zIndex: -1, color: "rgba(0, 0, 255, 0.5)", binsNumber:20 },
    { name: 'Data', type: 'scatter', data: data1, id: 's1', marker: { radius: 1.5 }, visible:false },
    { name: 'Data', type: 'scatter', data: data2, id: 's2', marker: { radius: 1.5 }, visible:false }]
  });

  (function(H) {
   H.Pointer.prototype.drag = function(e) {
     var container = this.chart.container.parentElement;
     container.style.left = container.offsetLeft + e.movementX + 'px';
     container.style.top = container.offsetTop + e.movementY + 'px';
   }
  })(Highcharts);
}

