// Copyright 2016 The Emscripten Authors.  All rights reserved.
// Emscripten is available under two separate licenses, the MIT license and the
// University of Illinois/NCSA Open Source License.  Both these licenses can be
// found in the LICENSE file.

var LibraryApp = {
  js_init: function() { return window.miniApp.init(); },
  js_getKey: function() { return window.miniApp.getKey(); },
  js_getPixel: function(x, y) { return window.miniApp.getPixel(); },
  js_loop: function() { return window.miniApp.loop(); },
  js_running: function() { return window.miniApp.running(); },
  js_setPixel: function(x, y, rgb) { return window.miniApp.setPixel(x, y, rgb); },
  js_ticks: function() { return window.miniApp.ticks(); }
};

mergeInto(LibraryManager.library, LibraryApp);


