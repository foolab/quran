// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

BackgroundItem {
        id: downloadLabel

        property alias showInstalled: installed.visible
        property alias showProgress: slider.visible
        property alias text: label.text
        property alias progress: slider.value

        property Component menuComponent
        property Item menu
        property Item visualParent

        width: parent.width
        property real itemHeight: showProgress ? Theme.itemSizeMedium : Theme.itemSizeSmall
        height: menu && menu.parent == downloadLabel ? menu.height + itemHeight : itemHeight

        onPressAndHold: {
                if (menuComponent && !menu) {
                        menu = menuComponent.createObject(visualParent)
                }

                if (menu) {
                        menu.show(downloadLabel)
                }
        }

        GlassItem {
                id: installed
                visible: showInstalled
                y: (Theme.itemSizeLarge - height) / 2
                anchors.right: parent.right
        }

        Label {
                id: label
                anchors {
                        top: parent.top
                        right: installed.left
                        rightMargin: 10
                        left: parent.left
                        leftMargin: 10
                }

                // TODO:
                height: itemHeight
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
                anchors {
                        top: label.bottom
                        right: installed.left
                        rightMargin: 10
                        left: parent.left
                        leftMargin: 10
                }
        }
}
