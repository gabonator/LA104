var dbg = new Debugger();

class DebuggerUi
{
  constructor(instance)
  {
    this.code = null;
    this.variables = [];
    this.instance = instance;
    this.offline = true;
  }

  demo()
  {
    this.setCode(html_editor.getValue());
    var stack = ['0x00000000', '0x08041b77 (ds213_gabuino) st_usbfs_poll', '0x08041b03 (ds213_gabuino) st_usbfs_poll', '0x42228198', '0x60000000', '0xfffffff9', '0x00000123', '0x00000000', '0x0000018f', '0x000000ef', '0x0800028d', '0x08008649 (ds213_os_bb9698a0) BIOS::LCD::PutPixel(int, int, unsigned short)', '0x080002ae', '0x69000000', '0x0000462c', '0x00000079', '0x00000000', '0x00000123', '0x2000525b', '0x000000c8', '0x00007900', '0x00012301', '0x200084b8', '0x20008498', '0x592e8480', '0x00007900', '0x20004780 (ds213_gabuino) MEMORY::userRetVal', '0x20008490', '0x2000552f', '0x00007b00', '0x00013100', '0x00006d00', '0x0000c800', '0x00000e00', '0x00006900', '0x200084ac', '0x20001524 (ds213_os_bb9698a0) Hw', '0x200084b4', '0x00000000', '0x200084c7', '0x200084c4', '0x20008500', '0x0073cdbd', '0x00007b00', '0x00013100', '0x00006d00', '0x0000c800', '0x00000080', '0x00007b00', '0x00013100', '0x00000022', '0x00012301', '0x00007938', '0x00000000', '0x00007b00', '0x20008508', '0x2000509b', '0x00003ecc', '0x00000000', '0x00000061', '0x00003ecc', '0x0000007b', '0x00000131', '0x00000001', '0x00000000', '0x0000006d', '0x000000c8', '0xfdffbd7c', '0x20004834 (ds213_gabuino) MEMORY::writeSum', '0x20008538', '0x200050f9', '0x20008540', '0x20005105', '0x20008548', '0x20005111', '0x20008550', '0x2000511d', '0x20008558', '0x20005009', '0x00007fe5', '0x08040751 (ds213_gabuino) MEMORY::Exec(unsigned long)', '0x08040725 (ds213_gabuino) MEMORY::Exec(unsigned long)', '0x00000000', '0x20004b1d', '0x08040bb1', '0x08041b03 (ds213_gabuino) st_usbfs_poll', '0x20004834 (ds213_gabuino) MEMORY::writeSum', '0x200049e0 (ds213_gabuino) command', '0x200049e0 (ds213_gabuino) command', '0x00000000'];
    stack = stack.map(f=>parseInt(f.substr(0,10)));
    this.decodeStack(stack)
      .then(() => this.fetchAndDecodeVariables());
  }

  frame()
  {
    this.offline = false;
    this.setCode(html_editor.getValue());

    // reset
    COMM._onReceive = COMM._defReceive;

    return dbg.initializeDebugger()
      .then( () => BIOS.frame())
      .then(f => {
        var arr = [];
        for (var i=0; i<f.length; i++) arr.push(f[i]); //"0x"+ ("00000000"+f[i].toString(16)).substr(-8));
        return arr;
      })
      .then((stack)=>this.decodeStack(stack))
      .then(() => this.fetchAndDecodeVariables());
  }

  setCode(code)
  {
    this.code = code;
  }

  decodeStack(stack)
  {
    var html = stack.map(p => dbg.decodeAddr(p))
      .filter(x => x)
      .filter(x => x.name.indexOf("st_usbfs_poll") == -1)
      .map(x => {
        if (x.line)
          return x.module + ": " + "<a href='#' onClick='"+this.instance+".jumpTo("+x.line+")'>" + x.name + " (" + x.line+")" /*+ " 0x"+x.addr.toString(16)*/ + "</a>"
        else
          return x.module + ": " + x.name + (x.offset ? " + " + x.offset : "");
      })
      .join("\n");

    $("#callstack").html(html)
  }

  fetchAndDecodeVariables()
  {
    var requests = Promise.resolve();
    this.variables = dbg.getVariables();
    for (var i in this.variables)
      requests = requests.then( ((v) => this.requestVariableBuffer(v).then(d => v.buffer = d)).bind(this, this.variables[i]) );
//      requests = requests.then( ((v) => this.requestVariable(v, "")).bind(this, this.variables[i]) );

    return requests.then(this.decodeVariables.bind(this))
  }

  formatVariable(v)
  {
    var format = this.getVariableFormatter(v.name);

    var aux = v.buffer;
    if (format)
      aux = eval(format)(aux);

    if (Array.isArray(aux) || ArrayBuffer.isView(aux))
      return aux.toString();

    aux = typeof(aux) == "object" ? JSON.stringify(aux) : aux;
//    aux = typeof(aux) == "object" ? aux.toString() : aux;
    return aux;
  }

  decodeVariables()
  {
    var html = this.variables.map(v => {
      var aux = this.formatVariable(v);
      return "<nobr>" +
        "<a href='#' onClick='"+this.instance+".setType(0x"+v.addr.toString(16)+")'>" + v.name + "</a> = " +
        "<a href='#' onClick='"+this.instance+".setValue(0x"+v.addr.toString(16)+")'>" + aux + "</a>" +
      "</nobr>"
    }).join("\n");
    $("#variables").html(html);
  }

  requestVariableBuffer(variable)
  {
    if (!this.offline)
    {
      return new Promise((resolve, reject) =>
      {
        this.timeout = setTimeout(()=>
        {
          // reset
          COMM._onReceive = COMM._defReceive;

          // second try
          BIOS.memRead(variable.addr, variable.len).then((data) =>
          {
            resolve(data);
          })
        }, 200);
        return BIOS.memRead(variable.addr, variable.len).then((data) =>
        {
          clearTimeout(this.timeout);
          resolve(data);
        })
      });
    }
    
    return new Promise((resolve, reject) =>
    {
      console.log("Debug mode!");
      // debug mode!
      if (variable.name == "mojPoint")
        return resolve(new Uint8Array([0, 0, 0, 7, 0, 0, 0, 8]));
      if (variable.name == "mojString")
        return resolve(new TextEncoder().encode("xxx\x00"));
      if (variable.name == "mojaPremenna")
        return resolve(new Uint8Array([0, 0, 0, 0x20]));
      resolve(new Uint8Array([]))
    });
  }

  setVariableFormatter(name, format)
  {
    window.localStorage.setItem("formatter_"+name, format);
  }

  getVariableFormatter(name)
  {
    return window.localStorage.getItem("formatter_"+name);
  }

  setVariableBuffer(variable, buffer)
  {
    if (!this.offline)
    {
//      console.log(buffer);
      return BIOS.memWrite(variable.addr, buffer)
        .then(() => BIOS.memRead(variable.addr, variable.len))
        .then((buf) => variable.buffer = buf );
    }
    //  return Promise.reject();

    // shoud write and readback!
    console.log(buffer);
    variable.buffer = buffer;
  }

  jumpTo(line)
  {
    html_editor.gotoLine(line, 0, true);
    return false;
  }

  setValue(varptr)
  {
    var variable = this.variables.find(v => v.addr == varptr);
    var formatter = this.getVariableFormatter(variable.name);
    var value;

    if (formatter && formatter.indexOf("string") != -1)
      value = new Parser().string(null, {zeroTerminated:true}).parse(variable.buffer);
    else if (variable.len == 1)
      value = new Parser().uint8().parse(variable.buffer);
    else if (variable.len == 2)
      value = new Parser().int16().parse(variable.buffer);
    else if (variable.len == 4)
      value = new Parser().int32().parse(variable.buffer);
    else
      value = variable.buffer.toString(); 

    value = prompt("Enter new value for " + variable.name, value);
    if (value == null)
      return;

    var buffer = null;
    if (formatter && formatter.indexOf("string") != -1)
      buffer = new TextEncoder().encode(value + "\x00");
    else if (variable.len == 1)
      buffer = new Uint8Array([parseInt(value)]);
    else if (variable.len == 2)
      buffer = new Uint8Array([parseInt(value), parseInt(value)>>8]);
    else if (variable.len == 4)
      buffer = new Uint8Array([parseInt(value), parseInt(value)>>8, parseInt(value)>>16, parseInt(value)>>24]);
    else
      buffer = new Uint8Array(value.split(",").map(v=>parseInt(v,10)))

    if (!buffer)
      throw "Formatting failed";
    this.setVariableBuffer(variable, buffer)
      .then( ()=>this.decodeVariables() )
    return false;
  }

  setType(varptr)
  {
    var help = `Formatting help:
Structures: buf => JSON.stringify(new Parser().uint32("x").uint32("y").parse(buf))
Strings: buf => "\\\"" + new Parser().string(null, {zeroTerminated:true}).parse(buf) + "\\\""
Integers: buf => "0x" + new Parser().uint32().parse(buf).toString(16)
`;
    console.log(help);
    var variable = this.variables.find(v => v.addr == varptr);
    var newformat = prompt("Enter formating string for " + variable.name + " of size " + 
      variable.len + " at 0x" + ("00000000"+variable.addr.toString(16)).substr(-8),
      this.getVariableFormatter(variable.name));

    if (newformat)
    {
      this.setVariableFormatter(variable.name, newformat);
      this.decodeVariables();
    }
    return false;
  }
};

var dbgui = new DebuggerUi("dbgui");
