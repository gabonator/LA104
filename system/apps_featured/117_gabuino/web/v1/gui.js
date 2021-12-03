class ResponseUi {
  constructor()
  {
    this.self = this;
    this.lastCode = null;
    this.isConnected = false;
    this.isRunning = false;

    $('#_run').on('click', this.onRun.bind(this.self));
    $('#_stop').on('click', this.onStop.bind(this.self));
    $('#_inspect').on('click', this.onInspect.bind(this.self));
    $('#_resume').on('click', this.onResume.bind(this.self));
    $('#_screenshot').on('click', this.onScreenshot.bind(this.self));
    $('#_clear').on('click', this.onClear.bind(this.self));
  }

  default()
  {
    if (this.isConnected)
      $("#_run").removeClass("btn-outline-success").addClass("btn-success");
    else
      $("#_run").removeClass("btn-success").addClass("btn-outline-success");

    if (this.isRunning)
      $("#_stop").removeClass("btn-outline-success").addClass("btn-success");
    else
      $("#_stop").removeClass("btn-success").addClass("btn-outline-success");
    $("#_inspect").removeClass("btn-success").addClass("btn-outline-success");
    $("#_breakpoint").removeClass("btn-success").addClass("btn-outline-success");
  }

  compilationStarted()
  {
    $("#_run").removeClass("btn-success").addClass("btn-outline-success");
  }
  compilationFinished()
  {
    $("#_stop").removeClass("btn-outline-success").addClass("btn-success");
    $("#_inspect").removeClass("btn-outline-success").addClass("btn-success");
  }
  compilationFailed()
  {
    $("#_run").removeClass("btn-outline-success").addClass("btn-success");
    $("#_inspect").removeClass("btn-success").addClass("btn-outline-success");
  }
  breakpointHit()
  {
    $("#_resume").removeClass("btn-outline-success").addClass("btn-success");
  }
  breakpointResumed()
  {
    $("#_resume").removeClass("btn-success").addClass("btn-outline-success");
  }
  disconnected()
  {
    this.isConnected = false;
    $("#_run").removeClass("btn-success").addClass("btn-outline-success");
  }
  connected()
  {
    this.isConnected = true;
    $("#_run").removeClass("btn-outline-success").addClass("btn-success");
  }
  //

  onRun(event)
  {
    this.compilationStarted();
    store_code();

    var newCode = html_editor.getValue();
    dbg.setCode(newCode);

    if (typeof(this.lastCode) != "undefined" && this.lastCode == newCode)
    {
      // reset bss!
      $("#_debugText").text("");
      run().then( ()=>
      {
        this.compilationFinished();
      });

      return;
    }
    this.lastCode = newCode; // only on successfull build!

    var tick0 = (new Date).getTime();
    var tick1, tick2, tick3, tick4;

    compile()
    .then( (elf) => {
      tick1 = (new Date).getTime();
      $("#_compilationText").text("Compilation took " + (tick1-tick0) + "ms.\n");  
      return elf;
    })
    .then( elf => processElf(elf))
    .then( (stats) => {
      tick2 = (new Date).getTime();
      $("#_compilationText").append("Processing took " + (tick2-tick1) + "ms.\n");  
      $("#_debugText").text("");
    })
    .then( () => resolveImports())
    .then( () => {
      tick3 = (new Date).getTime();
      $("#_compilationText").append("Resolving relocations took " + (tick3-tick2) + "ms.\n");  
      $("#_debugText").text("");
    })
    .then( () => flash() )
    .then( () =>
    {
      tick4 = (new Date).getTime();
      $("#_compilationText").append("Flashing took " + (tick4-tick3) + "ms.\n");  
      $("#_compilationText").append("Application size " + globalBlob.length + " bytes\n");  
    })
    .then( () => run() )
    .then( () =>
    {
      this.running = true;
      this.compilationFinished();
    })
    .catch( stderr =>
    {
      var errors = stderr.split("<stdin>:").join("");
  //    errors = errors.split("\r").join("<br>");
      $("#_compilationText").html("Failed: <pre>"+errors+"</pre>");  
      this.compilationFailed();
    });
  }

  onStop()
  {
    BIOS.stop()
    this.default();
  }

  onInspect()
  {
    $("#_debugtools").removeClass("d-none");
    dbgui.frame();
  }
 
  onResume()
  {
    higlightLine(-1);
    uisync(()=>revertBreakpoint(trapped).then(()=>this.breakpointResumed()))
  }

  onScreenshot()
  {
    screenshot();
  }

  onClear()
  {
    $("#_debugText").html("");
  }

  stopped()
  {
    this.running = false;
    this.default();
  }
};

var responseUi = new ResponseUi();

function screenshot()
{
  BIOS.screenshot().then(data => 
  {
    var offscreenCanvas = document.createElement('canvas');
    offscreenCanvas.style = "border:3px solid #000000; width:300px; height:300px"
    offscreenCanvas.width = 320;
    offscreenCanvas.height = 240;
    var context = offscreenCanvas.getContext('2d');
    context.fillStyle = "red";
    context.fillRect(10,10,100,100);

    var imagedata = context.createImageData(320, 240);
    var  j=0;
      for (var x=0; x<320; x++)
    for (var y=0; y<240; y++)
      {
        var rgb = data[j++];
        rgb = ((rgb << 8) | (rgb >> 8)) & 0xffff;
        var i = ((239-y)*320+x)*4;
        imagedata.data[i++] = (((rgb)&0x1f)<<3);
        imagedata.data[i++] = ((((rgb)>>5)&0x3f)<<2);
        imagedata.data[i++] = ((((rgb)>>11)&0x1f)<<3);
        imagedata.data[i++] = 255;
      }
    context.putImageData(imagedata, 0, 0);

    var element = "<img src='"+offscreenCanvas.toDataURL()+"'>";
    _DBGPRINT(element);
  });
}

function uisync(method)
{
  setTimeout(method, 10);
}

var running = false;
$('#_connect').on('click', function(event) {
//  event.preventDefault(); // To prevent following the link (optional)
  COMM.doConnect();
});

COMM.setDisconnected = () => { 
  $("#_connect").text("Connect"); 
  $("#_connectText").text("Disconnected"); 
  $("#_connect").removeClass("btn-outline-secondary").addClass("btn-secondary");
  responseUi.disconnected();
};
COMM.setConnected = () => { 
  $("#_connect").text("Disconnect"); 
  $("#_connectText").text("Connected"); 
  $("#_connect").removeClass("btn-secondary").addClass("btn-outline-secondary");
  responseUi.connected();
};
COMM.setConnecting = () => { 
  $("#_connect").text("Connecting..."); 
  $("#_connectText").text(""); 
  $("#_connect").removeClass("btn-secondary").addClass("btn-outline-secondary");
};
COMM.setConnectFailed = (err) => { 
  $("#_connect").text("Connect"); 
  $("#_connectText").text(err ? err : "Connection failed"); 
  $("#_connect").removeClass("btn-outline-secondary").addClass("btn-secondary");
  responseUi.disconnected();
};
COMM.setNoDevices = () => {
  $("#_connect").text("Connect"); 
  $("#_connectText").text("Not connected"); 
  $("#_connect").removeClass("btn-outline-secondary").addClass("btn-secondary");
  responseUi.disconnected();
};

function _DBGPRINT(msg)
{  
  $("#_debugText").append(msg);
}

var trapped = null;
function _DBGEVENT(e, arg)
{
  switch (e)
  {
    case 1: 
      responseUi.stopped();
      running = false; 
      console.log("Program finished"); 
      break;
    case 2: 
      trapped = arg; 
      console.log("Program trapped at 0x" + arg.toString(16)); 
      higlightLine(dbg.findLine(arg));
      responseUi.breakpointHit();
    break;
    default: console.log("Unrecognized event " + e);
  }
}

document.addEventListener('DOMContentLoaded', () => {
  responseUi.default();
  load_code()  

  for (var i in examples)
    $("#_examples").append('<a class="dropdown-item" href="#">'+i+'</a>');

  var elements = $("#_examples").children("a"); //.dropdown-item");
  for (var i=0; i<elements.length; i++)
  {
    elements[i].addEventListener('click', (event) => {
      html_editor.setValue(examples[event.target.innerText], -1)
    });
  };

});

