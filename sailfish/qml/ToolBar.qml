// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

Rectangle {
        border.color: color
        border.width: 5
	    width: parent.width
	    height: Theme.itemSizeSmall

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        // TODO: this creates a dependency non-components stuff
        Image {
                anchors.fill: parent
                source: "image://icon/" + theme.toolBarBackground
        }
}
