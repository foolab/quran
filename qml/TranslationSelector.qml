// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranDialog {
        title: qsTr("Choose translation")

        content: QuranListView {
                id: view
                model: InstalledTranslationsModel {
                        translations: _translations
                }

                anchors.fill: parent
                delegate: QuranBackgroundItem {
                        property int tid: translationId
                        onClicked: {
                                translationsManager.changeTranslation(tid)
                                pageStack.pop()
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
