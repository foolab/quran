// -*- qml-mode -*-
import QtQuick 1.0

Page {
        id: translationListPage

        tools: toolBar

        QueryDialog {
                id: rmDialog
                titleText: qsTr("Remove translation?");
                acceptButtonText: qsTr("Yes")
                rejectButtonText: qsTr("No")
        }

        Component {
                id: translationsDelegate

                // TODO: clicking it makes it the default translation ?
                TranslationLabel {
                        id: label
                        tid: modelData
                        width: view.width

                        function dialogRejected() {
                                rmDialog.accepted.disconnect(dialogAccepted);
                                rmDialog.rejected.disconnect(dialogRejected);
                        }

                        function dialogAccepted() {
                                _translations.removeTranslation(tid);
                        }

                        onRemoveClicked: {
                                rmDialog.message = text;
                                rmDialog.accepted.connect(dialogAccepted);
                                rmDialog.rejected.connect(dialogRejected);
                                rmDialog.open();
                        }
                }
        }

        ListView {
                id: view
                anchors.top: parent.top
                anchors.bottom: toolBar.top
                anchors.left: parent.left
                anchors.right: parent.right
                model: _translations.installedTranslations
                delegate: translationsDelegate
        }

        ToolBar {
                id: toolBar
                ToolBarLayout {
                        ToolButton { icon: "general_backspace"; onClicked: pageStack.pop(); }
                        ToolButton { icon: "general_add"; onClicked: pageStack.push("TranslationsAddPage"); }
		        }
		}
}
