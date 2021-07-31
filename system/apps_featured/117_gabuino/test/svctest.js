var request = require("request");


//var req = request.post("http://localhost:8382/symbols", function (err, resp, body) {
var req = request.post("http://cloud.valky.eu:8382/debug", function (err, resp, body) {
  if (err) {
    console.log('Error!' + err);
  } else {      
    console.log(JSON.parse(body));
  }
});

var form = req.form();
form.append('file', "#include <library.h>\nint q=1000; int main(void) { BIOS::DBG::Print(\"ahoj %d\", q+17); return 17; }", {
  filename: 'myfile.cpp',
  contentType: 'text/plain'
});