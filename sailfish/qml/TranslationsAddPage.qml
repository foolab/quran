// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0
import Quran 1.0

Page {
        id: translationsAddPage

        property int cid

        Component {
                id: delegate

                TranslationLabel {
                        id: label
                        tid: translationId
                        width: view.width
                        visualParent: view
                }
        }

        SilicaListView {
                id: view
                anchors.fill: parent
                model: TranslationCollection {
                        translations: _translations
                        language: translationsAddPage.cid
                }

                delegate: delegate
                header: PageHeader {
                        width: parent.width
                        title: _translations.categoryName(translationsAddPage.cid)
                }
        }
}
