var d = new Date(Date.UTC(2022, 1-1, 1, 23, 59, 59) );
function fmt(d)
{
  return d.getUTCFullYear() + "/" +
    ("00" + (d.getUTCMonth()+1)).substr(-2) + "/" +
    ("00" + d.getUTCDate()).substr(-2) + 
    " " +
    ("00" + d.getUTCHours()).substr(-2) + ":" +
    ("00" + d.getUTCMinutes()).substr(-2) + ":" +
    ("00" + d.getUTCSeconds()).substr(-2);
}

console.log("const char* testCases[] = {");
for (var i=0; i<365; i++)
{
  var nd = new Date(d.getTime() + 1000);
//  console.log(d.toUTCString(), nd.toUTCString());
  console.log(`"${fmt(d)}", "${fmt(nd)}",`)
  d = new Date(d.getTime() + 24*60*60*1000)
}
console.log("};");
