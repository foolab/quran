// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

BackgroundItem {
        id: downloadLabel
        property bool showProgress: true
        property bool showInstalled: true

        property alias text: label.text
        property alias progress: slider.value

        property Item menu

        width: parent.width
        height: menu && menu.active ? menu.height + Theme.itemSizeLarge : Theme.itemSizeLarge

        onPressAndHold: {
                if (menu) {
                        menu.show(downloadLabel)
                }
        }

        ToolButton {
                id: installed
                image: theme.downloaded
                visible: showInstalled
                y: (Theme.itemSizeLarge - height) / 2
                anchors.right: parent.right
        }

        Label {
                id: label
                anchors.top: parent.top
                anchors.right: installed.left
                anchors.rightMargin: 10
                anchors.left: parent.left
                anchors.leftMargin: 10
                // TODO:
                height: slider.visible ? Theme.itemSizeSmall : Theme.itemSizeMedium
                wrapMode: Text.WordWrap
                color: Theme.primaryColor
                truncationMode: TruncationMode.Fade
        }

        ProgressBar {
                // TODO: not positioned correctly
                id: slider
                visible: showProgress
                minimumValue: 0
                maximumValue: 100
                anchors.top: label.bottom
                anchors.right: installed.left
                anchors.rightMargin: 10
                anchors.left: parent.left
                anchors.leftMargin: 10
        }
}
