// -*- qml -*-
import QtQuick 1.0

Button {
        property int index: 1
        width: parent.width/3
        height: 60
        text: formatter.number(index);
        onClicked: numPad.insert(index);
}
