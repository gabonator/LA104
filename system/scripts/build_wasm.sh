#!/bin/bash

#
# To setup wasm building environment:
#
#   git clone https://github.com/emscripten-core/emsdk.git
#   ./emsdk install latest
#   ./emsdk activate latest
#
# check "which emcc" and remove references in /usr/local
# call emsdk_env.sh to setup environment, or set path to utilities:
# PATH=$PATH:~/Documents/git/ext/emsdk/upstream/emscripten
# PATH=$PATH:~/Documents/git/ext/emsdk/upstream/bin
#

buildApp () {
  (
    cd $1
    if [ ! -f build_wasm.sh ]; then
      exit 1
    fi

    rm -rf build
    ./build_wasm.sh 
#> /dev/null 2> /dev/null

    if [ $? -eq 1 ]; then
      echo "$1: Build failed"
      exit 1
    fi

    for f in build/*.wasm; do
      [ -e "$f" ] && exit 0
      break
    done
    echo "$1: No executable found"
    exit 1
  )

  if [ $? -eq 0 ]; then
    echo "$1: Success!"
  fi
}

cleanApp () {
  (
    cd $1
    rm -rf build
  )
}

for d in ../apps_featured/*/ ; do
    buildApp "$d"
done
for d in ../apps_shell/*/ ; do
    buildApp "$d"
done
for d in ../apps_experiments/*/ ; do
    buildApp "$d"
done
for d in ../apps/*/ ; do
    buildApp "$d"
done
for d in ../apps_usb/*/ ; do
    buildApp "$d"
done

for d in ../apps_ds213/*/ ; do
    buildApp "$d"
done

# copy fresh builds
mkdir ../build
mkdir ../build/wasm
mkdir ../build/wasm/apps
for d in ../apps*/*/ ; do
    cp $d/build/*.wasm ../build/wasm/apps > /dev/null 2> /dev/null
    cp $d/build/*.map ../build/wasm/apps > /dev/null 2> /dev/null
done

# cleanup
for d in ../apps_featured/*/ ; do
    cleanApp "$d"
done
for d in ../apps_shell/*/ ; do
    cleanApp "$d"
done
for d in ../apps_experiments/*/ ; do
    cleanApp "$d"
done
for d in ../apps/*/ ; do
    cleanApp "$d"
done
for d in ../apps_usb/*/ ; do
    cleanApp "$d"
done
for d in ../apps_ds213/*/ ; do
    cleanApp "$d"
done


cp ../os_platform/mac_sdl/data/la104.fat ../build/wasm/la104.fat
cp ../os_platform/wasm/htmllite/app.js ../build/wasm/app.js
cat > ../build/wasm/index.html <<- EOM

<div style="width:640px; height:480px;">
<canvas id="canvas" width="320" height="240" style="border: 1px solid #d0d0d0; transform:translate(160px, 120px) scale(2);"></canvas>
</div>
<br>
Use arrow keys, enter and backspace. Only help and few applications work (file manager, tools/ws2812, fun/trinagle)
<script>

class Device
{
  init() 
  { 
    var canvas = document.getElementById('canvas');
    this.ctx = canvas.getContext('2d');
    this.imageData = this.ctx.getImageData(0, 0, canvas.width, canvas.height);
    this.lastKey = 0;

//    this.videoMemoryPtr = null;
    this.imageDataChanged = false;

    document.onkeydown = (function(evt) {
        evt = evt || window.event;
        switch (evt.keyCode)
        {
          case 37: this.lastKey = "-".charCodeAt(0); break;
          case 39: this.lastKey = "+".charCodeAt(0); break;
          case 38: this.lastKey = "<".charCodeAt(0); break;
          case 40: this.lastKey = ">".charCodeAt(0); break;
          case 13: this.lastKey = "1".charCodeAt(0); break;
          case 8:  this.lastKey = "2".charCodeAt(0); break;
        }
    }).bind(this);
  }

  getKey() 
  {
    var aux = this.lastKey;
    this.lastKey = 0;
    return aux;
  }

  getPixel(x, y) 
  { 
    y = 240-y;
    var p = (y*320+x)*4;
    var r = this.imageData.data[p++];// = (((rgb)&0x1f)<<3);
    var g = this.imageData.data[p++];// = ((((rgb)>>5)&0x3f)<<2);
    var b = this.imageData.data[p++];// = ((((rgb)>>11)&0x1f)<<3);

    return ((r)>>3)|(((g)>>2)<<5)|(((b)>>3)<<11);
  }

  loop() 
  {
  }

  blit()
  {
/*
    var pTo = this.imageData.data;
    var pFrom = Module['memory'];
    for (var i=0; i<320*240*4; i++)
      pTo[i] = pFrom[this.videoMemoryPtr +i];
*/
    if (this.imageDataChanged)
      this.imageDataChanged = false;
    else
      return;

    this.ctx.putImageData(this.imageData, 0, 0);
  }

  running()
  {
    return 1;
  }

  setPixel(x, y, rgb) 
  { 
    y = 240-y;
    var p = (y*320+x)*4;
    this.imageData.data[p++] = (((rgb)&0x1f)<<3);
    this.imageData.data[p++] = ((((rgb)>>5)&0x3f)<<2);
    this.imageData.data[p++] = ((((rgb)>>11)&0x1f)<<3);
    this.imageData.data[p++] = 255;
    this.imageDataChanged = true;
  }

  ticks() 
  { 
    return new Date().getTime();
  }

  flashWrite(ptr, ofs, size)
  {
    for (var i=0; i<size; i++)
      Module['disk'][ofs+i] = Module['memory'][ptr + i];
  }

  flashRead(ptr, ofs, size)
  {
    for (var i=0; i<size; i++)
      Module['memory'][ptr + i] = Module['disk'][ofs+i];
  }

  getString(ptr)
  {
    var text = "";
    for (var i=0; i<100 && Module['memory'][ptr+i] != 0; i++)
      text += String.fromCharCode(Module['memory'][ptr+i]);
    return text;
  }

  setString(ptr, str)
  {                
    var i;
    for (i=0; i<str.length; i++)
      Module['memory'][ptr+i] = str.charCodeAt(i);
    Module['memory'][ptr+i] = 0;
  }

  log(ptr)
  {
    console.log(this.getString(ptr)); 
  }

  setArgument(ptr)
  {
//    shell.biosExecute(this.getString(ptr));
  }
}

var device = new Device()
var shell = null;

class Shell
{
  constructor()
  {
    Promise.resolve()
    .then(() => this.readAsync("la104.fat"))
    .then((data) => Module['disk'] = new Uint8Array(data))
    .then(() => this.main());
  }

  readAsync(url, onload, onerror) 
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

  execute(wasmBinary, imports, exports, tablesize) 
  {
//    Module = {}
    Module['wasmBinary'] = wasmBinary;
    Module['imports'] = {};
    Module['exports'] = exports;
    Module['tablesize'] = tablesize;

    var methods = Object.getOwnPropertyNames(Object.getPrototypeOf(imports));
    for (var i in methods)
      Module['imports']["js_"+methods[i]] = imports[methods[i]].bind(imports);

    return Module['run']();
  }

  LoadApp(file, args)
  {
    console.log("Load app " + file);
    var imports = {appFinish: null, appInit: null, appLoop: null, 
      _ZN4BIOS2OS11SetArgumentEPc:null, _ZN4BIOS2OS11GetArgumentEv:null,
      _ZN4BIOS2OS11HasArgumentEv:null//, _appVideoBuffer: null, _appVideoBufferChanged: null
    };

    var tablesize = {
      "apps/shell.wasm":84,
      "apps/viewtxt.wasm":46,
      "apps/experime_fractal_fractal.wasm":42,
      "apps/utils_charmap_charmap.wasm":52,
      "apps/fileman.wasm":56}[file];

    Promise.resolve()
    .then(() => this.readAsync(file))
    .then((wasmBinary) => this.execute(wasmBinary, device, imports, tablesize))
    .then(() => {
      if (args)
      {
        console.log("Setting arguments: "+args);
        var ptr = imports._ZN4BIOS2OS11GetArgumentEv();
        device.setString(ptr, file + " " + args);

        imports._ZN4BIOS2OS11SetArgumentEPc(ptr);
        imports._ZN4BIOS2OS11HasArgumentEv(); // reset HasArgument flag
      }

//      device.videoMemoryPtr = imports._appVideoBuffer();
      imports.appInit();
      if (Module.timer)
        clearInterval(Module.timer);

      Module.timer = setInterval(() =>
      {
        if (!imports.appLoop())
        {
          console.log("Process exit!");
          imports.appFinish();
          clearInterval(Module.timer);
          Module.timer = null;
          this.onProcessExit();
        }
//        if (imports._appVideoBufferChanged())
        device.blit();
      }, 50);
    })
    .catch((e)=>alert("App not found, " + file));
  }

  onProcessExit()
  {
    if (!Module._ZN4BIOS2OS11HasArgumentEv || !Module._ZN4BIOS2OS11HasArgumentEv())
    {
      // no execute command requested, return to shell
      this.LoadApp("apps/shell.wasm");
      return;
    }

    var newCommand = device.getString(Module._ZN4BIOS2OS11GetArgumentEv())
    console.log("Exit command: "+newCommand);
    if (newCommand.length != 0)
      this.biosExecute(newCommand);
  }

  main()
  {
    this.LoadApp("apps/shell.wasm");
//    this.LoadApp("apps/fileman.elf.wasm");
  }

  biosExecute(commandline)
  {
    var arr = commandline.split(" ");
    var command = arr.shift();
    var args = arr.join(" ");

    command = command.toLowerCase();
    if (command.substr(0, 1) == "/")
      command = command.substr(1);
    if (command.substr(0, 5) == "apps/")
      command = command.substr(5);
    command = command.replace(".elf", ".wasm");
    var path = "apps/" + command.split("/").slice(-1)[0];

    this.LoadApp(path, args);
  }
}

window.onload = () => { shell = new Shell() };
Module = {noRun:1};
</script>
<script src="app.js"></script>

EOM

cd ../build/wasm
rm ../wasm.zip
zip -r ../wasm.zip *
