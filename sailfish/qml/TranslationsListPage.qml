// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0
import Quran 1.0

// TODO: add -> download -> stop -> canceled download still shows here
Page {
        Component {
                id: translationsDelegate

                TranslationLabel {
                        id: label
                        tid: modelData
                        showCategory: true
                        visualParent: view
                        showInstalled: false
                }
        }

        SilicaListView {
                id: view
                anchors.fill: parent
                model: _translations.active
                delegate: translationsDelegate
                header: PageHeader {
                        width: parent.width
                        title: qsTr("Available translations")
                }

                ViewPlaceholder {
                        text: qsTr("No translations. Pull down to add a translation.")
                        enabled: _translations.active.length == 0
                }

                PullDownMenu {
                        MenuItem {
                                text: qsTr("Add translation")
                                onClicked: pageStack.push(Qt.resolvedUrl("TranslationLanguageList.qml"))
                        }
                }
        }
}
