// npm install express
// npm install express-fileupload
// npm install xml2json
// npm install encoding

process.title = "la104 gcc svc";

// export PATH=$PATH:/Applications/ARM/bin/:/Users/gabrielvalky/Documents/git/LA104/tools/elfstrip/
const _appbase = "/Users/gabrielvalky/Documents/git/LA104/system/apps/000_service";
const _laroot = "/Users/gabrielvalky/Documents/git/LA104";
const _gccroot = "/Applications/ARM/bin/";
/*
const _appbase = "/root/gcc/LA104/system/apps/000_service";
const _laroot = "/root/gcc/LA104";
const _gccroot = "/root/gcc/gcc-arm-none-eabi-10-2020-q4-major/bin/";
*/
const fs = require('fs');
const express = require('express');
const fileUpload = require('express-fileupload');
const app = express();

// Add headers
app.use(function (req, res, next) {
    // Website you wish to allow to connect
    res.setHeader('Access-Control-Allow-Origin', '*');
    // Request methods you wish to allow
    res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS, PUT, PATCH, DELETE');
    // Request headers you wish to allow
    res.setHeader('Access-Control-Allow-Headers', 'X-Requested-With,content-type');
    // Set to true if you need the website to include cookies in the requests sent
    // to the API (e.g. in case you use sessions)
    res.setHeader('Access-Control-Allow-Credentials', true);
    // Pass to next layer of middleware
    next();
});

app.use(fileUpload());
app.use(express.static('public'));

function _toString(buffer)
{
  var textIn = "";
  for (var i=0; i<buffer.length; i++)
    textIn += String.fromCharCode(buffer[i]);
  return textIn;
}

app.post('/compile', function(req, res) 
{
  if (!req.files)
    return res.status(400).send('No files were uploaded.');

  let sampleFile = req.files.file;
  var fileName = sampleFile.name.replace(".xml", ".txt");
  if (fileName.indexOf(".txt") == -1)
    fileName += ".txt";
  var buffer = sampleFile.data;
  var textIn = _toString(buffer);

  compile(textIn).then( out =>
  {
    if (out.code != 0)
    {
      res.end(JSON.stringify(out));
      return;
    }

    optimize(out.files["app.elf"]).then( out =>
    {
      res.end(JSON.stringify(out));
    });
  });
});

app.post('/compileg', function(req, res) 
{
  if (!req.files)
    return res.status(400).send('No files were uploaded.');

  let sampleFile = req.files.file;
  var fileName = sampleFile.name.replace(".xml", ".txt");
  if (fileName.indexOf(".txt") == -1)
    fileName += ".txt";
  var buffer = sampleFile.data;
  var textIn = _toString(buffer);

  compile(textIn, '-g').then( out =>
  {
    if (out.code != 0)
    {
      res.end(JSON.stringify(out));
      return;
    }

    optimize(out.files["app.elf"]).then( out =>
    {
      res.end(JSON.stringify(out));
    });
  });
});

app.post('/assembly', function(req, res) 
{
  if (!req.files)
    return res.status(400).send('No files were uploaded.');

  let sampleFile = req.files.file;
  var fileName = sampleFile.name.replace(".xml", ".txt");
  if (fileName.indexOf(".txt") == -1)
    fileName += ".txt";
  var buffer = sampleFile.data;
  var textIn = _toString(buffer);

  debug(textIn).then( out =>
  {
    res.end(JSON.stringify(out));
  });
});

app.post('/symbols', function(req, res) 
{
  if (!req.files)
    return res.status(400).send('No files were uploaded.');

  let sampleFile = req.files.file;
  var fileName = sampleFile.name.replace(".xml", ".txt");
  if (fileName.indexOf(".txt") == -1)
    fileName += ".txt";
  var buffer = sampleFile.data;
  var textIn = _toString(buffer);

  symbols(textIn).then( out =>
  {
    res.end(JSON.stringify(out));
  });
});

app.get("/", function(req, res) {
  res.end(require("fs").readFileSync("index.html"))
});

app.listen(8382, function() {
  console.log("Server started at localhost:8382");
});


const { spawn } = require("child_process");

function compile(code, extra)
{
  return new Promise((resolve) => {
    var args = [
    "-O0",
    "-Werror",
    "-fno-common",
    "-mcpu=cortex-m3",
    "-mthumb",
    "-msoft-float",
    "-fno-exceptions",
    "-fno-rtti",
    "-fno-threadsafe-statics",
    "-fno-use-cxa-atexit",
    "-Wno-psabi",
    "-DLA104",
    "-Wl,-emain",
    //"-MD",
    //"../source/main.cpp",
    //"../source/platform.cpp",
    //"../source/radsens/radSens1v2.cpp",
    "-I../../../os_library/include/", "-I../source/arduino/",
//    "-S", // generate assembly
    "-lbios_la104", "-L../../../os_library/build", "-nostartfiles", "-T", "../source/app.lds", "-o", "app.elf",
    "-x", "c++", "-"
    ]
    if (extra)
      args.push(extra)

    const ls = spawn("arm-none-eabi-g++", args, {
      cwd:_appbase + "/build",
      env: {
          PATH: process.env.PATH
      }
});

    var out = {};

    ls.stdin.write(code);
    ls.stdin.end();

    ls.stdout.on("data", data => {
        if (typeof(out.stdout) == "undefined") out.stdout = "";
        out.stdout += data;
        console.log(`stdout: ${data}`);
    });

    ls.stderr.on("data", data => {
        if (typeof(out.stderr) == "undefined") out.stderr = "";
        out.stderr += data;
        console.log(`stderr: ${data}`);
    });

    ls.on('error', (error) => {
        console.log(`unhandled error: ${error.message}`);
    });

    ls.on("close", code => {
        console.log(`child process exited with code ${code}`);
        out.code = code;
        if (code == 0)
          out.files = {"app.elf": _appbase + "/build/app.elf"}; 
        resolve(out);
//        prepare();
    });

  });
}

function optimize(name)
{
  return new Promise((resolve) => {
    const ls = spawn("elfstrip", ["app.elf", "appmini.elf"], {
      cwd:_appbase+"/build",
      env: {
          PATH: process.env.PATH //+":"+_laroot+"/tools/elfstrip/"
      }});

    var out = {};

    ls.stdout.on("data", data => {
        if (typeof(out.stdout) == "undefined") out.stdout = "";
        out.stdout += data;
        console.log(`stdout: ${data}`);
    });

    ls.stderr.on("data", data => {
        if (typeof(out.stderr) == "undefined") out.stderr = "";
        out.stderr += data;
        console.log(`stderr: ${data}`);
    });

    ls.on('error', (error) => {
        console.log(`unhandled error: ${error.message}`);
    });

    ls.on("close", code => {
        console.log(`child process exited with code ${code}`);
        out.code = code;
        if (code == 0)
        {
          delete out.stdout;
          out.files = {"app.elf": fs.readFileSync(_appbase+"/build/appmini.elf").toString("base64")};
        }
        resolve(out);
    });
  });
}
//compile("int main(void) { return 12; }");

function exec(command, args, o)
{
  return new Promise((resolve) => {


    var _args = args.split(" ");
console.log([command, ..._args].join(" "));
//console.log(_args);

var opt = {
//      cwd:_appbase + "/build",
      env: {
          PATH: process.env.PATH+":"+_gccroot
      }
}
if (o && o.cwd) opt.cwd = o.cwd;

    const ls = spawn(command, _args, opt);

    var out = {};

    if (o && o.stdin)
    {
      ls.stdin.write(o.stdin);
      ls.stdin.end();
    }                                                                      

    ls.stdout.on("data", data => {
        if (typeof(out.stdout) == "undefined") out.stdout = "";
        out.stdout += data;
        //console.log(`stdout: ${data}`);
    });

    ls.stderr.on("data", data => {
        if (typeof(out.stderr) == "undefined") out.stderr = "";
        out.stderr += data;
        console.log(`stderr: ${data}`);
    });

    ls.on('error', (error) => {
        console.log(`unhandled error: ${error.message}`);
    });

    ls.on("close", code => {
        console.log(`child process exited with code ${code}`);
        out.code = code;
        resolve(out);
    });

  });
}


function symbols(code)
{
  fs.writeFileSync(_appbase + "/source/test.cpp", code);

  var cflags = "-O0 -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics " +
    "-fno-use-cxa-atexit -Wno-psabi -DLA104 " +
    "-Wl,-emain " +
    "-I../../../os_library/include/ -I../source/arduino/ -lbios_la104 -L../../../os_library/build";

  return Promise.resolve()
  .then( () => exec("arm-none-eabi-g++", cflags + 
    " -nostartfiles -T ../source/app.lds -o appsym.elf ../source/test.cpp", {cwd:_appbase + "/build"}))
  .then( (r) => 
  {
    if (r.code != 0) 
      throw "compilation failed";
  })
  .then( () => exec("arm-none-eabi-nm", "-a --print-size -C appsym.elf", {cwd:_appbase + "/build"}))
  .then( (r) => 
  {
    if (r.code != 0) 
      throw "symbol export failed";
    fs.writeFileSync(_appbase+"/build/test.txt", r.stdout);
  })
/*
  .then( () => exec("arm-none-eabi-nm", "-a --print-size -C "))
  .then( (r) => 
  {
    if (r.code != 0) 
      throw "compilation failed";

    fs.writeFileSync("test.txt", r.stdout);
  })*/
  .then( () => exec("node", "nmparse.js "+_appbase+"/build/test.txt test"))
  .then( (r) => 
  {
    if (r.code != 0) 
      throw "symbol processing failed";
    return r;
  })
}

function debug(code)
{
  fs.writeFileSync(_appbase + "/source/test.cpp", code);

  var cflags = "-O0 -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics " +
    "-fno-use-cxa-atexit -Wno-psabi -DLA104 " +
    "-Wl,-emain " + "-g " +
    "-I../../../os_library/include/ -I../source/arduino/ -lbios_la104 -L../../../os_library/build";

  return Promise.resolve()
/*
  .then( () => exec("arm-none-eabi-g++", cflags + " -S -fverbose-asm " + 
    "-nostartfiles -T ../source/app.lds -o test_gcc.asm -c ../source/test.cpp", {cwd:_appbase + "/build"}))
  .then( (r) => 
  {
    if (r.code != 0) 
      throw "compilation failed - 1";
  })
*/
  .then( () => exec("arm-none-eabi-g++", cflags +  " -nostartfiles -T ../source/app.lds " + "-o test.elf ../source/test.cpp", {cwd:_appbase + "/build"}) )
  .then( (r) => 
  {
    if (r.code != 0) 
      throw "compilation failed - 2";
  })
  .then( () => exec("arm-none-eabi-objdump", "-l -C -d test.elf", {cwd:_appbase + "/build"}) ) // TODO: in stdout
  .then( (r) => 
  {
    if (r.code != 0) 
      throw "compilation failed - 3";
    fs.writeFileSync(_appbase + "/build/objdump.asm", r.stdout);
  })
  .then( () => exec("node", "assembly.js "+_appbase+"/build/objdump.asm"))
  .then( (r) =>
  {
    console.log("done!!!");
    if (r.code != 0) 
      throw "symbol processing failed - 4";
    return r;
  })
  .catch( (e) =>
  {
    console.log(e);
    return JSON.stringify({error:e});
  });
}
