
let startButton = document.querySelector("#start");
startButton.addEventListener('click', function() {
  MODEM.Init().then( i => {
    if (!i) 
    {
      console.log("Modem init failed");
      return;
    }

    var level = 1;

    MODEM.Start()
//      .then( () => MODEM.SetFrequency(434424000)) // keyfob
//      .then( () => MODEM.SetFrequency(433856000)) // conrad, OS/rain
//      .then( () => MODEM.SetFrequency(433942000)) // conrad, OS/temp
      .then( () => MODEM.SetFrequency(433900000)) // conrad, OS/temp
      .then( () =>
      {
        setInterval(() =>
        {
          MODEM.Transfer().then( (data) => 
          {
            if (data && data.length) 
            {
              for (var i=0; i<data.length; i++)
                pulseMachine(data[i], level = 1-level);
    //          console.log(data);
            }
          });
        }, 200);

        console.log("Modem running");
        document.write("<pre>Modem running\n");
      });
  });
});


var decoder = new Decoder();
document.write("<pre>started\n");
var buf = [];
var last;
function pulseMachinePush(i)
{
  if (i==-1)
  {
    if (buf.length > 20 && buf.length < 1000)
    {
      console.log("l="+buf.length + " d=" + buf.reduce((a, b) => a+b, 0) + " " + JSON.stringify(buf));
//last = buf;
//      if (buf.length > 100)

if (buf[0] >= 20 && buf[0] <= 27 && buf[1] >= 20 && buf[1] <= 27 && buf[2] >= 20 && buf[2] <= 27)
{
  var pref = "22222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222";
  var key = "2222222222222222222222222222222222222222222222222222222222222" + "22222423333332224";
  var kf = buf.map(x => Math.floor((x+6)/12));
//console.log(buf.map(x => Math.floor((x+2)/12*10)).join(" "));

  var kfs = kf.join("");
  if (kfs.indexOf(key) != -1)
  {
    var end = kfs.indexOf(key) + key.length;
    var ps = kfs.substr(end);
    console.log(ps);
    var ps1 = "";
    var lev = 0;;
    // 2222222222222222444444224224224444222222224224422224224222222444422224224422444222222222242222444444222222222242222
/*
    for (var i=0; i<ps.length; i++)
    {
      if (ps[i] == "2")
      {
        ps1 += lev;
        lev = 1 - lev;
      } else
      if (ps[i] == "4")
      {
        ps1 += lev;
        ps1 += lev;
        lev = 1 - lev;
      } else
      {
        ps1 += "ERROR!";
        break;
      }
    }
// 010101010101001101010101001100101101001011010010110011001011010100110010110100110010110011010010101010101010101010110101001011010101010011010101001
//    console.log(ps1);
    var ps2 = "";
    for (var i=0; i<ps1.length; i+=2)
    {
      if (ps1.substr(i, 2) == "01")
        ps2 += "A";
      else if (ps1.substr(i, 2) == "10")
        ps2 += "B";
      else
        ps2 += "?";
    }
*/

    var ps1 = "";
    for (var i=0; i<ps.length; i++)
    {
      if (ps[i] == "2" && ps[i+1] == "2")
      {
        i++;
        ps1 += "0";
      } else
      if (ps[i] == "4")
      {
        ps1 += "1";
      } else
      {
        ps1 += "ERROR!";
        break;
      }

    }

    kfs = "KEY PREAMBLE " + ps1;
  }
  else if (kfs.substr(0, pref.length) == pref) 
    kfs = "STATIC PREAMBLE " + kfs.substr(pref.length);

  console.log(kf.reduce((a, b) => a+b, 0) + " -> " + kfs);

// 2222222222222423442222224422442244422444222242222
// 222222222222244422442222422224442222422224224222242242242242222444224422224222244442242244422422422422224442244444

// 22222222222224233333322242222222222222222444444224224224444222222224224422224224222222444422224224422444222222222242222444444222222222242222
// 22222222222224233333322242222222222224442244224442
//  2222222222224233333322242222222222224422444422224222222222222224422224222222422224444442222422224222242242222422444444444224422224222242222
// 222222222222242333333222422222222222222224222222442244224224224224442222222222442244224422222244222222422444224422444224224444444222242222
// 222222222222242333333222422222222222242242222442242242222224442222224222222224222222224222242222224224444444444222222442222224422422224222242222
// 22222222222224233333322242222222222222244222222224422444444224444224222222444224442222442
//  2222222222224233333322242222222222222222224422422442222442242242222222242222222242222422422422422444224422422222242222
// 22222222222224233333322242222222222224224222242222442242244442244222222444442222224222222422442244222242222422222222224222222422222222224

// 2222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222224233333322242222222222224442244222222222242244442244444222222422222222224422422444442242242244222244222242222222244224222242222
// 22222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222242333333222422222222224224222244222244224444224222244222242222422222244444222242222442244222242242244224444222244224222242222
// 22222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222244444222222222244444224222222422222222224222222422224422222244422224222242222422222222224442244222242222
}
      var good = false;
        k = 19;
        l = 0;
      for (var k=14; k<60; k++)
      {
//        for (l=-6; l<=6; l++)
        {
          var d = decoder.decode(buf.map((x, i) => (x + ((i&1)*2-1)*l) * k ));
//          var d = decoder.decode(buf.map(x => x*k ));
          if (d.length)
          {
            document.write(k + "," + l + ": " + JSON.stringify(d)+"\n");
//            console.log(JSON.stringify(buf));
            console.log(d); 
            good = true;
		break;
          }
        }
//        if (good) break;
      }

    }
    buf = [];
    return;
  }
  buf.push(i);
}

var interval1 = 0, interval2 = 0, leading = true, terminated = false;
function pulseMachine(interval, level)
{
  if (interval == -1)
  {
    interval1 = 0;
    interval2 = 0;
    leading = true;
    pulseMachinePush(-1);
    return;
  }

  if (interval1 == 0)
  {
    interval += interval2;
  }
  
  if (interval > 800)
  {
    pulseMachinePush(-1);
    leading = true;     
  }

  if (interval1 != 0 && interval2 != 0)
  {
    if (interval2 > 800)
    {
      pulseMachinePush(-1);
      leading = true;     
    }

    if (leading)
    {
      if (level == 0)   
        leading = false;
    }
    else
    {
      pulseMachinePush(interval2);
    }
  }

  interval2 = interval1;
  interval1 = interval;
}

/*
function pulseMachine(interval)
{
	if (interval == -1)
	{
		interval1 = 0;
		interval2 = 0;
		leading = true;
		pulseMachinePush(-1);
		return;
	}
  if (interval1 == 0)
  {
    interval += interval2;
  }

  if (interval > 800)
  {
// ked predosly nie je nula, tak nejoinoval, takze by mohol byt v bufri nejaky uzitocny?
	  if (!terminated)
	  {
		  terminated = true;
if (interval1 != 0)
  pulseMachinePush(interval1);
			pulseMachinePush(-1);

	  }
		interval1 = 0;
		interval2 = 0;
    leading = true;
    return;
  }

  if (interval1 != 0 && interval2 != 0)
  {
    if (leading)
      leading = false;
    else
	{
      pulseMachinePush(interval2);
		terminated = false;
	}
  }

  interval2 = interval1;
  interval1 = interval;
}
*/
