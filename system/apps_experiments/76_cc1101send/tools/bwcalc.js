function GetOscFrequency()
{
  return 26e6;
}

//int CHANBW_M = (mRegisters[MDMCFG4] >> 4) & 3;  // 0..3
//int CHANBW_E = mRegisters[MDMCFG4] >> 6; // 0..3

function GetBandwidth(CHANBW_M, CHANBW_E)
{
  return (GetOscFrequency() / 8 / (4 + CHANBW_M)) >> CHANBW_E;
}

function CalcBandwidth(bw)
{
  var _e = Math.ceil(-Math.log2(GetBandwidth(m, e) / GetOscFrequency() * 8 * (4+8)));
  _e = Math.max(0, _e);
  var _m = Math.floor(GetOscFrequency() / 8 / (1<<_e)/bw - 4);

  return {e:_e, m:_m};
}

function CalcBandwidthFix(bw)
{
  var root2 = [0, 1, 2, 2, 3, 3, 3, 3];
  var t = GetOscFrequency() / 8 / bw;
  var _e = root2[Math.floor(t/12)];
//  var _m = Math.floor(t / (1<<_e) - 4);
  var _m = (Math.floor(t) >> _e) - 4;
  return {e:_e, m:_m};
}

for (var e=0; e<=3; e++)
  for (var m=0; m<=3; m++)
  {
    var bw = GetBandwidth(m, e);
    var _calc = CalcBandwidthFix(bw);
    var _bw = GetBandwidth(_calc.m, _calc.e);

    console.log({e:e, m:m, bw:bw, _e:_calc.e, _m:_calc.m, _bw:_bw, _err:bw-_bw});
  }

/*
  bw = osc / 8 / (4 + m) / (2^e)

  minimize (2^e) to make largest possible mantissa (m=7)

  e = log2(bw / osc * 8 * (4+7))

  bw * (4+m) = osc/8/(2^e)
  m = (osc/8/(2^e) - 4*bw) / bw

*/