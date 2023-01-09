namespace Verilator
{
    FILE* fLog = nullptr;
    void Begin()
    {
      fLog = fopen("log.html", "w");
      fputs(R"---(
<style>
#head {font-weight:bold; display: inline-block; width:40px; border:2px solid rgba(0, 0, 0, 0); border-style: hidden solid hidden hidden; margin-left:10px;}
#comment {display: inline-block; margin-left:10px;
  background: #c0c0f0;
  border-radius: 6px;
  padding-left:6px;
  padding-right:6px;
  font-weight:bold;
  color:#202020;
}

#comment_bench {display: inline-block; margin-left:10px;
  background: #c0f0c0;
  border-radius: 6px;
  padding-left:6px;
  padding-right:6px;
  font-weight:bold;
  color:#202020;
}

#comment_verify {display: inline-block; margin-left:10px;
  background: #f0c0c0;
  border-radius: 6px;
  padding-left:6px;
  padding-right:6px;
  font-weight:bold;
  color:#202020;
}
.global { padding-left:14px;}
.group {border:2px solid #d0d0d0; border-radius:6px; padding-left:12px;}
#sig00 {padding-left:4px; background:#ffffff; display: inline-block; width:30px; border:1px solid transparent; border-left-color:#000; margin-left:10px;}
#sig01 {padding-left:4px; background:#d0d0d0; display: inline-block; width:30px; border:1px solid transparent; border-right-color:#000; border-top-color:#000; margin-left:10px;}
#sig10 {padding-left:4px; background:#ffffff; display: inline-block; width:30px; border:1px solid transparent; border-left-color:#000; border-top-color:#000; margin-left:10px;}
#sig11 {padding-left:4px; background:#d0d0d0; display: inline-block; width:30px; border:1px solid transparent; border-right-color:#000; margin-left:10px;}

</style>

<script>
class __ {
  constructor()
  {
    this.rising = true;
    this.falling = false;
    if (this.rising && this.falling)
      this.sig = ["clk", "ss", "sck", "mosi", "miso"];
    else
      this.sig = ["ss", "sck", "mosi", "miso"];
    this.last = {clk:0, ss:0, sck:0, miso:0, mosi:0};
    this.nested = 0;
    this.suppress = false;
  }
  heading()
  {
    for (var s of this.sig)
      document.write(`<div id='head'>${s}</div>`)
  }
  state(sig)
  {
    if (!this.falling)
      this.suppress = !(sig.clk && !this.last.clk);
    if (!this.suppress)
    {
      document.write("<br>");
      for (var s of this.sig)
        document.write(`<div id='sig${this.last[s]}${sig[s]}'>${sig[s]}</div>`);
    }
    this.last = sig;
  }
  blog(msg) { if (this.suppress) return; document.write("<br>"); this.log("comment_bench", msg) }
  vlog(msg) { this.log("comment_verify", msg) }
  slog(msg) { this.log("comment", msg) }
  tlog(msg) { this.log("comment", msg) }
  groupBegin(msg) { 
    if (this.nested++ == 0)
      document.write(`</div>`);
    document.write(`<div class='group'><input type='button' value='Collapse: ${msg}' onClick='_.collapse(this);'><br><div class='group_contents'>`); 
  }
  groupEnd() { document.write("</div></div>");
    if (--this.nested == 0)
      document.write("<div class='global'>");
  }
  log(cl, msg) { 
//   if (this.suppress) return; 
   msg = msg.split("<").join("&lt;").split(">").join("&gt;");
   document.write(`<div id='${cl}'>${msg}</div>`); 
  }

  collapse(el)
  {
    el = el.parentElement.querySelector(".group_contents"); 
    el.style.display = el.style.display == "none" ? "" : "none";
  }
}
var _ = new __();
document.write("<div class='global'>");
_.heading();
</script>

<script>
function print()
{
)---", fLog);
    }

    void End()
    {
      if (!fLog)
        return;
      fputs(R"(
}
print();
</script>)", fLog);
      fclose(fLog);
    }

    int PrintSim(const char *format, ...)
    {
      if (!fLog)
        return 0;
      va_list args;
      va_start(args, format);
      char temp[1024];
      vsprintf(temp, format, args);
      if (temp[strlen(temp)-1] == '\n')
        temp[strlen(temp)-1] = 0;
      fprintf(fLog, "_.slog('%s');", temp);
      va_end(args);
      return 0;
    }

    int PrintTrace(const char *format, ...)
    {
      if (!fLog)
        return 0;
      va_list args;
      va_start(args, format);
      char temp[1024];
      vsprintf(temp, format, args);
      if (temp[strlen(temp)-1] == '\n')
        temp[strlen(temp)-1] = 0;
      fprintf(fLog, "_.vlog('%s');", temp);
      va_end(args);
      return 0;
    }

    int PrintBench(const char *format, ...)
    {
      if (!fLog)
        return 0;
      va_list args;
      va_start(args, format);
      char temp[1024];
      vsprintf(temp, format, args);
      if (temp[strlen(temp)-1] == '\n')
        temp[strlen(temp)-1] = 0;
      fprintf(fLog, "_.blog('%s');", temp);
      va_end(args);
      return 0;
    }

    int PrintVerify(const char *temp2)
    {
      if (!fLog)
        return 0;
      char temp[1024];
      int i;
      for (i=0; temp2[i]; i++)
        temp[i] = (temp2[i] == '\n') ? ' ' : temp2[i];
      temp[i] = 0;
      fprintf(fLog, "_.vlog('%s');", temp);
      return 0;
    }

    int PrintState(const char *format, ...)
    {
      if (!fLog)
        return 0;
      va_list args;
      va_start(args, format);
      char temp[1024];
      vsprintf(temp, format, args);
      if (temp[strlen(temp)-1] == '\n')
        temp[strlen(temp)-1] = 0;
      fprintf(fLog, "\n_.state(%s);", temp);
      va_end(args);
      return 0;
    }
    void GroupBegin(const char *format, ...)
    {
      if (!fLog)
        return;
      va_list args;
      va_start(args, format);
      char temp[1024];
      vsprintf(temp, format, args);
      fprintf(fLog, "\n_.groupBegin('%s');\n", temp);
      va_end(args);
    }
    void GroupEnd()
    {
      if (!fLog)
        return;
      fprintf(fLog, "_.groupEnd();\n");
    }
}

