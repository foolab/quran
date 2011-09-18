// -*- qml -*-
import QtQuick 1.0

Row {
        id: toolBarLayout
        width: parent.width

        onChildrenChanged: layout();
        onWidthChanged: layout();

        function __enabledChildren() {
                var x = 0;
                var len = 0;

                for (x = 0; x < children.length; x++) {
                        if (children[x].enabled) {
                                ++len;
                        }
                }

                return len;
        }

        function layout() {
                var len = __enabledChildren();
                if (len == 0) {
                        spacing = 0;
                }
                else if (children == 1) {
                        spacing = (width - children[0].width) / 2;
                }
                else {
                        var totalWidth = 0;
                        var x = 0;
                        for (x = 0; x < children.length; x++) {
                                if (children[x].enabled) {
                                        totalWidth += children[x].width;
                                }
                        }

                        spacing = (width - (totalWidth))/(len - 1);
                }
        }
}
