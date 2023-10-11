class WasmRtl433
{
  constructor()
  {
    this.ready = this.getCode()
      .then(code => this.load(code))
  }
  load(wasmBinary)
  {
    var TOTAL_MEMORY = 67108864;
    var WASM_PAGE_SIZE = 4096;

    var env = {
      emscripten_memcpy_big: (dest, src, num) => { return this._emscripten_memcpy_big(dest, src, num); },
      emscripten_date_now: () => Date.now(),
      fd_write:(fd, iov, iovcnt, pnum) => { return this._fd_write(fd, iov, iovcnt, pnum); },
      memory:new WebAssembly.Memory({ 'initial': TOTAL_MEMORY / WASM_PAGE_SIZE, 'maximum': TOTAL_MEMORY / WASM_PAGE_SIZE }),
    };

    for (var i of ["__assert_fail", "exit", "strftime", "fd_close", "_tzset_js", "_mktime_js", 
      "_localtime_js", "__syscall_fcntl64", "__syscall_openat", "__syscall_ioctl", "fd_read", 
      "emscripten_resize_heap", "fd_seek"])
    {
      env[i] = () => { throw "not implemented" };
    }

    return WebAssembly.instantiate(wasmBinary, {env:env, wasi_snapshot_preview1: env})
      .then(obj => {
        this.exports = obj.instance.exports;
        this.HEAPU8 = new Uint8Array(this.exports.memory.buffer);
        this.HEAPU32 = new Uint32Array(this.exports.memory.buffer);
        this.exports.__wasm_call_ctors();
        this.analyseBuffer = new Uint32Array(
          this.exports.memory.buffer,
          this.exports.analyseBuffer.value,
          1024*4
        );
      })
  }

  _emscripten_memcpy_big(dest, src, num)
  {
    this.HEAPU8.copyWithin(dest, src, src + num);
  }
  _fd_write(fd, iov, iovcnt, pnum) 
  {
    var ret = 0;
    for (var i = 0; i < iovcnt; i++) {
      var ptr = this.HEAPU32[iov>>2];
      var len = this.HEAPU32[(iov+4)>>2];
      iov += 8;
      for (var j=ptr; j<ptr+len; j++)
        this.stdout += String.fromCharCode(this.HEAPU8[j]);
      ret += len;
    }
    this.HEAPU32[pnum>>2] = ret;
    return 0;
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
        js.onload = () => {
          resolve(wasmBinary)
          wasmBinary = null;
        };
        document.body.appendChild(js);
      });
    });
  }
  decode(sequence)
  {
    if (!("exports" in this))
    {
      console.log("Rtl433 not ready");
      return;
    }
    for (var i=0; i<sequence.length; i++)
      this.analyseBuffer[i] = sequence[i];
    this.stdout = "";
    if (this.exports.analyse(this.exports.analyseBuffer.value, sequence.length))
    {
      return this.stdout.split("\n").filter(x=>x.length).map(JSON.parse);
    }
  }
}
