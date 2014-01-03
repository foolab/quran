// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

IconButton {
        id: button
        property string image
        width: Theme.itemSizeSmall
        height: Theme.itemSizeSmall

        onImageChanged: icon.source = "image://theme/" + image
}
