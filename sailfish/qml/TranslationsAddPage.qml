// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

TranslationsPage {
        id: translationsAddPage

        property int cid: 0

        tools: toolBar

        TitleLabel {
                id: title
                width: parent.width
                anchors.top: parent.top
                text: _translations.categoryName(parent.cid)
                // TODO: proper alignment
        }

        Component {
                id: delegate

                TranslationLabel {
                        id: label
                        tid: modelData
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
                model: _translations.translations(parent.cid)
                delegate: delegate
        }

        ToolBar {
                id: toolBar
                ToolBarLayout {
                        ToolButton { icon: theme.pageBack; onClicked: pageStack.pop(); }
		        }
		}
}
