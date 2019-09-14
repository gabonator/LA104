var header = null, data = null;
var _head = (json) => header = json;
var _data = (json) => data = json;

var div = document.createElement("div");
div.id = "chart";
div.style = "width:100%; height:300px;";
document.documentElement.appendChild(div);

var script = document.createElement('script');
script.src = "https://code.highcharts.com/highcharts.js";
script.onload = () => main(header, data);
document.documentElement.appendChild(script);

function main(header, data)
{
  var maxSensors = 0;
  for (var i in data)
    maxSensors = Math.max(maxSensors, data[i].length-1);

  var series = [];
  var palette = ["#ff0000", "#00ff00", "#0000ff", "#ffff00", "#00ffff", "#ff00ff", "#000000", "#ffffff"];

  for (var i=0; i<maxSensors; i++)
  {
    var seriesdata = [];
    for (var j in data)
      seriesdata.push([data[j][0], data[j][i+1]]);
    
    series.push({name:"Sensor " + i, data:seriesdata, showInLegend:false, color:palette[i]});
  }

  var chart = new Highcharts.Chart({
    chart: {
        renderTo: 'chart',
    },
    plotOptions: { series: { animation: false } },
    title: { text: header.device + " Temperature measurement '" + header.file + "'" },
    yAxis: {
        gridLineColor: 'rgba(0, 0, 0, 0.1)',
        title: { text: null},
    },
    tooltip: {
		        crosshairs: true,
		        shared: true,
    headerFormat: '<span style="font-size: 10px">{point.key}s</span><br/>',
    pointFormatter: function() {
      return '<span style="color:' + this.color + '">\u25CF</span> ' + this.series.name + ': <b>' + this.y.toFixed(1) + " " + String.fromCharCode(176) + "C" + '</b><br/>';
    }
		    },    
    series: series
  });
};
