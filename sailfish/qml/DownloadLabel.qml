// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

BackgroundItem {
        id: downloadLabel

        property alias showInstalled: installed.visible
        property alias showProgress: slider.visible
        property alias text: label.text
        property int progress
        onProgressChanged: slider.value = progress / 100.0
        property Component menuComponent
        property Item menu
        property Item visualParent

        width: parent.width
        height: menu && menu.parent == downloadLabel ? menu.height + Theme.itemSizeLarge : Theme.itemSizeLarge

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
                anchors.verticalCenter: label.verticalCenter
                anchors.right: parent.right
        }

        Label {
                id: label
                anchors {
                        top: parent.top
                        right: installed.left
                        rightMargin: theme.marginSmall
                        left: parent.left
                        leftMargin: theme.marginSmall
                }

                font.family: Theme.fontFamily
                height: Theme.itemSizeLarge
                wrapMode: Text.WordWrap
                color: Theme.primaryColor
                truncationMode: TruncationMode.Fade
                verticalAlignment: Text.AlignVCenter
        }

        ProgressCircle {
                id: slider
                visible: showProgress
                anchors.centerIn: installed
        }
}
