
// DMA  -> L:1000, H:0, L:1000, H:0, L:300, H:1000, L:0, H:1000, L:0, H:100, L:500, H:500, L:500, H:500
// REAL -> L:2300, H:2100, L:500, H:500, L:500, H:500

// DMA pushes time intervals into ring buffer. If this interval is longer than specific time (e.g. 1000 pulses)
// DMA interrupt routine will push [1000, 0] into the ring buffer. Purpose for doing this is to quicky identify
// that the signal transmission is finished

// this code extracts real time intervals from ring buffer values

join([
  1000, 0, 1000, 0, 300, // leading 1300
  1000, 0, 1000, 0, 100, 500, 500, 500, 500,  // data
  1000, 0, 1000, 0, 1000, 0, 1000, 0, 1000, 0, 1000, 0, 300, // leading 6300
  1000, 0, 1000, 0, 100, 500, 500, 500, 500, // data
  1000, 0, 1000, 0, 1000, 0, 1000, 0, // trailing 4000
], console.log);

function join(arr, h)
{
  var interval1 = 0;
  var interval2 = 0;
  var result = [];
  // 0, 0, [1000] => 0, 1000
  // 0, 1000, [0] => 1000, 0

  // 1000, 0, [1000] => 0, 2000
  //        0, 2000, [0] => 2000, 0
  //           2000, 0, [500] => 0, !!!2500
  //           0, 2500, [1000] => 2500, 1000
  //           2500, 1000, [0] => 2500!!!  1000, 0
  //           1000, 0, [300] => 0, 1300
  //           0, 1300, 1000 => 1300, 1000
  for (var i=0; i<arr.length; i++) 
  {
    var interval = arr[i];
    if (interval1 == 0)
    {
      interval += interval2;
      if (interval >= 5000)
      {
        if (result.length > 0)
        {
          result.shift(); // ignore leading zero pulse
          h(result);
        }
        result = [];
      }
    }

    if (interval1 != 0 && interval2 != 0)
      result.push(interval2);

    interval2 = interval1;
    interval1 = interval;
  }
  result.shift(); // ignore leading zero pulse
  h(result);
}