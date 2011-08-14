// -*- qml-mode -*-
import QtQuick 1.0

Row {
        id: toolBarLayout
        width: parent.width

        onChildrenChanged: layout();
        onWidthChanged: layout();

        function layout() {
                var len = children.length
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
                                totalWidth += children[x].width;
                        }

                        spacing = (width - (totalWidth))/(len - 1);
                }
        }
}
