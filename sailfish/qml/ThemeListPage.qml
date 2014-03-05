// -*- qml -*-
import QtQuick 2.0

QuranPage {
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
                        color: mouse.pressed ? _colors.pressedColor : modelData == _settings.theme ? _colors.selectionBackgroundColor : _colors.backgroundColor

                        Label {
                                id: label
                                x: 16
                                text: modelData
                                width: parent.width
                                anchors.verticalCenter: parent.verticalCenter
                                color: modelData == _settings.theme ? _colors.selectionTextColor : _colors.textColor
                        }

                        MouseArea {
                                id: mouse
                                anchors.fill: parent
                                onClicked: _settings.theme = modelData;
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
