var fs = require("fs");
var prefix = `Module = Module ? Module : {}; Module['wasmBinary'] = (() => { var wasmcode="`;
var suffix = `"; return Uint8Array.from(atob(wasmcode), c => c.charCodeAt(0));})();`;

fs.writeFileSync("app_wasm.js", prefix + fs.readFileSync("app.wasm").toString("base64") + suffix);
fs.createReadStream(__dirname + '/index.html').pipe(fs.createWriteStream('index.html'));
fs.createReadStream(__dirname + '/app.js').pipe(fs.createWriteStream('app.js'));
