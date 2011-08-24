// -*- qml-mode -*-
import QtQuick 1.0
import Translations 1.0

TranslationsPage {
        id: translationLanguageListPage

        tools: toolBar

        TitleLabel {
                id: title
                width: parent.width
                anchors.top: parent.top
                text: qsTr("Choose language")
        }

        Component {
                id: delegate

                Rectangle {
                        width: view.width
                        height: label.height * 2
                        color: mouse.pressed ? "steelblue" : "white"

                        MouseArea {
                                id: mouse
                                anchors.fill: parent
                                onClicked: {
                                        pageStack.push("TranslationsAddPage");
                                        pageStack.currentPage.cid = modelData;
                                }
                        }

                        Label {
                                id: label
                                x: 16
                                font.pointSize: 24
                                text: _translations.categoryName(modelData)
                                color: mouse.pressed ? "white" : "black"
                                anchors.verticalCenter: parent.verticalCenter
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
                model: _translations.categories
                delegate: delegate
        }

        ToolBar {
                id: toolBar
                ToolBarLayout {
                        ToolButton { icon: theme.pageBack; onClicked: pageStack.pop(); }
		        }
		}
}