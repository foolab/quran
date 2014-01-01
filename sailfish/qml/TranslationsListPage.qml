// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

TranslationsPage {
        id: translationListPage

        tools: toolBar

        TitleLabel {
                id: title
                width: parent.width
                anchors.top: parent.top
                text: qsTr("Translations")
        }

        Component {
                id: translationsDelegate

                TranslationLabel {
                        id: label
                        tid: modelData
                        width: view.width
                        showCategory: true

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
                clip: true
                anchors.top: title.bottom
                anchors.bottom: toolBar.top
                anchors.left: parent.left
                anchors.leftMargin: 16
                anchors.right: parent.right
                anchors.rightMargin: 16
                model: _translations.active
                delegate: translationsDelegate
        }

        ToolBar {
                id: toolBar
                ToolBarLayout {
                        ToolButton { icon: theme.pageBack; onClicked: pageStack.pop(); }
                        ToolButton { icon: theme.addTranslation; onClicked: pageStack.push("TranslationLanguageList"); }
		        }
		}
}
