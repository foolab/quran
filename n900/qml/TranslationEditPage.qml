// -*- qml-mode -*-
import QtQuick 1.0
import Translations 1.0

Page {
        id: translationEditPage

        property int tid: 0

        tools: toolBar

        function __rmDialogAccepted() {
                rmDialog.accepted.disconnect(__rmDialogAccepted);
                rmDialog.rejected.disconnect(__rmDialogRejected);
                _translations.removeTranslation(tid);
                pageStack.pop();
        }

        function __rmDialogRejected() {
                rmDialog.accepted.disconnect(__rmDialogAccepted);
                rmDialog.rejected.disconnect(__rmDialogRejected);
        }

        function askForRemoval() {
                rmDialog.message = _translations.translationName(tid);
                rmDialog.accepted.connect(__rmDialogAccepted);
                rmDialog.rejected.connect(__rmDialogRejected);
                rmDialog.open();
        }

        QueryDialog {
                id: rmDialog
                titleText: qsTr("Remove translation?");
                acceptButtonText: qsTr("Yes")
                rejectButtonText: qsTr("No")
        }

        Label {
                id: name
                text: _translations.translationName(tid);
                font.pointSize: 22
                width: parent.width - 20
                x: 10
                y: 10
                horizontalAlignment: Text.AlignHCenter
        }

        DialogButton {
                text: qsTr("Delete translation");
                width: 400
                anchors.top: name.bottom
                anchors.topMargin: 30
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: askForRemoval();
        }

        ToolBar {
                id: toolBar
                ToolBarLayout {
                        ToolButton { icon: "general_backspace"; onClicked: pageStack.pop(); }
		        }
		}
}
