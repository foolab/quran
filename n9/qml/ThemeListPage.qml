// -*- qml -*-
import QtQuick 1.0

Page {
	    id: themeListPage

        tools: toolBar

        TitleLabel {
                id: title
                width: parent.width
                anchors.top: parent.top
                text: qsTr("Themes")
        }

        Component {
                id: themeDelegate

                Rectangle {
                        width: view.width
                        height: label.height * 2
                        color: mouse.pressed ? colors.pressedColor : modelData == settings.theme ? colors.selectionBackgroundColor : colors.backgroundColor

                        Label {
                                id: label
                                x: 16
                                text: modelData
                                width: parent.width
                                anchors.verticalCenter: parent.verticalCenter
                                color: modelData == settings.theme ? colors.selectionTextColor : colors.textColor
                        }

                        MouseArea {
                                id: mouse
                                anchors.fill: parent
                                onClicked: settings.theme = modelData;
                        }
                }
        }

        ListView {
                id: view
                clip: true
                anchors.top: title.bottom
                anchors.bottom: toolBar.top
                anchors.left: parent.left
                anchors.leftMargin: 16
                anchors.right: parent.right
                anchors.rightMargin: 16
                model: _theme.themes();
                delegate: themeDelegate
        }

        ToolBar {
                id: toolBar
                ToolBarLayout {
                        ToolButton { icon: theme.pageBack; onClicked: pageStack.pop(); }
		        }
		}
}
