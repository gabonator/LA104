var fs = require("fs");
var prefix = `Module = typeof(Module) != "undefined" ? Module : {}; Module['disk'] = (() => { var wasmcode="`;
var suffix = `"; return Uint8Array.from(atob(wasmcode), c => c.charCodeAt(0));})();`;

fs.writeFileSync("app_fat.js", prefix + fs.readFileSync("la104.fat").toString("base64") + suffix);
