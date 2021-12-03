identTimer = null;
function identify()
{
  return new Promise( (resolve, reject) =>
  {
    identTimer = setTimeout(()=>
    {
      // TODO: first packet is always broken without response
      // reset
      COMM._onReceive = COMM._defReceive;

      BIOS.identify().then( resolve );
    }, 200);
    BIOS.identify().then( id => { clearTimeout(identTimer); resolve(id); });
  });
}
function compile()
{
  dbg.setCode(html_editor.getValue());
  return identify()
    .then( (id) => 
    {
      dbg.setDeviceInfo(id.device, id.os, id.host, id.id);
    })
    .then( () => dbg.compile(html_editor.getValue()))
    .then(binary => { 
      clearError();
      return binary;
    })
    .catch(error => { 
      handleError(error);
      return Promise.reject(error);
    })
}

function writeDword(ofs, val)
{
  globalBlob[ofs-globalOffset+3] = val >> 24;
  globalBlob[ofs-globalOffset+2] = val >> 16;
  globalBlob[ofs-globalOffset+1] = val >> 8;
  globalBlob[ofs-globalOffset+0] = val >> 0;
}

function readDword(ofs)
{
  var t = 0;
  t = globalBlob[ofs-globalOffset+3]; 
  t <<= 8;
  t |= globalBlob[ofs-globalOffset+2];
  t <<= 8;
  t |= globalBlob[ofs-globalOffset+1];
  t <<= 8;
  t |= globalBlob[ofs-globalOffset+0];
  return t;
}

function clearBss(blob, ofs, len)
{
  for (var i=0; i<len; i++)
    blob[i+ofs] = 0;
}

function flash()
{
  return new Promise((resolve, reject) =>
  {
    var checksum = 0;
    for (var i=0; i<globalBlob.length; i++)
      checksum += globalBlob[i];

    BIOS.memWrite(globalOffset, globalBlob)
    .then( (chk) => {
      if (checksum != chk)
      {
        alert("checksum wrong!");
        reject();
      }
      else
      {
        resolve() 
      }
    });
  });
}

function run()
{ 
  console.log("globalInit: ", globalInit);
  var prepare = Promise.resolve();
  if (globalInit.length > 0)
  {
    prepare = globalInit.reduce((p, x) => p.then(_ => BIOS.exec(x|1)), Promise.resolve())
//    for (var i=0; i<globalInit.length; i++)
//      BIOS.exec(globalInit[i]|1); // we should chain these!
  }
  return prepare.then( () => {
    console.log("globalInit done, executing main app");
    return BIOS.exec(globalEntry|1) 
  })
}

var globalOffset;
var globalBlob;
var globalInit;
var globalResolve = [];
function processElf(elf)
{
  return new Promise((resolve, reject) =>
  {
    var elfinfo = parseElf(elf);

    elfinfo.relocations.forEach(r => console.log(r.name + " -> " + r.addr));
    elfinfo.sections.forEach(s => console.log(s.name + " - " + s.addr.toString(16) + " - " + (s.addr + s.size).toString(16)));
    var begin = elfinfo.sections[0].addr;
    var end = elfinfo.sections[elfinfo.sections.length-1].addr + elfinfo.sections[elfinfo.sections.length-1].size;
    console.log("blob size " + (end-begin));
    if (end - begin > 5*1024)
      throw "too big blob";
  // TODO: BSS!!!!
    var appblob = new Uint8Array(end-begin);
    globalBlob = appblob;
    var init = [];
    for (var i=0; i<elfinfo.sections.length; i++)
    {
      var section = elfinfo.sections[i];
      if (section.name != ".bss")
        section.data.copyTo(appblob, section.addr - begin);
      else
        clearBss(appblob, section.addr - begin, section.size);

      if (section.name == ".init_array")
      {
        globalOffset = begin;
        globalBlob = appblob;
        for (var i=0; i<section.size; i+=4)
          init.push(readDword(section.addr + i));
      }
    }
    for (var i=0; i<elfinfo.relocations.length; i++)
    {
      var rel = elfinfo.relocations[i];
      globalResolve.push({...rel});
  //    writeDword(appblob, rel.addr, symbolCache[rel.name])
    }

    console.log("flash to: "+begin.toString(16) + " entry: "+elfinfo.entry.toString(16));
    console.log(appblob);
    globalOffset = begin;
    globalBlob = appblob;
    globalInit = init;
    globalEntry = elfinfo.entry;
    resolve(elfinfo)
//    continueResolve(resolve);
  });
}

function resolveImports()
{
  return new Promise((resolve, reject) =>
  {
    continueResolve(resolve);
  });
}

var resolveCache = {};

function continueResolve(resolve)
{
  if (globalResolve.length == 0)
  {
    resolve();
    return;
  }
  var symbol = globalResolve.shift();
  if (typeof(resolveCache[symbol.name]) != "undefined")
  {
    ptr = resolveCache[symbol.name];
    console.log(symbol.addr.toString(16) + " <- " + ptr.toString(16) + " cached " + symbol.name)
    writeDword(symbol.addr, ptr);
    continueResolve(resolve);
    return;
  }

  BIOS.getProcAddr(symbol.name).then( (ptr) =>
  {
    resolveCache[symbol.name] = ptr;
    console.log(symbol.addr.toString(16) + " <- " + ptr.toString(16))
    writeDword(symbol.addr, ptr);
    continueResolve(resolve);
  })
}

