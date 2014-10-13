// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranPage {
        QuranListView {
                anchors.fill: parent

                header: QuranPageHeader {
                        width: parent.width
                        title: qsTr("Choose recitation")
                }

                model: RecitationModel {
                        recitations: _recitations
                }

                delegate: QuranBackgroundItem {
                        onClicked: {
                                recitationsManager.changeRecitation(recitationId)
                                popPage()
                        }

                        anchors {
                                left: parent.left
                                leftMargin: theme.marginMedium
                                right: parent.right
                                rightMargin: theme.marginMedium
                        }

                        height: theme.itemSizeSmall

                        QuranLabel {
                                anchors.fill: parent
                                wrapMode: Text.WordWrap
                                fadeTruncation: true
                                text: name
                                color: _recitations.current == recitationId ? theme.highlightColor : theme.primaryColor
                        }
                }
        }
}
