// -*- qml-mode -*-
import QtQuick 1.0

Page {
        id: translationsAddPage

        tools: toolBar

        QueryDialog {
                id: dlDialog
                titleText: qsTr("Download translation?");
                acceptButtonText: qsTr("Yes")
                rejectButtonText: qsTr("No")
        }

        QueryDialog {
                id: rmDialog
                titleText: qsTr("Remove translation?");
                acceptButtonText: qsTr("Yes")
                rejectButtonText: qsTr("No")
        }

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

                                        function dlDialogRejected() {
                                                dlDialog.accepted.disconnect(dlDialogAccepted);
                                                dlDialog.rejected.disconnect(dlDialogRejected);
                                        }

                                        function dlDialogAccepted() {
                                                dlDialog.accepted.disconnect(dlDialogAccepted);
                                                dlDialog.rejected.disconnect(dlDialogRejected);
                                                startDownload();
                                        }

                                        function rmDialogRejected() {
                                                rmDialog.accepted.disconnect(rmDialogAccepted);
                                                rmDialog.rejected.disconnect(rmDialogRejected);
                                        }

                                        function rmDialogAccepted() {
                                                rmDialog.accepted.disconnect(rmDialogAccepted);
                                                rmDialog.rejected.disconnect(rmDialogRejected);
                                                _translations.removeTranslation(tid);
                                        }

                                        onClicked: {
                                                dlDialog.message = text;
                                                dlDialog.accepted.connect(dlDialogAccepted);
                                                dlDialog.rejected.connect(dlDialogRejected);
                                                dlDialog.open();
                                        }

                                        onRemoveClicked: {
                                                rmDialog.message = text;
                                                rmDialog.accepted.connect(rmDialogAccepted);
                                                rmDialog.rejected.connect(rmDialogRejected);
                                                rmDialog.open();
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
                        ToolButton { icon: "general_backspace"; onClicked: pageStack.pop(); }
		        }
		}
}
