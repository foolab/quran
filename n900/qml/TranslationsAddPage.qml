// -*- qml-mode -*-
import QtQuick 1.0
import Translations 1.0

TranslationsPage {
        id: translationsAddPage

        tools: toolBar

        Component {
                id: translationsDelegate

                Column {
                        property bool expanded: false
                        id: col
                        ListView.delayRemove: true

                        Rectangle {
                                width: view.width
                                height: label.height * 2
                                color: mouse.pressed || col.expanded ? "steelblue" : "white"
                                MouseArea {
                                        id: mouse
                                        anchors.fill: parent
                                        onClicked: col.expanded = !col.expanded
                                }

                                Label {
                                        x: 20
                                        id: label
                                        font.pointSize: 22
                                        anchors.verticalCenter: parent.verticalCenter
                                        text: _translations.categoryName(modelData)
                                        color: mouse.pressed || col.expanded ? "white" : "black"
                                }
                        }

                        Repeater {
                                model: col.expanded ? _translations.translations(modelData) : 0

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
                }
        }

        ListView {
                id: view
                anchors.top: parent.top
                anchors.bottom: toolBar.top
                anchors.left: parent.left
                anchors.right: parent.right
                model: _translations.categories
                delegate: translationsDelegate
        }

        ToolBar {
                id: toolBar
                ToolBarLayout {
                        ToolButton { icon: theme.pageBack; onClicked: pageStack.pop(); }
		        }
		}
}
