// -*- qml-mode -*-
import QtQuick 1.0

Page {
        id: translationsPage

        property int __tid: -1

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

        QueryDialog {
                id: stopDialog
                titleText: qsTr("Stop download?");
                acceptButtonText: qsTr("Yes")
                rejectButtonText: qsTr("No")
        }

        function __rmDialogAccepted() {
                rmDialog.accepted.disconnect(__rmDialogAccepted);
                rmDialog.rejected.disconnect(__rmDialogRejected);
                _translations.removeTranslation(__tid);
        }

        function __rmDialogRejected() {
                rmDialog.accepted.disconnect(__rmDialogAccepted);
                rmDialog.rejected.disconnect(__rmDialogRejected);
        }

        function __stopDialogAccepted() {
                stopDialog.accepted.disconnect(__stopDialogAccepted);
                stopDialog.rejected.disconnect(__stopDialogRejected);
                _translations.stopDownload(__tid);
        }

        function __stopDialogRejected() {
                stopDialog.accepted.disconnect(__stopDialogAccepted);
                stopDialog.rejected.disconnect(__stopDialogRejected);
        }

        function __dlDialogAccepted() {
                dlDialog.accepted.disconnect(__dlDialogAccepted);
                dlDialog.rejected.disconnect(__dlDialogRejected);
                _translations.startDownload(__tid);
        }

        function __dlDialogRejected() {
                dlDialog.accepted.disconnect(__dlDialogAccepted);
                dlDialog.rejected.disconnect(__dlDialogRejected);
        }


        function askForStop(tid) {
                __tid = tid;
                stopDialog.message = _translations.translationName(__tid);
                stopDialog.accepted.connect(__stopDialogAccepted);
                stopDialog.rejected.connect(__stopDialogRejected);
                stopDialog.open();
        }

        function askForDownload(tid) {
                __tid = tid;
                dlDialog.message = _translations.translationName(__tid);
                dlDialog.accepted.connect(__dlDialogAccepted);
                dlDialog.rejected.connect(__dlDialogRejected);
                dlDialog.open();
        }

        function askForRemoval(tid) {
                __tid = tid;
                rmDialog.message = _translations.translationName(__tid);
                rmDialog.accepted.connect(__rmDialogAccepted);
                rmDialog.rejected.connect(__rmDialogRejected);
                rmDialog.open();
        }
}


