// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranPage {
        Component {
                id: language

                QuranListDelegate {
                        contentHeight: quranTheme.itemSizeLarge
                        onClicked: pushAnimated(Qt.resolvedUrl("TranslationsAddPage.qml"), {cid: translationId})
                        QuranLabel {
                                id: label
                                anchors.fill: parent
                                text: name
                                color: quranTheme.primaryColor
                                verticalAlignment: Text.AlignVCenter
                        }
                }
        }

        QuranListView {
                id: view
                anchors.fill: parent
                model: TranslationCategoriesModel {
                        translations: _translations
                }
                delegate: language
                header: QuranPageHeader {
                        width: parent.width
                        title: qsTr("Choose language")
                }
        }
}
