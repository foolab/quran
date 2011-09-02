// -*- qml-mode -*-
import QtQuick 1.0

Dialog {
	    id: dialog

        faderOpacity: 0.3

        property int selectedChapter: -1
        property int selectedVerse: -1

        InfoBanner {
                id: error
                text: qsTr("Invalid selection");
                // TODO: something wrong with the animation
                parent: parent ? parent : dialog
        }

        title: Rectangle {
                id: title

                width: parent ? parent.width * 0.9 : undefined
                height: label.height

                anchors.horizontalCenter: parent.horizontalCenter
                color: "steelblue"

                Label {
                        id: label
                        width: parent.width
                        color: "white"
                        x: 10
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("Choose a verse");
                }
        }

        content: Row {
                width: title.width
                anchors.horizontalCenter: parent.horizontalCenter

                Component.onCompleted: parent.anchors.topMargin = 0; // HACK

                Rectangle {
                        width: parent.width * 0.4
                        height: dialog.height * 0.75

                        ListView {
                                id: verse
                                clip: true
                                width: parent.width
                                height: parent.height

                                model: _data.suraSize(selectedChapter);

                                Connections {
                                        target: dialog
                                        onSelectedChapterChanged: {
                                                selectedVerse = -1;
                                        }
                                }

                                delegate: Rectangle {
                                        width: verse.width
                                        height: 70
                                        property bool selected: selectedVerse == index
                                        color: selected || mouse.pressed ? "steelblue" : "white"

                                        NumberLabel {
                                                width: parent.width
                                                height: parent.height
                                                anchors.centerIn: parent
                                                number: modelData
                                                color: "black"
                                        }

                                        MouseArea {
                                                id: mouse
                                                anchors.fill: parent
                                                onClicked: selectedVerse = index;
                                        }
                                }
                        }
                }

                ListView {
                        id: chapter
                        clip: true
                        width: parent.width * 0.6
                        height: dialog.height * 0.75

                        model: 114
                        delegate: IndexPageCell {
                                width: chapter.width
                                sura: index
                                selected: index == selectedChapter
                                onClicked: selectedChapter = index;
                        }
                }
        }

        buttons: DialogButton {
                text: qsTr("Go");
                width: title.width
                anchors.horizontalCenter: parent.horizontalCenter
                Component.onCompleted: parent.anchors.topMargin = 0; // HACK

                onClicked: {
                        if (selectedVerse == -1 || selectedChapter == -1) {
                                error.show();
                                return;
                        }

                        pagePosition.setPosition(selectedChapter, selectedVerse);
                        _settings.pageNumber = _data.pageNumberForSuraAndAya(selectedChapter, selectedVerse);
                        accept();
                        pageStack.pop();
                }
        }
}
