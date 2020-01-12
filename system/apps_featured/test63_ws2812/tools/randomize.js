var arr = [];
for (var i =0; i<256; i++)
  arr[i] = i;

for (var i = 0; i < 7777; i++)
{
  var a = Math.floor(Math.random()*arr.length);
  var b = Math.floor(Math.random()*arr.length);
  var t = arr[a];
  arr[a] = arr[b];
  arr[b] = t;
}

console.log("const uint8_t randomized["+arr.length+"] = {" + arr.join(", ") + "};");