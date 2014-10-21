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

                delegate: QuranListDelegate {
                        onClicked: {
                                recitationsManager.changeRecitation(recitationId)
                                popPage()
                        }

                        contentHeight: quranTheme.itemSizeLarge

                        QuranLabel {
                                wrapMode: Text.WordWrap
                                truncateText: true
                                text: name
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                color: _recitations.current == recitationId ? quranTheme.highlightColor : quranTheme.primaryColor
                        }
                }
        }
}
