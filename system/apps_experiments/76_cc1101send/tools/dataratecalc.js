// CC1101 - calculation of data rate (MDMCFG4.DRATE_E, MDMCFG3.DRATE_M)
var fOsc = 26e6;

var maxError = 0;
var sumError = 0;
var cntError = 0;


function Calc(rate)
{
  var base = (1<<28) / fOsc;
  var E = Math.ceil(Math.log2(rate * base / (256 + 255)));
  var M = Math.floor(rate * base / (1<<E) - 256 + 0.5);

  //Average error: 0.07% Max error: 0.19%
  return {E:E, M:M};
}

function CalcFix(rate)
{
  // var t = rate * (1<<28) / fOsc;
  // maximal common divisor of 26 000 000 and 268435456 is 128
  // max rate value is 2048 to fit uint32_t
  //var t = rate * (1<<21) / Math.floor(fOsc/128);
  var t = rate * (1 << 28) / fOsc;

  var Ebase = Math.floor(t/511);

  var E = 0, Ev = 1;
  while (Ebase > 0)
  {
    Ebase >>= 1;
    Ev <<= 1;
    E++;
  }

  var M = Math.floor((t + Ev/2) / Ev - 256);

  var check = Calc(rate);
  if (E != check.E || M != check.M)
    throw "err";

  return {E:E, M:M};
}

for (var i=100; i<10000; i+=1)
{
  var rate = i;

  var base = (1<<28) / fOsc;
  var c = CalcFix(i);

  var check = (256+c.M) * (1<<c.E) / base;

  var error = Math.abs(rate-check)/rate;
  maxError = Math.max(error, maxError);
  sumError += error;
  cntError++;

  console.log({rate:rate, check:check.toFixed(1), error:(error*100).toFixed(1) + "%", E:c.E, M:c.M});
  if (Math.ceil(c.E) > 15)
    throw "Exponent too big";
  if (Math.floor(c.M) > 255)
    throw "Mantisa too big";
}

console.log("Average error: " + ((sumError/cntError)*100).toFixed(2) + "%");
console.log("Max error: " + (maxError*100).toFixed(2) + "%");

/*
        rate = (256+M)* (2^E) / 2^28 * fOsc

        rate * 2^28 / 26e6 

        rate * 10.324 = (256+M)*(2^E)

        M = rate * 10.324 / (2^E) - 256
        looking for biggest possible M, but less than 256

        M = 1000 * 10.324 / (2^E) - 256

        M = 10324 / (2^E) - 256

        255 = 10324 / (2^E) - 256
        255+256 = 10324 / (2^E)
        (2^E) = 10324/(255+256)

        2^E = 20
        E = 4 / 5?

        E = 4:
        M = rate * 10.324 / (2^4) - 256 = 10324 / 16 - 256 = 389.25

        E = 5:
        M = rate * 10.324 / (2^5) - 256 = 10324 / 32 - 256 = 66.625 ...OK

        E = 5, M = 66 -> rate = (256+M)* (2^E) / 2^28 * fOsc = (256+66)* (2^5) / 2^28 * 26e6 = 998.02
        E = 5, M = 67 -> rate = (256+M)* (2^E) / 2^28 * fOsc = (256+67)* (2^5) / 2^28 * 26e6 = 1001.11
*/