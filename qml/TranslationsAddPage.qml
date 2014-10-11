// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranPage {
        id: translationsAddPage

        property int cid

        Component {
                id: delegate

                TranslationDelegate {
                        tid: translationId
                        showCategory: false
                }
        }

        QuranListView {
                id: view
                anchors.fill: parent
                model: TranslationCollection {
                        translations: _translations
                        language: translationsAddPage.cid
                }

                delegate: delegate
                header: QuranPageHeader {
                        width: parent.width
                        title: _translations.categoryName(translationsAddPage.cid)
                }
        }
}
