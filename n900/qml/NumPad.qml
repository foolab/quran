// -*- qml-mode -*-
import QtQuick 1.0

Grid {
	    columns: 3
        width: 180

        signal insert(int val);
        signal backspace

        Repeater {
                model: 9
                Button {
                        width: parent.width / 3
                        height: 60
                        text: _formatter.number(index + 1);
                        onClicked: insert(index + 1);
                }
        }

        Button {
                width: parent.width / 3
                height: 60
                text: _formatter.number(0);
                onClicked: insert(0);
        }

        Button {
                width: (parent.width / 3) * 2
                height: 60
                text: "<<"
                onClicked: backspace();
        }
}
