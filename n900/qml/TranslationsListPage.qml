// -*- qml-mode -*-
import QtQuick 1.0
import Translations 1.0

TranslationsPage {
        id: translationListPage

        tools: toolBar

        Component {
                id: translationsDelegate

                // TODO: clicking it makes it the default translation ?
                TranslationLabel {
                        id: label
                        tid: modelData
                        width: view.width

                        onClicked: {
                                if (status == Translation.Downloading) {
                                        askForStop(tid);
                                }
                                else if (status == Translation.Error) {
                                        askForDownload(tid);
                                }
                                else if (status == Translation.Installed) {
                                        pageStack.push("TranslationEditPage");
                                        pageStack.currentPage.tid = tid;
                                }
                        }
                }
        }

        ListView {
                id: view
                anchors.top: parent.top
                anchors.bottom: toolBar.top
                anchors.left: parent.left
                anchors.right: parent.right
                model: _translations.active
                delegate: translationsDelegate
                footer: DialogButton {
                        text: qsTr("Add translation");
                        anchors.horizontalCenter: parent.horizontalCenter
                        onClicked: pageStack.push("TranslationsAddPage");
                }
        }

        ToolBar {
                id: toolBar
                ToolBarLayout {
                        ToolButton { icon: "general_backspace"; onClicked: pageStack.pop(); }
		        }
		}
}
