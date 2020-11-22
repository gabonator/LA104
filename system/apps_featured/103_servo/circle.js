var points = [];
var knob_width = 24;
var knob_height = 24;

function plot(x, y, a)
{
//  x += knob_width/2-1;
//  y += knob_height/2;
//  if (a>=8)
    points.push({x:x, y:y, a:a});
}

function plot_4_points (x, y, f)
{
    var opac = Math.floor(255*f);
    x = Math.floor(x);
    y = Math.floor(y);

    if (x>0 && y>0)
    {
        plot ( x, +y, opac);
//        plot ( x, -y, opac);
//        plot (-x, +y, opac);
//        plot (-x, -y, opac);
        return;
    }

    if (x==0)
    {
        plot ( x, +y, opac);
//        plot ( x, -y, opac);
        return;
    }
    if (y==0)
    {
        plot ( x, +y, opac);
//        plot (-x, +y, opac);
    }
}


function wu_circle(r)
{
    var width = knob_width;
    var height = knob_height;
    var k = 1;
    
    var rsq = r*r;
    var ffd = r / Math.sqrt(2.0);  // forty-five-degree coord

    for (var xi=0; xi<=ffd+1; xi++)
    {
      var yj = Math.sqrt(rsq-xi*xi);
      var flr = Math.floor(yj);
      var frc = yj - flr;
      if (frc < 0.5)
      {
        plot_4_points (xi*k, (yj)*k, 1-frc);
        plot_4_points (xi*k, (yj+1)*k, frc);
      } else
      {
        plot_4_points (xi*k, (yj+1)*k, frc);
        plot_4_points (xi*k, (yj)*k, 1-frc);
      }
    }

    for (var yi=Math.floor(ffd); yi>0; yi--)
    {
      var xj = Math.sqrt(rsq-yi*yi);
      var flr = Math.floor(xj);
      var frc = xj - flr;
      if (frc < 0.5)
      {
        plot_4_points (xj*k, yi*k, 1-frc);
        plot_4_points ((xj+1)*k, yi*k, frc);
      } else {
        plot_4_points ((xj+1)*k, yi*k, frc);
        plot_4_points (xj*k, yi*k, 1-frc);
      }
    }
}

wu_circle(7);

function angle(p)
{
  var angle = Math.atan2(p.y, p.x)/Math.PI*180;
  if (angle < 0)
    angle += Math.PI*2;
  return angle;
}

//points.sort( (a, b) => angle(a) - angle(b) )

//console.log(points);
var all = [];
for (var i in points)
{
  all.push(points[i].x);
  all.push(points[i].y);
  all.push(points[i].a);
}

var full = [];
for (var i=0; i<20*4; i++)
{
  var pt = circlepoint(all, i);
  full.push(pt.x)
  full.push(pt.y)
  full.push(pt.a)
}

console.log(exportBuffer(full));
            


function circlepoint(circle_data, i)
{
    var N = all.length/3;
    i=N*7-i;
    i%=N*4;
    if (i<N)
    {
        x = circle_data[i*3+0];
        y = circle_data[i*3+1];
        a = circle_data[i*3+2];
    }
    else if (i<N*2)
    {
        i = i-N;

        x = circle_data[i*3+1];
        y = -circle_data[i*3+0];
        a = circle_data[i*3+2];
    }
    else if (i<N*3)
    {
        i = i-2*N;

        x = -circle_data[i*3+0];
        y = -circle_data[i*3+1];
        a = circle_data[i*3+2];
    }
    else if (i<N*4)
    {
        i = i-3*N;

        x = -circle_data[i*3+1];
        y = circle_data[i*3+0];
        a = circle_data[i*3+2];
    }
    
//    x = x + knob_width/2-1;
//    y = y + knob_height/2;
    return {x:x, y:y, a:a}
}


function exportBuffer(buf)
{
  var id = "circle";
  var aux = ["static const uint8_t "+id+"_data["+buf.length/3+"*3] = {"];
  var ofs = 0;

  while (ofs<buf.length)
  {   
    var count = Math.min(buf.length - ofs, 16);
    var line = "  ";
    for (var i=0; i<count; i++)
    {
      var v = buf[i+ofs];
      line += v + ", "; //"0x" + ("000"+v.toString(16)).substr(-2) + ", ";
    }
    aux.push(line);
    ofs += count;
  }
  aux.push("};");
  return aux.join("\n");
}