// -*- qml -*-
import QtQuick 2.2
import QtQuick.Controls 1.2 as Ctrls
import QuranAndroid 1.0

Ctrls.ApplicationWindow {
        id: rootWindow
        property alias pageStack: stack
        property alias initialPage: stack.initialItem

        // These are for the desktop
        width: 550
        height: 700

        property string __pageTitle

        visible: true

        Orientation {
            orientation: settings.orientation == 1 ? Orientation.OrientationPortrait : settings.orientation == 2 ? Orientation.OrientationLandscape : Orientation.OrientationAll
            Component.onCompleted: applyOrientation()
        }

        toolBar: Ctrls.ToolBar {
                id: title
                height: quranTheme.toolBarHeight
                width: parent.width

                Rectangle {
                    anchors.fill: parent
                    color: quranTheme.backgroundColor
                }

                ToolButton {
                        id: backButton
                        anchors.left: parent.left
                        height: parent.height
                        onClicked: pageStack.pop()
                        opacity: pageStack.depth > 1 ? 1.0 : 0.0
                        visible: opacity > 0

                        property bool _highlight: highlight || (mouse.pressed && mouse.containsMouse)
                        icon.source: _highlight ? "image://icon/back.png?" + quranTheme.buttonHighlightColor : "image://icon/back.png?" + quranTheme.buttonNormalColor

                        Behavior on opacity {
                                PropertyAnimation { duration: 400 }
                        }
                }

                Item {
                    id: separator
                    anchors.left: backButton.right
                    width: visible ? quranTheme.spacing : 0
                    height: parent.height
                    visible: backButton.visible
                    Behavior on width {
                        SequentialAnimation {
                            PauseAnimation { duration: 400 }
                            PropertyAnimation { duration: 400 }
                        }
                    }
                }

                QuranLabel {
                    id: label
                    anchors.left: separator.right
                    height: parent.height
                    text: stack.currentItem ? stack.currentItem.__pageTitle : ""
                    font.pixelSize: quranTheme.fontSizeLarge
                    verticalAlignment: Text.AlignVCenter
                    color: mouse.pressed && mouse.containsMouse ? quranTheme.highlightColor : quranTheme.textColor
                    visible: backButton.visible
                    opacity: backButton.opacity

                    MouseArea {
                        id: mouse
                        anchors.fill: parent
                        onClicked: pageStack.pop()
                    }
                }

                Behavior on height {
                        PropertyAnimation { duration: 200 }
                }

                Row {
                        id: toolBarItem
                        height: parent.height
                        anchors.top: parent.top
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: quranTheme.spacing
                        children: pageStack.currentItem.toolBar
                }

                MenuButton {
                        anchors.right: parent.right
                        menu: pageStack.currentItem.menu
                }
        }

        Ctrls.StackView {
                id: stack
                anchors.fill: parent

                // focus is needed for back key navigation
                focus: true

                Keys.onReleased: {
                    if (event.key === Qt.Key_Back && stack.depth > 1) {
                        stack.pop()
                        event.accepted = true
                    }
                }
        }
}
