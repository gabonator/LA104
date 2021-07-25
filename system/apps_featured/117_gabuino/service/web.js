// npm install express
// npm install express-fileupload
// npm install xml2json
// npm install encoding

const _appbase = "/Users/gabrielvalky/Documents/git/LA104/system/apps/000_service";
const _laroot = "/Users/gabrielvalky/Documents/git/LA104";
const _gccroot = "/Applications/ARM/bin/";

const fs = require('fs');
const express = require('express');
const fileUpload = require('express-fileupload');
const app = express();

app.use(fileUpload());
app.use(express.static('public'));

function _toString(buffer)
{
  var textIn = "";
  for (var i=0; i<buffer.length; i++)
    textIn += String.fromCharCode(buffer[i]);
  return textIn;
}

app.post('/', function(req, res) 
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

app.get("/", function(req, res) {
  res.end(require("fs").readFileSync("index.html"))
});

app.listen(8382, function() {
  console.log("Server started at localhost:8382");
});


const { spawn } = require("child_process");

function compile(code)
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
    //"-MD",
    //"../source/main.cpp",
    //"../source/platform.cpp",
    //"../source/radsens/radSens1v2.cpp",
    "-I../../../os_library/include/", "-I../source/arduino/",
//    "-S", // generate assembly
    "-lbios_la104", "-L../../../os_library/build", "-nostartfiles", "-T", "../source/app.lds", "-o", "app.elf",
    "-x", "c++", "-"
    ]

    const ls = spawn("arm-none-eabi-g++", args, {
      cwd:_appbase + "/build",
      env: {
          PATH: process.env.PATH+":"+_gccroot
      }});

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
          PATH: process.env.PATH+":"+_laroot+"/tools/elfstrip/"
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

function exec(command, args)
{
  return new Promise((resolve) => {
    var args = args.split(" ")

    const ls = spawn(command, args, {
      cwd:_appbase + "/build",
      env: {
          PATH: process.env.PATH+":"+_gccroot
      }});

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
        resolve(out);
    });

  });
}
/*
function debug(code)
{
  return new Promise((resolve, reject) =>
  {
    fs.writeFileSync(_appbase + "/source/test.cpp", code);

    var cflags = "-O0 -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics" +
      "-fno-use-cxa-atexit -Wno-psabi -DLA104 " +
      "-I../../../os_library/include/ -I../source/arduino/ -lbios_la104 -L../../../os_library/build ";

    Promise.resolve()
    .then( () => exec("arm-none-eabi-g++", cflags + "-S -fverbose-asm -l -Os " + 
      "-nostartfiles -T ../source/app.lds -o test_gcc.asm -c test.cpp"))
    .then( (r) => 
    {
      if (r.code != 0) 
        throw "compilation failed";
    })
    .then( () => exec("arm-none-eabi-g++", cflags +  "-nostartfiles -T ../source/app.lds -o test.elf test.cpp") )
    .then( (r) => 
    {
      if (r.code != 0) 
        throw "compilation failed";
    })
    .then( () => exec("arm-none-eabi-objdump", "-l -C -S -d -S test.elf") ) // TODO: in stdout
    .then( (r) => 
    {
      if (r.code != 0) 
        throw "compilation failed";
      objdump_asm = r.stdout;
    })
    .then( () => exec("arm-none-eabi-nm", "-a --print-size -C app.elf") ) // TODO: in stdout
    .then( (r) => 
    {
      if (r.code != 0) 
        throw "compilation failed";
      symbols = r.stdout;
    })  
  });
}
*/