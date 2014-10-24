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

                delegate: ListDelegate {
                        onClicked: {
                                recitationsManager.changeRecitation(recitationId)
                                popPage()
                        }

                        QuranLabel {
                                truncateText: true
                                text: name
                                width: parent.width
                                height: quranTheme.itemSizeLarge
                                verticalAlignment: Text.AlignVCenter
                                color: _recitations.current == recitationId ? quranTheme.highlightColor : quranTheme.primaryColor
                        }
                }
        }
}
