// -*- qml -*-
import QtQuick 2.0

Row {
        x: 26
        width: parent.width - (x * 2)
        spacing: 5

        Component.onCompleted: layout();

        onChildrenChanged: layout();

        onWidthChanged: layout();

        function layout() {
                if (width == 0) {
                return;
                }

                var len = children.length;
                if (len == 0) {
                return;
                }

                if (len == 1) {
                children[0].width = width;
                return;
                }

                var sp = (len - 1) * spacing;
                var w = (width - sp)/len;
                var x = 0;
                for (x = 0; x < len; x++) {
                children[x].width = w;
                }
        }
}
