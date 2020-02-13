# RTL433 decoder WASM build

Slightliy modified [RTL433 library](https://github.com/merbanan/rtl_433) by Benjamin Larsson (and contributors) so it can be compiled using emscripten and used in javascript to decode OOK modulated signals.

test [code](../../system/apps_featured/69_webusbosc/web/v2/externsions/rtl433/index.html):
```javascript
// on-off intervals in ms
var sequence = [520, 500, 420, 520, 400, 520, 440, 520, 400, 520, 400, 540, 400, 520, 440, 
  500, 440, 480, 440, 500, 440, 500, 460, 440, 460, 500, 420, 500, 440, 520, 420, 520, 420, 
  500, 460, 460, 480, 440, 480, 440, 500, 460, 480, 460, 460, 460, 480, 920, 980, 940, 960, 
  460, 500, 440, 500, 440, 480, 460, 480, 920, 500, 460, 500, 420, 980, 920, 980, 940, 480, 
  440, 500, 460, 460, 460, 960, 940, 980, 920, 500, 460, 480, 440, 480, 440, 960, 460, 520, 
  920, 960, 480, 480, 420, 500, 460, 480, 440, 500, 920, 480, 440, 500, 440, 500, 420, 500, 
  440, 500, 440, 980, 960, 440, 460, 480, 460, 500, 420, 520, 440, 480, 460, 480, 420, 500, 
  460, 480, 440, 500, 440, 500, 440, 480, 440, 480, 460, 980, 440, 480, 960, 960, 440, 500, 
  460, 460, 960, 460, 460, 500, 420, 980, 440, 500, 440, 500, 920, 960, 960, 960, 940, 980, 
  920, 960, 460, 500, 940, 960, 960, 460, 460, 960, 460, 480];

console.log(decode(sequence));
```

returns:
```javascript
[
 {
  "protocol" : 11, 
  "description" : "Oregon Scientific Weather Sensor", 
  "brand" : "OS", 
  "model" : "THGR810", 
  "id" : 246, 
  "channel" : 1, 
  "battery" : "OK", 
  "temperature_C" : 0.800, 
  "humidity" : 76
 }
]
```

used as external signal analyser/decoder in WebUsbOscilloscope for DS203 [69_webusbosc](../../system/apps_featured/69_webusbosc/web/v2)
