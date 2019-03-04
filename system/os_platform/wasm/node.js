var Module = require("./app.js")
Module['onRuntimeInitialized'] = () =>
{
  var data = [4760, 4600, 640, 1660, 640, 1660, 640, 1660, 620, 500, 640, 480, 640, 500, 640, 480, 640, 500, 
  620, 1660, 640, 1660, 640, 1660, 640, 480, 640, 500, 640, 480, 640, 480, 640, 500, 640, 1640, 660, 1660, 640, 
  1640, 640, 500, 640, 480, 640, 500, 620, 500, 640, 480, 640, 480, 660, 480, 640, 480, 640, 1660, 640, 1660, 
  640, 1660, 640, 1660, 640, 1660, 640];

  var ptr = Module._appGetDataPtr();
  for (var i=0; i<data.length; i++) 
    Module.HEAP16[ptr/2+i] = data[i];

  Module._appSetDataCount(data.length);
  Module._appAnalyse();
  console.log(Module.AsciiToString(Module._appAnalyseResultPtr()));
}
