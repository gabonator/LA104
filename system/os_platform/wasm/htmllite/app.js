var Module = typeof Module !== 'undefined' ? Module : {};

function Main() 
{
  var WASM_PAGE_SIZE = 65536;
  var TOTAL_MEMORY = 65536;
  var DYNAMICTOP_PTR = 11856;

  var abort = (what) => { throw 'abort(' + what + '). Build with -s ASSERTIONS=1 for more info.'};

  var env =
     { "abort": abort, 
       "_abort": abort,
       "__assert_fail": abort, 
       "__setErrNo": abort, 
       "emscripten_get_heap_size": abort, 
       "emscripten_memcpy_big": abort, 
       "emscripten_resize_heap": abort,
       "abortOnCannotGrowMemory": abort, 
       "DYNAMICTOP_PTR": DYNAMICTOP_PTR,
       memory:new WebAssembly.Memory({ 'initial': TOTAL_MEMORY / WASM_PAGE_SIZE, 'maximum': TOTAL_MEMORY / WASM_PAGE_SIZE }),
       table:new WebAssembly.Table({'initial': 84,'maximum': 84,'element': 'anyfunc'}),
       '__memory_base': 1024, 
       '__table_base': 0
  };

  var imports = Module["imports"];
  for (var i in imports)
    env[i] = imports[i];

  return WebAssembly.instantiate(new Uint8Array(Module['wasmBinary']), {env:env})
  .then( output => 
   {
    var exports = output.instance.exports;
    Module['asm'] = exports;
    if (Module["asm"]["__wasm_call_ctors"]) Module["asm"]["__wasm_call_ctors"]();
    Module["memory"] = new Uint8Array(env.memory.buffer);

    var _exports = Module['exports'];
    for (var i in _exports)
      Module[i] = _exports[i] = Module["asm"][i];
        
    if (Module['onRuntimeInitialized']) Module['onRuntimeInitialized']();
  });
};

Module['imports'] = {};
Module['exports'] = {};
Module['run'] = Main;

if (!Module['noRun'])
  Main();
