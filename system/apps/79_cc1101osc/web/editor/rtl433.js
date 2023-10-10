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
    var WASM_PAGE_SIZE = 65536;
    var TOTAL_MEMORY = 131072;
    var DYNAMICTOP_PTR = 48928;
    var HEAP_SIZE = 32768;

    var abort = (what) => { throw 'abort(' + what + '). Build with -s ASSERTIONS=1 for more info.'};

    var env = {
      "abort": abort, "_abort": abort, "___assert_fail": abort, "___setErrNo": abort,
      "___syscall140": abort, "___syscall146": abort, "___syscall54": abort, "___syscall6": abort,
      "_emscripten_memcpy_big": abort, "_emscripten_resize_heap": abort, "abortOnCannotGrowMemory": abort, 
      "_emscripten_get_heap_size": () => HEAP_SIZE,
      "DYNAMICTOP_PTR": DYNAMICTOP_PTR,
      memory:new WebAssembly.Memory({ 'initial': TOTAL_MEMORY / WASM_PAGE_SIZE, 'maximum': TOTAL_MEMORY / WASM_PAGE_SIZE }),
      table:new WebAssembly.Table({'initial': 160,'maximum': 160, 'element': 'anyfunc'}),
      '__memory_base': 1024, 
      '__table_base': 0
    };

    return WebAssembly.instantiate(new Uint8Array(binary), {env:env})
    .then( output => 
     {
      var exports = output.instance.exports;
      Module = typeof Module !== 'undefined' ? Module : {};  
      Module["memory"] = new Uint8Array(env.memory.buffer);
      Module["imports"] = {};

      for (var i in imports)
        Module.imports[i] = exports[i];
      return Module;
    });
  };
}

class WasmRtl433
{
  constructor()
  {
    var wasm = new Wasm;
    this.getCode()
      .then(code => wasm.load(code, {_convert:null, _buffer:null}))
      .then(module => this.module = module);
  }
  getCode()
  {
    return new Promise((resolve, reject) => {
      if (typeof(Module) != "undefined" && Module.wasmBinary)
      {
        resolve(Module.wasmBinary);
        return;
      }
      window.addEventListener('load', () => {
        var js = document.createElement("script");
        js.type = "text/javascript";
        js.src = "rtl433_wasm.js";
        js.onload = () => resolve(Module.wasmBinary);
        document.body.appendChild(js);
      });
    });
  }
  getString(ptr)
  {
    var text = "";
    for (var i=0; i<1000 && this.module['memory'][ptr+i] != 0; i++)
      text += String.fromCharCode(this.module['memory'][ptr+i]);
    return text;
  };
  decode(sequence)
  {
    var ptr = this.module.imports._buffer();
    for (var i=0; i<sequence.length; i++)
    {
      var v = sequence[i];
      this.module['memory'][ptr+i*4+0] = v & 0xff;
      this.module['memory'][ptr+i*4+1] = (v >> 8) & 0xff;
      this.module['memory'][ptr+i*4+2] = (v >> 16) & 0xff;
      this.module['memory'][ptr+i*4+3] = (v >> 24) & 0xff;
    }

    var result = this.module.imports._convert(ptr, sequence.length);
    return JSON.parse(this.getString(result));
  }
}
