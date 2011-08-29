// -*- qml-mode -*-
import QtQuick 1.0

Page {
        id: favoritesPage

        tools: toolBar

        Connections {
                target: _bookmarks
                onCleared: {
                        view.model.clear();
                        pageStack.pop();
                }
        }

        Component.onCompleted: {
            _bookmarks.bookmarkRemoved.connect(view.model.bookmarkRemoved);
        }

        Component.onDestruction: {
            _bookmarks.bookmarkRemoved.disconnect(view.model.bookmarkRemoved);
        }

        Component {
                id: sectionDelegate
                Rectangle {
                        width: view.width
                        color: _settings.highlightColor
                        height: title.height

                        Label {
                                anchors.right: parent.right
                                anchors.rightMargin: 20
                                id: title
                                font.pointSize: 26
                                font.family: _settings.fontFamily
                                horizontalAlignment: Text.AlignRight
                                verticalAlignment: Text.AlignVCenter
                                text: _data.fullSuraName(section)
                        }
                }
        }

        Component {
                id: favoritesPageDelegate

                Rectangle {
                        id: rect
                        width: parent.width
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: Math.max(rm.height, ayaText.height)
                        color: index % 2 ? Qt.lighter(_settings.highlightColor, 1.2) : Qt.lighter(_settings.highlightColor, 1.3)

                        ToolButton {
                                id: rm
                                color: parent.color
                                icon: theme.remove
                                anchors.top: parent.top
                                anchors.bottom: parent.bottom
                                anchors.right: parent.right
                                onClicked: {
                                        rmDialog.message = _data.text(sura, aya);
                                        rmDialog.open();
                                        rmDialog.accepted.connect(accepted);
                                        rmDialog.rejected.connect(rejected);
                                }

                                function accepted() {
                                        _bookmarks.remove(bookmark);
                                        rmDialog.accepted.disconnect(accepted);
                                        rmDialog.rejected.disconnect(rejected);
                                }

                                function rejected() {
                                        rmDialog.accepted.disconnect(accepted);
                                        rmDialog.rejected.disconnect(rejected);
                                }

                        }

                        Button {
                                id: ayaText
                                anchors.verticalCenter: parent.verticalCenter
                                border.width: 0
                                color: parent.color
                                clip: true
                                anchors.left: parent.left
                                anchors.right: rm.left
                                font.pointSize: 18
                                font.family: _settings.fontFamily
                                text: _data.text(sura, aya);
                                elide: Text.ElideRight
                                textAlignment: Text.AlignRight
                                onClicked: {
                                        pagePosition.setPosition(sura, aya);
                                        pageStack.pop();
                                }
                        }
                }
        }

        ListView {
                id: view
                clip: true
                model: FavoritesModel {}
                anchors.top: parent.top
                anchors.topMargin: 16
                anchors.left: parent.left
                anchors.leftMargin: 16
                anchors.right: parent.right
                anchors.rightMargin: 16
                anchors.bottom: toolBar.top
                anchors.bottomMargin: 16

                section.property: "sura"
                section.criteria: ViewSection.FullString
                section.delegate: sectionDelegate
                delegate: favoritesPageDelegate
        }

        ToolBar {
                id: toolBar
                ToolBarLayout {
                        ToolButton { icon: theme.pageBack; onClicked: pageStack.pop(); }

                        DialogButton {
                                id: clearButton
                                text: qsTr("Clear");
                                onClicked: clearDialog.open();
                                anchors.verticalCenter: parent.verticalCenter
                                // TODO: border or padding
                        }
                }
        }

        QueryDialog {
                id: rmDialog
                titleText: qsTr("Remove favorite?")
                acceptButtonText: qsTr("Yes")
                rejectButtonText: qsTr("No")
        }

        QueryDialog {
                id: clearDialog
                titleText: qsTr("Clear all favorites?")
                acceptButtonText: qsTr("Yes")
                rejectButtonText: qsTr("No")
                onAccepted: _bookmarks.clear();
        }

/*
  signal scrollTo(int sura, int aya);
*/
}
