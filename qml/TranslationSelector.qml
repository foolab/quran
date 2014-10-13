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
                                color: _translations.current == tid ? theme.highlightColor : theme.primaryColor
                        }
                }
        }
}
