// -*- qml -*-
import QtQuick 1.0
import Quran 1.0

TranslationsPage {
        id: translationsAddPage

        property int cid: -1

        tools: toolBar

        TitleLabel {
                id: title
                width: parent.width
                anchors.top: parent.top
                text: _translations.categoryName(parent.cid)
        }

        Component {
                id: delegate

                TranslationLabel {
                        id: label
                        tid: translationId
                        width: view.width

                        onClicked: {
                                if (status == Translation.Downloading) {
                                        askForStop(tid);
                                }
                                else if (status == Translation.None ||
                                         status == Translation.Error) {
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
                model: TranslationCollection {
                        translations: _translations
                        language: translationsAddPage.cid
                }

                delegate: delegate
        }

        ToolBar {
                id: toolBar
                ToolBarLayout {
                        ToolButton { icon: theme.pageBack; onClicked: pageStack.pop(); }
		        }
		}
}