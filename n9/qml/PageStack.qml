// -*- qml -*-
import QtQuick 1.0

import "PageStack.js" as Stack

Item {
        id: pageStack

	    property Item currentPage: null

        function pop() {
                Stack.pop();
        }

        function push(page, ignored, immediate) {
                Stack.push(page + ".qml", ignored, immediate);
        }

        function replace(page, ignored, immediate) {
                Stack.replace(page + ".qml", ignored, immediate);
        }
}
