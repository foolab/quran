// -*- c-mode -*-

var __cache = {};

var __stack = [];

// All functions have to be in a separate file because of this:
// https://bugreports.qt.nokia.com/browse/QTBUG-14986

function __cacheGet(id) {
  var comp = __cache[id];
  if (!comp) {
    comp = Qt.createComponent(id);
  }

  if (!comp) {
    console.log("Failed to load " + id);
  }
  else if (comp.status != Component.Ready) {
    console.log("Failed to load " + id + " " + comp.errorString());
    comp = undefined;
  }
  else {
    __cache[id] = comp;
  }

  return comp;
}

function push(page, ignored, immediate) {
  // TODO: kill point

  var comp = __cacheGet(page);
  if (!comp) {
    return;
  }

  var obj = comp.createObject(pageStack.parent);

  if (__stack.length > 0) {
    __stack[__stack.length - 1].state = "disabled";
  }

  __stack.push(obj);

  if (immediate) {
    obj.state = "shownNow";
  }
  else {
    obj.state = "shown";
  }

  pageStack.currentPage = obj;
}

function pop() {
  if (__stack.length <= 1) {
    console.log("Either page stack is empty or has only 1 page.");
    return;
  }

  var obj = __stack.pop();

  obj.state = "hidden";

  obj.destroy(1000);

  __stack[__stack.length - 1].state = "shown";

  pageStack.currentPage = __stack[__stack.length - 1];
}
