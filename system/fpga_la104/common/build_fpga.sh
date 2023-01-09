set -e
APPNAME=${PWD##*/}

ARGS="curl --request POST http://cloud.valky.eu:8086/synth"
for var in "$@"
do
  ARGS="${ARGS} --form "\"$(basename $var)=@${PWD}/$var\"
done

cat > log_quartus.html <<- EOM

<script>
window.addEventListener("load", () => {
  document.querySelectorAll("p").forEach(el =>
  {
    el.style = "white-space:pre-wrap";
    var html = el.innerHTML;
    var hasInsert = false;
    while (html.indexOf("\x1b") != -1)
    {
      var tokbegin = html.indexOf("\x1b");
      var tokend = html.indexOf("m", tokbegin);
      var tok = html.substr(tokbegin+1, tokend-tokbegin);
      var insert = "";
      if (tok == "[0;32m")
        insert = "<font color=green>"
      else if (tok == "[0m")
        insert = "<font color=black>"
      else if (tok == "[0;36m")
        insert = "<font color=blue>"
      else if (tok == "[0;31m")
        insert = "<font color=red>"
      else {
        console.log("What to do? ", tok);
        throw "wrong attribute"
      }
      if (hasInsert)
        insert = "</font>" + insert;
      hasInsert = true;
      html = html.substr(0, tokbegin) + insert + html.substr(tokend+1);
    }
    if (hasInsert)
     html += "</font>";
    el.innerHTML = html;
  })
  if (!document.querySelector("b:last-child")?.innerHTML.includes("exit code"))
  {
    setTimeout(()=>document.location.reload(), 2000);
    setTimeout(()=>window.scrollTo(0, document.body.scrollHeight), 0);
  }
});
</script>
EOM

eval unbuffer ${ARGS} | tee -a log_quartus.html

mkdir -p build
curl -s http://cloud.valky.eu:8086/result --output build/${APPNAME}.fpg
cp build/${APPNAME}.fpg /Volumes/LA104/
