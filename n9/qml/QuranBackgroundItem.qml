// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.0

MouseArea {
        id: mouse
        Image {
                anchors.fill: parent
                visible: mouse.pressed
                source: theme.inverted ? "image://theme/meegotouch-panel-inverted-background-pressed" : "image://theme/meegotouch-panel-background-pressed"
        }
}
