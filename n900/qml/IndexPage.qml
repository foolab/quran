// -*- qml-mode -*-
import QtQuick 1.0

// TODO: use 2 columns.

Page {
        id: indexPage
        tools: toolBar

        Component {
                id: indexPageDelegate
                Rectangle {
                        color: mouse.pressed ? "steelblue" : "white"
                        MouseArea {
                                id: mouse
                                anchors.fill: parent
                                onClicked: {
                                        _settings.pageNumber = _data.pageNumberForSura(number.number);
                                        pageStack.pop();
                                }
                        }

                        width: view.width
                        height: verse.height * 2

                        NumberLabel {
                                color: "black"
                                id: number
                                number: index
                                width: 100
                                anchors.right: parent.right
                                anchors.rightMargin: 10
                                anchors.top: parent.top
                                anchors.topMargin: parent.height/4
                                horizontalAlignment: Text.AlignRight
                                font.pointSize: 24
                        }

                        Label {
                                id: verse
                                font.pointSize: 24
                                text: _data.fullSuraName(index);
                                font.bold: true
                                anchors.right: number.left
                                anchors.top: parent.top
                                anchors.topMargin: parent.height/4
                        }
                }
        }

        ListView {
                id: view
                clip: true
                model: _data.suraCount();
                anchors.top: parent.top
                anchors.topMargin: 16
                anchors.left: parent.left
                anchors.leftMargin: 16
                anchors.right: parent.right
                anchors.rightMargin: 16
                anchors.bottom: toolBar.top
                anchors.bottomMargin: 16
                // snapMode: ListView.SnapOneItem
                // TODO: this is not working :|
//                currentIndex: _data.firstSuraForPage(_settings.pageNumber);
                delegate: indexPageDelegate
        }

        ToolBar {
                id: toolBar
                ToolBarLayout {
                        ToolButton { icon: "general_backspace"; onClicked: pageStack.pop(); }
                }
        }

}
