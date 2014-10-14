// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranPage {
        QuranListView {
                anchors.fill: parent

                header: QuranPageHeader {
                        width: parent.width
                        title: qsTr("Choose translation")
                }

                model: InstalledTranslationsModel {
                        translations: _translations
                }

                delegate: QuranBackgroundItem {
                        property int tid: translationId
                        onClicked: {
                                translationsManager.changeTranslation(tid)
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
                                color: _translations.current == tid ? quranTheme.highlightColor : quranTheme.primaryColor
                        }
                }
        }
}
