var app = process.argv.length == 3 ? process.argv[2] : "app";
var fs = require("fs");
var prefix = `Module = typeof(Module) != "undefined" ? Module : {}; Module['wasmBinary'] = (() => { var wasmcode="`;
var suffix = `"; return Uint8Array.from(atob(wasmcode), c => c.charCodeAt(0));})();`;

fs.writeFileSync(app+"_inline.js", prefix + fs.readFileSync(app + ".wasm").toString("base64") + suffix);

var index = fs.readFileSync(__dirname + '/index.html').toString();
index = index.replace("\"app_wasm.js\"", "\"" + app+"_inline.js" + "\"")
index = index.replace("\"app.js\"", "\"" + app+"_app.js" + "\"")
fs.writeFileSync(app + ".html", index);
fs.createReadStream(__dirname + '/app.js').pipe(fs.createWriteStream(app + "_app.js"));
fs.unlinkSync(app + ".js")
