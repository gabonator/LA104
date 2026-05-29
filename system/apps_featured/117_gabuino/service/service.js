// npm install express
// npm install express-fileupload
// npm install xml2json
// npm install encoding

process.title = "la104 gcc svc";
optlevel = "-O3";

//var optimize = "";
// export PATH=$PATH:/Applications/ARM/bin/:/Users/gabrielvalky/Documents/git/LA104/tools/elfstrip/
const _appbase = "/Users/gabrielvalky/Documents/git/LA104/system/apps/000_service";
const _laroot = "/Users/gabrielvalky/Documents/git/LA104";
const _gccroot = "/Applications/ARM/bin/";
/*

const fs = require('fs');
const express = require('express');
const fileUpload = require('express-fileupload');
const app = express();
var crypto = require('crypto');

function buildToolPath()
{
  var paths = [process.env.PATH || ""];

  if (_gccroot)
    paths.push(_gccroot);

  paths.push(_laroot + "\\tools\\elfstrip");
  paths.push(_laroot + "\\tools");

  return paths.join(";");
}

// Add headers
app.use(function (req, res, next) {
    res.setHeader('Access-Control-Allow-Origin', '*');
    res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS, PUT, PATCH, DELETE');
    res.setHeader('Access-Control-Allow-Headers', 'X-Requested-With,content-type');
    res.setHeader('Access-Control-Allow-Credentials', true);
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

app.post('/compile/:device', function(req, res)
{
  if (["la104", "ds213", "ds203"].indexOf(req.params.device) == -1)
    return res.status(400).send('Unsupported device: ' + req.params.device);

  if (!req.files)
    return res.status(400).send('No files were uploaded.');

  let sampleFile = req.files.file;
  var fileName = sampleFile.name.replace(".xml", ".txt");
  if (fileName.indexOf(".txt") == -1)
    fileName += ".txt";

  var buffer = sampleFile.data;
  var textIn = _toString(buffer);

  if (!checkSafeIncludes(textIn))
    return res.status(400).send('Cannot compile this code');

  compile(textIn, req.params.device).then( out =>
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

app.post('/assembly/:device', function(req, res)
{
  if (["la104", "ds213", "ds203"].indexOf(req.params.device) == -1)
    return res.status(400).send('Unsupported device');

  if (!req.files)
    return res.status(400).send('No files were uploaded.');

  let sampleFile = req.files.file;
  var fileName = sampleFile.name.replace(".xml", ".txt");
  if (fileName.indexOf(".txt") == -1)
    fileName += ".txt";

  var buffer = sampleFile.data;
  var textIn = _toString(buffer);

  if (!checkSafeIncludes(textIn))
    return res.status(400).send('Cannot compile this code');

  debug(textIn, req.params.device).then( out =>
  {
    res.end(JSON.stringify(out));
  });
});

app.post('/symbols/:device', function(req, res)
{
  if (["la104", "ds213", "ds203"].indexOf(req.params.device) == -1)
    return res.status(400).send('Unsupported device');

  if (!req.files)
    return res.status(400).send('No files were uploaded.');

  let sampleFile = req.files.file;
  var fileName = sampleFile.name.replace(".xml", ".txt");
  if (fileName.indexOf(".txt") == -1)
    fileName += ".txt";

  var buffer = sampleFile.data;
  var textIn = _toString(buffer);

  if (!checkSafeIncludes(textIn))
    return res.status(400).send('Cannot compile this code');

  symbols(textIn, req.params.device).then( out =>
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

/*
 * Fixed source filter.
 *
 * The original checkSafeIncludes() only validated #include lines.
 * However, the service compiles user-controlled C/C++ source on the server.
 * Inline assembly and assembler directives are not required by the normal web
 * examples, but they can be abused through directives such as .incbin to embed
 * local server files into the generated ELF.
 *
 * Therefore, reject the whole inline-assembly / assembler-directive surface
 * instead of only blocking one concrete sample.
 */
function checkSafeIncludes(code)
{
  var hash = crypto.createHash('md5').update(code).digest('hex');
  fs.writeFileSync("logs/" + hash, code);

  var allowed = [
    "#include <library.h>",
    "#include <math.h>"];

  var forbiddenPatterns = [
    /\b(__asm__|__asm|asm)\b/i,
    /\bvolatile\s+asm\b/i,

    /\.incbin\b/i,
    /\.include\b/i,
    /\.ascii\b/i,
    /\.asciz\b/i,
    /\.string\b/i,
    /\.byte\b/i,
    /\.short\b/i,
    /\.long\b/i,
    /\.quad\b/i,

    /^\s*\.[A-Za-z_][A-Za-z0-9_]*\b/m,

    /^\s*#\s*pragma\b/im,
    /^\s*#\s*line\b/im
  ];

  for (var j=0; j<forbiddenPatterns.length; j++)
  {
    if (forbiddenPatterns[j].test(code))
      return false;
  }

  var lines = code.split("\n");
  for (var i=0; i<lines.length; i++)
  {
    var line = lines[i].trim();

    if (/^\s*#\s*include\b/.test(line))
    {
      if (allowed.indexOf(line) == -1)
        return false;
    }
  }

  return true;
}

const { spawn } = require("child_process");

function compile(code, device)
{
  return new Promise((resolve) => {
    var args = [
      optlevel,
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
      "-D" + device.toUpperCase(),
      "-Wl,-emain",
      "-I../../../os_library/include/",
      "-I../source/arduino/",
      "-lbios_" + device,
      "-L../../../os_library/build",
      "-nostartfiles",
      "-T",
      "../source/app.lds",
      "-o",
      "app.elf",
      "-x",
      "c++",
      "-"
    ];

    const ls = spawn("arm-none-eabi-g++", args, {
      cwd: _appbase + "\\build",
      env: {
        PATH: buildToolPath()
      }
    });

    var out = {};
    var resolved = false;

    function finish(obj) {
      if (!resolved) {
        resolved = true;
        resolve(obj);
      }
    }

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
      out.code = 1;
      out.error = error.message;
      finish(out);
    });

    ls.on("close", code => {
      console.log(`child process exited with code ${code}`);
      out.code = code;
      if (code == 0)
        out.files = {"app.elf": _appbase + "\\build\\app.elf"};
      finish(out);
    });
  });
}

function optimize(name)
{
  return new Promise((resolve) => {
    const ls = spawn("elfstrip", ["app.elf", "appmini.elf"], {
      cwd: _appbase + "\\build",
      env: {
        PATH: buildToolPath()
      }
    });

    var out = {};
    var resolved = false;

    function finish(obj) {
      if (!resolved) {
        resolved = true;
        resolve(obj);
      }
    }

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
      out.code = 1;
      out.error = error.message;
      finish(out);
    });

    ls.on("close", code => {
      console.log(`child process exited with code ${code}`);
      out.code = code;
      if (code == 0)
      {
        delete out.stdout;
        out.files = {"app.elf": fs.readFileSync(_appbase + "\\build\\appmini.elf").toString("base64")};
      }
      finish(out);
    });
  });
}

function exec(command, args, o)
{
  return new Promise((resolve) => {
    var _args = args.split(" ");
    console.log([command, ..._args].join(" "));

    var opt = {
      env: {
        PATH: buildToolPath()
      }
    };

    if (o && o.cwd)
      opt.cwd = o.cwd;

    const ls = spawn(command, _args, opt);

    var out = {};
    var resolved = false;

    function finish(obj) {
      if (!resolved) {
        resolved = true;
        resolve(obj);
      }
    }

    if (o && o.stdin)
    {
      ls.stdin.write(o.stdin);
      ls.stdin.end();
    }

    ls.stdout.on("data", data => {
      if (typeof(out.stdout) == "undefined") out.stdout = "";
      out.stdout += data;
    });

    ls.stderr.on("data", data => {
      if (typeof(out.stderr) == "undefined") out.stderr = "";
      out.stderr += data;
      console.log(`stderr: ${data}`);
    });

    ls.on('error', (error) => {
      console.log(`unhandled error: ${error.message}`);
      out.code = 1;
      out.error = error.message;
      finish(out);
    });

    ls.on("close", code => {
      console.log(`child process exited with code ${code}`);
      out.code = code;
      finish(out);
    });
  });
}

function symbols(code, device)
{
  fs.writeFileSync(_appbase + "\\source\\test.cpp", code);

  var cflags = optlevel + " -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics " +
    "-fno-use-cxa-atexit -Wno-psabi -D" + device.toUpperCase() + " " +
    "-Wl,-emain " +
    "-I../../../os_library/include/ -I../source/arduino/ -lbios_" + device + " -L../../../os_library/build";

  return Promise.resolve()
  .then( () => exec("arm-none-eabi-g++", cflags +
    " -nostartfiles -T ../source/app.lds -o appsym.elf ../source/test.cpp", {cwd: _appbase + "\\build"}))
  .then( (r) =>
  {
    if (r.code != 0)
      throw "compilation failed";
  })
  .then( () => exec("arm-none-eabi-nm", "-a --print-size -C appsym.elf", {cwd: _appbase + "\\build"}))
  .then( (r) =>
  {
    if (r.code != 0)
      throw "symbol export failed";
    fs.writeFileSync(_appbase + "\\build\\test.txt", r.stdout);
  })
  .then( () => exec("node", "nmparse.js " + _appbase + "\\build\\test.txt test"))
  .then( (r) =>
  {
    if (r.code != 0)
      throw "symbol processing failed";
    return r;
  })
  .catch( (e) =>
  {
    console.log(e);
    return JSON.stringify({error:e});
  });
}

function debug(code, device)
{
  fs.writeFileSync(_appbase + "\\source\\test.cpp", code);

  var cflags = optlevel + " -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics " +
    "-fno-use-cxa-atexit -Wno-psabi -D" + device.toUpperCase() + " " +
    "-Wl,-emain " + "-g " +
    "-I../../../os_library/include/ -I../source/arduino/ -lbios_" + device + " -L../../../os_library/build";

  return Promise.resolve()
  .then( () => exec("arm-none-eabi-g++", cflags + " -nostartfiles -T ../source/app.lds " + "-o test.elf ../source/test.cpp", {cwd: _appbase + "\\build"}) )
  .then( (r) =>
  {
    if (r.code != 0)
      throw "compilation failed - 2";
  })
  .then( () => exec("arm-none-eabi-objdump", "-l -C -d test.elf", {cwd: _appbase + "\\build"}) )
  .then( (r) =>
  {
    if (r.code != 0)
      throw "compilation failed - 3";
    fs.writeFileSync(_appbase + "\\build\\objdump.asm", r.stdout);
  })
  .then( () => exec("node", "assembly.js " + _appbase + "\\build\\objdump.asm"))
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
