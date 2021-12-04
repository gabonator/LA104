
var editor_markers = [];
var annotations = [];
function clearError()
{            
  annotations = [];  
  for (var i=0; i<editor_markers.length; i++)
    html_editor.getSession().removeMarker(editor_markers[i]);
  editor_markers = [];                                                                         
  html_editor.getSession().setAnnotations([]);
}

function addError(line, column, msg)
{
  annotations.push({
  row: line-1,
  column: column,
  text: msg,
  type: "error" // also warning and information
  });

  //editor_markers.push( html_editor.session.addMarker(new Range(8, 0, 9, 0),
  //"activeBreakpoint", "line") );

  editor_markers.push(html_editor.getSession().addMarker(new aceRange(line-1, 0, line-1, 200), "errorHighlight", "line", true));
}

function handleError(stderr)
{
//  console.log(stderr);
  annotations = [];
  var errors = stderr.split("\n");
  for (var i=0; i<errors.length; i++)
  {
    var error = errors[i];
    var matches = error.match("\\<stdin\\>:(\\d+):(\\d+): (.*)$");
    if (matches)
    {
//      console.log(error);
      addError(parseInt(matches[1]), parseInt(matches[2]), matches[3]);
    }
  }
  html_editor.getSession().setAnnotations(annotations);
}

function higlightLine(l)
{
  clearError ();
  annotations = [];
  if (l)
    addError(l, 0, "bla");
  html_editor.getSession().setAnnotations(annotations);
}


var html_editor;
var aceRange;
function initAceEditor() {
  ace.require("ace/ext/language_tools");
  aceRange = ace.require("ace/range").Range

  html_editor = ace.edit("my_html");
  html_editor.setTheme("ace/theme/xcode");
  html_editor.getSession().setMode("ace/mode/c_cpp");
  html_editor.setFontSize("15px") ;
  html_editor.setPrintMarginColumn(false);
//  html_editor.session.setValue($("#my_html_hidden").text());
  html_editor.setOptions({
  //maxLines: 20,
      enableBasicAutocompletion: true
  });

/*
var langTools = ace.require("ace/ext/language_tools");
var rhymeCompleter = {
        getCompletions: function(editor, session, pos, prefix, callback) {

console.log(prefix);

          callback(null, {name: "blabotat", value: "blabotat", score: 10, meta: "rhyme"})

        }
    }
    langTools.addCompleter(rhymeCompleter);
*/
  html_editor.on("guttermousedown", function(e) {
      var target = e.domEvent.target; 
      if (target.className.indexOf("ace_gutter-cell") == -1)
          return; 
      if (!html_editor.isFocused()) 
          return; 
    
  //  console.log(e.clientX > 25 + target.getBoundingClientRect().left);  
  //  console.log(e.clientX, 25 + target.getBoundingClientRect().left);
    
      if (e.clientX > 50 + target.getBoundingClientRect().left) 
          return; 

      e.stop();

      var breakpoints = e.editor.session.getBreakpoints(/*newRow, 0*/);
      if(typeof breakpoints[e.getDocumentPosition().row] !== typeof undefined)
      {
         // TODO! clear breakpoint
         e.editor.session.clearBreakpoint(row);
      } else 
      {

        var row = e.getDocumentPosition().row + 1; // returns zero based line number
  //      uisync( ()=>


        debuggerTryBreakpoint(row).then( (row) =>
        {
          console.log("set break at row " + row);
          if (row == -1)
            return;
                       
          row--; // zero based line                              

    //      var row = e.getDocumentPosition().row;
  //        if(typeof breakpoints[row] === typeof undefined)
              e.editor.session.setBreakpoint(row);
  //        else
  //            e.editor.session.clearBreakpoint(row);
        })
      }

  });
}

function editorClearBreakpoint(row)
{
  html_editor.session.clearBreakpoint(row-1);
}

document.addEventListener("DOMContentLoaded", function(){
  initAceEditor()
});


function store_code()
{
  var code = html_editor.getValue();
  window.localStorage.setItem("lastCode", code);
}

function load_code()
{
  var last = window.localStorage.getItem("lastCode");
  if (last)
    html_editor.setValue(last, -1);
  else
    html_editor.setValue(`#include <library.h>

int main(void)
{
    while (!BIOS::KEY::GetKey())
    {
        BIOS::DBG::Print("Hello! ");
        BIOS::SYS::DelayMs(250);
    }
    
    return 0;
}`, -1);
   
}
