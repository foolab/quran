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
                                leftMargin: quranTheme.marginMedium
                                right: parent.right
                                rightMargin: quranTheme.marginMedium
                        }

                        height: quranTheme.itemSizeSmall

                        QuranLabel {
                                anchors.fill: parent
                                wrapMode: Text.WordWrap
                                fadeTruncation: true
                                text: name
                                color: _recitations.current == recitationId ? quranTheme.highlightColor : quranTheme.primaryColor
                        }
                }
        }
}
