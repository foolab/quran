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
            color: quranTheme.colors.background
        }

        ToolButton {
            id: backButton
            anchors.left: parent.left
            height: parent.height
            onClicked: pageStack.pop()
            opacity: pageStack.depth > 1 ? 1.0 : 0.0
            visible: opacity > 0

            highlight: (pressed && containsMouse) || (mouse.pressed && mouse.containsMouse)
            icon: "image://icon/back.png"

            Behavior on opacity {
                PropertyAnimation { duration: quranTheme.animations.slow }
            }
        }

        Item {
            id: separator
            anchors.left: backButton.right
            width: visible ? quranTheme.sizes.spacing : 0
            height: parent.height
            visible: backButton.visible
            Behavior on width {
                SequentialAnimation {
                    PauseAnimation { duration: quranTheme.animations.slow }
                    PropertyAnimation { duration: quranTheme.animations.slow }
                }
            }
        }

        QuranLabel {
            id: label
            anchors.left: separator.right
            height: parent.height
            text: stack.currentItem ? stack.currentItem.__pageTitle : ""
            font.pixelSize: quranTheme.fonts.large
            verticalAlignment: Text.AlignVCenter
            color: (backButton.pressed && backButton.containsMouse) || (mouse.pressed && mouse.containsMouse) ? quranTheme.colors.primaryHighlight : quranTheme.colors.primary
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
                anchors.verticalCenter: parent.verticalCenter
                property MenuAction _action
                icon: _action.icon
                onClicked: _action.clicked()
            }
        }

        onActionsChanged: {
            if (stack.depth > 1 && actions != null && actions.length > 0) {
                for (var x = 0; x < actions.length; x++) {
                    toolTemplate.createObject(toolsContainer,
                        {"_action": actions[x]})
                }
            } else {
                for (var x = 0; x < toolsContainer.children.length; x++) {
                    toolsContainer.children[x].destroy()
                }
            }
        }

        Row {
            id: toolsContainer
            spacing: quranTheme.sizes.spacing
            anchors {
                right: parent.right
                top: parent.top
                bottom: parent.bottom
            }
        }
    }

    Ctrls.StackView {
        id: stack
        clip: true
        anchors {
            top: stack.depth == 1 ? parent.top : title.bottom
            bottom: stack.currentItem && stack.currentItem.toolBar ? stack.currentItem.toolBar.top : parent.bottom
            left: parent.left
            right: parent.right
        }
    }

    KeyFilter {
        enabled: stack.depth > 1
        onBackTriggered: {
            stack.pop()
        }
    }
}
