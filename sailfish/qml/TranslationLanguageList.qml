// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0
import Quran 1.0

QuranPage {
        Component {
                id: language

                BackgroundItem {
                        width: view.width
                        height: Theme.itemSizeSmall
                        onClicked: pageStack.push(Qt.resolvedUrl("TranslationsAddPage.qml"), {cid: translationId})
                        Label {
                                id: label
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.leftMargin: 16
                                text: name
                                color: Theme.primaryColor
                                anchors.verticalCenter: parent.verticalCenter
                        }
                }
        }

        SilicaListView {
                id: view
                anchors.fill: parent
                model: TranslationCategoriesModel {
                        translations: _translations
                }
                delegate: language
                header: PageHeader {
                        width: parent.width
                        title: qsTr("Choose language")
                }
        }
}
