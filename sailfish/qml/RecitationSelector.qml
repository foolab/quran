// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0
import Quran 1.0

Dialog {
        id: dialog

        DialogHeader {
                id: header
                title: qsTr("Choose recitation")
                acceptText: title
        }

        SilicaListView {
                id: view

                model: RecitationModel {
                        recitations: _recitations
                }

                anchors {
                        top: header.bottom
                        bottom: parent.bottom
                        right: parent.right
                        left: parent.left
                }

                delegate: BackgroundItem {
                        onClicked: {
                                recitationsManager.changeRecitation(recitationId)
                                pageStack.pop()
                        }

                        anchors {
                                left: parent.left
                                leftMargin: 16
                                right: parent.right
                                rightMargin: 16
                        }

                        height: Theme.itemSizeSmall

                        Label {
                                anchors.fill: parent
                                wrapMode: Text.WordWrap
                                truncationMode: TruncationMode.Fade
                                text: name
                                font.family: Theme.fontFamily
                                color: _recitations.current == recitationId ? Theme.highlightColor : Theme.primaryColor
                        }
                }
        }
}
