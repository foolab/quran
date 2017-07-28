// -*- qml -*-
/*
 * Copyright (c) 2011-2017 Mohammed Sameer <msameer@foolab.org>.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.2
import QtQuick.Controls 1.2 as Ctrls
import QuranAndroid 1.0

Item {
    id: rootWindow
    property alias pageStack: stack
    property Component initialPage

    // We need to set a size otherwise we end up creating all the delegates
    // upon startup
    width: 550
    height: 700

    property string __pageTitle

    visible: true
    AndroidSupport {
        orientation: settings.orientation == 1 ? AndroidSupport.OrientationPortrait : settings.orientation == 2 ? AndroidSupport.OrientationLandscape : AndroidSupport.OrientationAll
        Component.onCompleted: applyOrientation()
    }

    Ctrls.ToolBar {
        id: title
        height: quranTheme.toolBarHeight

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

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
            color: (backButton.pressed && backButton.containsMouse) || (mouse.pressed && mouse.containsMouse) ? quranTheme.highlightColor : quranTheme.textColor
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
            children: pageStack.currentItem ? pageStack.currentItem.toolBar : 0
        }

        MenuButton {
            anchors.right: parent.right
            menu: pageStack.currentItem ? pageStack.currentItem.menu : null
        }
    }

    Ctrls.StackView {
        id: stack
        clip: true
        anchors {
            top: title.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        // HACK: Something is really strange here. Initially StackView is created
        // with 0 as width and a negative height. Assigning the MainPage directly
        // to StackView initialItem leads to our main ListView creating all of
        // its delegates thus slowing down application startup.
        // It's also strange that if we push the component directly then we have the
        // same issue. Thus we create the the object ourselves and pass the proper
        // width and height to make sure ListView behaves and we push that created
        // object which makes everybody happy!

        property bool __done: width > 0 && height > 0
        on__DoneChanged: {
            if (__done) {
                var obj = rootWindow.initialPage.createObject(stack,
                    {width: stack.width, height: stack.height})
                push(obj)
            }
        }

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
