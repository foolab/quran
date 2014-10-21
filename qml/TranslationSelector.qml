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

                delegate: QuranListDelegate {
                        property int tid: translationId
                        onClicked: {
                                translationsManager.changeTranslation(tid)
                                popPage()
                        }

                        contentHeight: quranTheme.itemSizeLarge

                        QuranLabel {
                                anchors.fill: parent
                                wrapMode: Text.WordWrap
                                truncateText: true
                                text: name
                                verticalAlignment: Text.AlignVCenter
                                color: _translations.current == tid ? quranTheme.highlightColor : quranTheme.primaryColor
                        }
                }
        }
}
