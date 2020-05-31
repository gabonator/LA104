var simulator = false;

document.querySelector("#simulator").addEventListener('click', function() {
  var simTick = 0;
  var level = 0;
  if (simulator) return;
  simulator = true;
  started = true;

  setInterval(() => {
    var phase = simTick++ % (5*5);
    if (!started)
      return;

    var signal = [];
    if (phase == 20) 
    {
        signal.push(1000);
        signal.push(9000);
        signal.push(1000);
        signal.push(9000);
        signal.push(5000);
        signal.push(5000); // 30ms
      signal = signal.concat([0, 10*1000, 0, 40*1000, 0, 40*1000, 0, 40*1000, 0, 40*1000]);
    }
    else if (phase == 3) 
    {
        signal.push(20000);
        signal.push(20000);
        signal.push(20000);
        signal.push(20000);
        signal.push(20000);
        signal.push(20000);
        signal.push(20000);
        signal.push(20000);
        signal.push(20000);
        signal.push(20000);
    }
    else if (phase == 5) 
    {
        signal.push(40000);
        signal.push(40000);
        signal.push(40000);
        signal.push(40000);
        signal.push(0);
        signal.push(40000);
    }
    else if (phase == 6) 
    {
        signal.push(25000);
        signal.push(3*20);
        signal.push(15000);
        signal.push(30000);
        signal.push(7*20);
        signal.push(10000);
        signal.push(30000);
        signal.push(5*20);
        signal.push(10000);
        signal.push(40000);
        signal.push(0);
        signal.push(40000-(3+7+5)*20);
    }
    else if (phase == 10)
    {
//      signal = [0, 40*1000, 0, 10*10000];
      for (var i = 0; i<100*1000; i += 10000)
      {
        signal.push(5000);
        signal.push(5000);
      }

      for (var i = 0; i<100*1000; i += 2000)
      {
        var d = Math.floor(Math.random()*10);
        signal.push(1000+d*20);
        signal.push(1000-d*20);
      }
    } 
    else if (phase == 11)
    {
      for (var i = 0; i<50*1000; i += 500)
      {
        var d = Math.floor(Math.random()*5);
        signal.push(250+d*20);
        signal.push(250-d*20);
      }
      signal = signal.concat([0, 40*1000, 0, 40*1000, 0, 40*1000, 0, 30*1000]);
    } else
      signal = [0, 40*1000, 0, 40*1000, 0, 40*1000, 0, 40*1000, 0, 40*1000];

   var sum = signal.reduce((a,b) => a+b, 0);
   if (sum != 200000)
     throw "wrong len " + phase;
   var data = signal;

   memory.push(data);
              for (var i=0; i<data.length; i++)
              {
                canvas.drawPulse(data[i], level);
                pulseMachine(data[i], level = 1-level);
              }
              canvas.drawPulseFinish();

  }, 200);
});

