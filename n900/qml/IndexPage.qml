// -*- qml -*-
import QtQuick 1.0

// TODO: use positionViewAtIndex ?
Page {
        id: indexPage

        tools: toolBar

        property Component partDialogComponent: null
        property Item partDialog: null

        property Component pageDialogComponent: null
        property Item pageDialog: null

        property Component verseDialogComponent: null
        property Item verseDialog: null

        function showPartDialog() {
                if (!partDialogComponent) {
                        partDialogComponent = Qt.createComponent("PartSelectionDialog.qml");
                        partDialog = partDialogComponent.createObject(indexPage);
                }

                partDialog.open();
        }

        function showPageDialog() {
                if (!pageDialogComponent) {
                        pageDialogComponent = Qt.createComponent("PageSelectionDialog.qml");
                        pageDialog = pageDialogComponent.createObject(indexPage);
                }

                pageDialog.open();
        }

        function showVerseDialog() {
                if (!verseDialogComponent) {
                        verseDialogComponent = Qt.createComponent("VerseSelectionDialog.qml");
                        verseDialog = verseDialogComponent.createObject(indexPage);
                }

                verseDialog.open();
        }

        TitleLabel {
                id: title
                width: parent.width
                anchors.top: parent.top
                text: qsTr("Index")
        }

        Component {
                id: indexPageDelegate
                Item {
                        width: view.width
                        height: Math.max(left.height, right.height);
                        IndexPageCell {
                                id: left
                                sura: index + 57
                                onClicked: {
                                        pagePosition.setPosition(sura, 0);
                                        pageStack.pop();
                                }
                        }

                        IndexPageCell {
                                id: right
                                anchors.left: left.right
                                sura: index
                                onClicked: {
                                        pagePosition.setPosition(sura, 0);
                                        pageStack.pop();
                                }
                        }
                }
        }

        ListView {
                id: view
                clip: true
                model: _data.suraCount()/2;
                anchors.top: title.bottom
                anchors.left: parent.left
                anchors.leftMargin: 16
                anchors.right: parent.right
                anchors.rightMargin: 16
                anchors.bottom: toolBar.top
                delegate: indexPageDelegate
        }

        ToolBar {
                id: toolBar
                ToolBarLayout {
                        ToolButton { icon: theme.pageBack; onClicked: pageStack.pop(); }
                        ToolButton { icon: theme.verse; onClicked: showVerseDialog(); }
                        ToolButton { icon: theme.page; onClicked: showPageDialog(); }
                        ToolButton { icon: theme.part; onClicked: showPartDialog(); }
                }
        }
}
