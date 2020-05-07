
let startButton = document.querySelector("#start");
startButton.addEventListener('click', function() {
  MODEM.Init().then( i => {
    if (!i) 
    {
      console.log("Modem init failed");
      return;
    }

    MODEM.Start();
    setInterval(() =>
    {
      MODEM.Transfer().then( (data) => 
      {
        if (data && data.length) 
        {
          for (var i=0; i<data.length; i++)
            pulseMachine(data[i]);
//          console.log(data);
        }
      });
    }, 200);

    console.log("Modem running");
    document.write("Modem running");
  })
});


var decoder = new Decoder();
document.write("<pre>started");
var buf = [];
function pulseMachinePush(i)
{
  if (i==-1)
  {
    if (buf.length > 20 && buf.length < 1000)
    {
      console.log(buf);
//      if (buf.length > 100)

for (var k=20; k<50; k+=2)
{
  var d = decoder.decode(buf.map(x => x * k));
  if (d.length)
  {
    document.write(k + ": " + JSON.stringify(d)+"\n");
    console.log(JSON.stringify(buf));
    console.log(d);
    break;
  }
}
    }
    buf = [];
    return;
  }
  buf.push(i);
}

var interval1 = 0, interval2 = 0, leading = true, terminated = false;
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

