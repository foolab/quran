// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0
import Quran 1.0

// TODO: use positionViewAtIndex ?
QuranPage {
        id: indexPage

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

        function showVerseDialog(chapter) {
                if (!verseDialogComponent) {
                        verseDialogComponent = Qt.createComponent("VerseSelectionDialog.qml");
                        verseDialog = verseDialogComponent.createObject(indexPage);
                }

                verseDialog.chapter = chapter;

                verseDialog.open();
        }

        Component {
                id: indexPageDelegate

                IndexPageCell {
                        id: cell
                        sura: index
                        width: view.width
                        onClicked: {
                                pagePosition.setPosition(sura, 0)
                                pageStack.pop();
                        }

                        ToolButton {
                                image: theme.verse
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                onClicked: showVerseDialog(sura);
                        }
                }
        }

        SilicaListView {
                id: view
                model: _data.suraCount();
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: 16
                anchors.right: parent.right
                anchors.rightMargin: 16
                anchors.bottom: toolBar.top
                delegate: indexPageDelegate
                header: PageHeader {
                        width: view.width
                        title: qsTr("Index")
                }
        }

        ToolBar {
                id: toolBar
                ToolBarLayout {
                        anchors.fill: parent
                        Component.onCompleted: setItems(children);
                        ToolButton { image: theme.page; onClicked: showPageDialog(); }
                        ToolButton { image: theme.part; onClicked: showPartDialog(); }
                        ToolButton { image: theme.search; onClicked: pageStack.replace("SearchPage"); }
                }
        }
}
