
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

      var row = e.getDocumentPosition().row;
      e.stop();

      debuggerTryBreakpoint(row).then( (row) =>
      {
console.log("set row " + row);
        if (row == -1)
          return;
                                                           
        var breakpoints = e.editor.session.getBreakpoints(/*newRow, 0*/);

  //      var row = e.getDocumentPosition().row;
        if(typeof breakpoints[row] === typeof undefined)
            e.editor.session.setBreakpoint(row);
        else
            e.editor.session.clearBreakpoint(row);
      });

  });
}

// the function is called at the end of a function that
// includes a getJSON() request to a Python script which
// queries a MongoDB database and json dumps the results.  
// eg loadMyContent() {
// getJSON() and return content

document.addEventListener("DOMContentLoaded", function(){
  initAceEditor()
});
// }

// Problems when using div instead of xmp:  

// In Firebug I can *see* the required HTML in the 'response'
// and 'json' tabs ie:
// <div id="acey_html_hidden"><html>test</html></div>
// but the Ace edior is just showing:
// test

// In Firebug's HTML view of the hidden div, when using <div> tags, the <html> tags have been stripped out.  
//
//When using <xmp> tags the <html> tags have not been stripped out.  
