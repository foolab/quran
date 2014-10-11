// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranPage {
        Component {
                id: translationsDelegate

                TranslationDelegate {
                        tid: translationId
                        showCategory: true
                }
        }

        QuranPageMenu {
                view: view

                actions: [
                MenuAction {
                        text: qsTr("Add translation")
                        onClicked: pushAnimated(Qt.resolvedUrl("TranslationLanguageList.qml"))
                }
                ]
        }

        QuranListView {
                id: view
                anchors.fill: parent
                model: ActiveTranslationsModel {
                        translations: _translations
                }

                delegate: translationsDelegate
                header: QuranPageHeader {
                        width: parent.width
                        title: qsTr("Available translations")
                }

                QuranViewPlaceholder {
                        text: qsTr("No translations. Pull down to add a translation.")
                        enabled: view.count == 0
                }
        }
}
