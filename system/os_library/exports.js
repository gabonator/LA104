var exec = require('child_process').exec;
function execute(command, callback){
    exec(command, function(error, stdout, stderr){ callback(stdout); });
};

var demangled = [], mangled = [];

var source = require("fs").readFileSync("./source/library.cpp").toString();

var method = {};
var lines = source.split("\n");
var namespace = [];
for (var i in lines)
{
  var line = lines[i];
  var matchBeginNamespace = line.match("namespace (.*)");
  if (matchBeginNamespace)
  {
    namespace.push(matchBeginNamespace[1]);
  }
  var matchEndNamespace = line.match("^\\s*}\\s*$");
  if (matchEndNamespace)
  {
    namespace.pop();
  }
  var matchDecl = line.match("^\\s*(\\w*)\\s*(\\w*)\\s*\\((.*)\\)");
  if (matchDecl)
  {
    var decl = matchDecl[1];
    if (decl.substr(0, 1) == "E") // local enum
      decl = namespace.join("::") + "::" + decl;

    method[namespace.join("::") + "::" + matchDecl[2]] = decl;
//    console.log(matchDecl[1] + " " + namespace.join("::") + "::" + matchDecl[2] + "(" + matchDecl[3] + ")");
  }
}

var _path = "/Users/gabrielvalky/Downloads/gcc-arm-none-eabi-7-2018-q2-update/bin/";
execute(_path + "arm-none-eabi-nm --demangle ./build/libbios.so", out =>
{
  var lines = out.split("\n");
  for (var i in lines)
  {
    var tokens = lines[i].match("^.* T (.*)$")
    if (tokens && tokens.length == 2)
      demangled.push(tokens[1]);
  }

  execute(_path + "arm-none-eabi-nm ./build/libbios.so", out =>
  {
    var lines = out.split("\n");
    for (var i in lines)
    {
      var tokens = lines[i].match("^.* T (.*)$")
      if (tokens && tokens.length == 2)
        mangled.push(tokens[1]);
    }
    finish();
  })

});


function finish()
{
  console.log(`// Automatically generated

uint32_t GetProcAddress(char* symbol)
{
  if (strncmp(symbol, "_ZN4BIOS", 8) == 0)
  {
    symbol += 8;
  } else
  if (strncmp(symbol, "_ZN3GUI", 7) == 0)
  {
    symbol += 7;
  } else
  {
    return 0;
  }

  while (*symbol >= '0' && *symbol <= '9')
    symbol++;

`);

  for (var i = 0; i<demangled.length; i++)
  {
    var tokens = demangled[i].match("^(.*)(\\(.*\\))$")
    var mname = mangled[i];
    mname = mname.replace("_ZN4BIOS", "").replace("_ZN3GUI", "");
    while ("0123456789".indexOf(mname.substr(0, 1)) != -1)
      mname = mname.substr(1);

    var retvalue = method[tokens[1]];
    if (!retvalue)
      retvalue = "unknown";
//    console.log('  // ' + mangled[i]);
    console.log('  if (strcmp(symbol, "' + mname + '") == 0)');
    console.log('    return (uint32_t)static_cast<'+retvalue+'(*)' + tokens[2] + '>(' + tokens[1] + '); ');
  }
  console.log('  return 0;\n}\n');
}              
