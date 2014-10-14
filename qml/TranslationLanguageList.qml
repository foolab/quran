// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranPage {
        Component {
                id: language

                QuranBackgroundItem {
                        width: view.width
                        height: quranTheme.itemSizeSmall
                        onClicked: pushAnimated(Qt.resolvedUrl("TranslationsAddPage.qml"), {cid: translationId})
                        QuranLabel {
                                id: label
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.leftMargin: quranTheme.marginMedium
                                text: name
                                color: quranTheme.primaryColor
                                anchors.verticalCenter: parent.verticalCenter
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
