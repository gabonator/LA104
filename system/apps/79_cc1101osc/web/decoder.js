class Wasm
{
  readAsync(url) 
  {
    return new Promise( (resolve, reject) =>
    {
      var xhr = new XMLHttpRequest();
      xhr.open('GET', url, true);
      xhr.responseType = 'arraybuffer';
      xhr.onload = function xhr_onload() {
        if (xhr.status == 200 || (xhr.status == 0 && xhr.response)) { // file URLs can return 0
          resolve(xhr.response);
          return;
        }
        reject();
      };
      xhr.onerror = onerror;
      xhr.send(null);
    });
  }

  load(binary, imports) 
  {
    var GLOBAL_BASE = 1024,
    TOTAL_STACK = 4096,
    TOTAL_MEMORY = 131072,
    STATIC_BASE = 1024,
    STACK_BASE = 65072,
    STACKTOP = STACK_BASE,
    STACK_MAX = 69168
    , DYNAMICTOP_PTR = 64816
    ;

    var abort = (what) => { 
      throw 'abort(' + what + '). Build with -s ASSERTIONS=1 for more info.'
    };

    var env = {
      "abort": abort, "_abort": abort, "___assert_fail": abort, "___setErrNo": abort,
      "___syscall140": abort, "___syscall146": abort, "___syscall54": abort, "___syscall6": abort,
      "_emscripten_memcpy_big": abort, "_emscripten_resize_heap": abort, "abortOnCannotGrowMemory": abort, 
      "_emscripten_get_heap_size": () => TOTAL_MEMORY,
      "DYNAMICTOP_PTR": DYNAMICTOP_PTR,
      memory:new WebAssembly.Memory({ 'initial': TOTAL_MEMORY >> 16, 'maximum': TOTAL_MEMORY >> 16 }),
      table:new WebAssembly.Table({'initial': 160, 'maximum': 160, 'element': 'anyfunc'}),
      '__memory_base': STATIC_BASE, 
      '__table_base': 0
    };

    return WebAssembly.instantiate(new Uint8Array(binary), {env:env})
    .then( output => 
     {
      var exports = output.instance.exports;
      var Module = {};  
      Module["memory"] = new Uint8Array(env.memory.buffer);
      Module["imports"] = {};

      for (var i in imports)
        Module.imports[i] = exports[i];
      return Module;
    });
  };
}

class Decoder
{
  constructor()
  { 
    this.wasm = new Wasm;
    // rtl433.wasm image is built by LA104/tools/rtl433/buildwasm.sh
    this.wasm.readAsync("rtl433.wasm")
      .then( binary => this.wasm.load(binary, {_convert:null, _buffer:null}))
      .then( (function(module)
      {
        this.buffer = module.imports._buffer;
        this.convert = module.imports._convert;
        this.memory = module.memory;
        this.getString = (ptr) => 
          {
            var text = "";
            for (var i=0; i<1000 && this.memory[ptr+i] != 0; i++)
              text += String.fromCharCode(this.memory[ptr+i]);
            return text;
          };
        this.setDword = (ptr, dw) => 
          {
            this.memory[ptr+0] = dw & 0xff;
            this.memory[ptr+1] = (dw >> 8) & 0xff;
            this.memory[ptr+2] = (dw >> 16) & 0xff;
            this.memory[ptr+3] = (dw >> 24) & 0xff;
          }

      }).bind(this));
  }

  decode(sequence)
  {
    var ptr = this.buffer();
    for (var i=0; i<sequence.length; i++)
      this.setDword(ptr+i*4, sequence[i]);

    var result = this.convert(ptr, sequence.length);
    var str = this.getString(result);
    return JSON.parse(str);
  }

  test()
  {
    var sequence = [520, 500, 420, 520, 400, 520, 440, 520, 400, 520, 400, 540, 400, 520, 440, 500, 440, 480, 440, 500, 440, 500, 460, 440, 460, 500, 420, 500, 440, 520, 420, 520, 420, 500, 460, 460, 480, 440, 480, 440, 500, 460, 480, 460, 460, 460, 480, 920, 980, 940, 960, 460, 500, 440, 500, 440, 480, 460, 480, 920, 500, 460, 500, 420, 980, 920, 980, 940, 480, 440, 500, 460, 460, 460, 960, 940, 980, 920, 500, 460, 480, 440, 480, 440, 960, 460, 520, 920, 960, 480, 480, 420, 500, 460, 480, 440, 500, 920, 480, 440, 500, 440, 500, 420, 500, 440, 500, 440, 980, 960, 440, 460, 480, 460, 500, 420, 520, 440, 480, 460, 480, 420, 500, 460, 480, 440, 500, 440, 500, 440, 480, 440, 480, 460, 980, 440, 480, 960, 960, 440, 500, 460, 460, 960, 460, 460, 500, 420, 980, 440, 500, 440, 500, 920, 960, 960, 960, 940, 980, 920, 960, 460, 500, 940, 960, 960, 460, 460, 960, 460, 480];
    console.log(this.decode(sequence));
  }
}
