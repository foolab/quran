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
                        tid: modelData
                        width: view.width
                }
        }

        SilicaListView {
                id: view
                anchors.fill: parent
                model: _translations.translations(parent.cid)
                delegate: delegate
                header: PageHeader {
                        width: parent.width
                        title: _translations.categoryName(cid)
                }
        }
}
