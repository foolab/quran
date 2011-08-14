// -*- qml-mode -*-
import QtQuick 1.0

import "PageStack.js" as Stack

Item {
        id: pageStack

        function pop() {
                Stack.pop();
        }

        function push(page, ignored, immediate) {
                Stack.push(page + ".qml", ignored, immediate);
        }
}
