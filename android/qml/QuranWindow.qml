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
import QtQuick.Controls 2.2 as Ctrls
import QuranAndroid 1.0

Item {
    id: rootWindow
    property alias pageStack: stack
    property alias initialPage: stack.initialItem

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
        visible: stack.depth > 1
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

        property var actions: pageStack.currentItem.menu ? pageStack.currentItem.menu.actions : null
        Component {
            id: toolTemplate
            ToolButton {
                property MenuAction _action
            }
        }

        onActionsChanged: {
            if (stack.depth > 1 && actions != null && actions.length > 0) {
                for (var x = 0; x < actions.length; x++) {
                    var obj = toolTemplate.createObject(toolsContainer,
                        {"_action": actions[x],
                         "icon.source": actions[x].icon})
                    obj.clicked.connect(function() { obj._action.clicked() })
                }
            } else {
                for (var x = 0; x < toolsContainer.children.length; x++) {
                    toolsContainer.children[x].destroy()
                }
            }
        }

        Row {
            id: toolsContainer
            anchors.right: parent.right
        }
    }

    Ctrls.ToolBar {
        id: toolBar
        height: visible ? quranTheme.toolBarHeight : 0
        visible: stack.depth == 1
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        Rectangle {
            anchors.fill: parent
            color: quranTheme.backgroundColor
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
            top: stack.depth == 1 ? parent.top : title.bottom
            bottom: stack.depth == 1 ? toolBar.top : parent.bottom
            left: parent.left
            right: parent.right
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
