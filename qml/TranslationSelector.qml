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

                delegate: ListDelegate {
                        property int tid: translationId
                        onClicked: {
                                translationsManager.changeTranslation(tid)
                                popPage()
                        }

                        QuranLabel {
                                width: parent.width
                                height: quranTheme.itemSizeLarge
                                truncateText: true
                                text: name
                                verticalAlignment: Text.AlignVCenter
                                color: _translations.current == tid ? quranTheme.highlightColor : quranTheme.primaryColor
                        }
                }
        }
}
