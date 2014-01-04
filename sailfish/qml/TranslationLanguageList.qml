// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0
import Quran 1.0

Page {
        Component {
                id: language

                BackgroundItem {
                        width: view.width
                        height: Theme.itemSizeSmall
                        onClicked: pageStack.push(Qt.resolvedUrl("TranslationsAddPage.qml"), {cid: modelData})
                        Label {
                                id: label
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.leftMargin: 16
                                text: _translations.categoryName(modelData)
                                color: Theme.primaryColor
                                anchors.verticalCenter: parent.verticalCenter
                        }
                }
        }

        SilicaListView {
                id: view
                anchors.fill: parent
                model: _translations.categories
                delegate: language
                header: PageHeader {
                        width: parent.width
                        title: qsTr("Choose language")
                }
        }
}
