const testFolder = '.';
const fs = require('fs');

var out = {};

fs.readdirSync(testFolder).forEach(file => {
  if (file.substr(-4) != ".cpp")
    return;
  var cont = fs.readFileSync(file).toString();
  out[file] = cont;
});

console.log("examples = ")
console.log(out);