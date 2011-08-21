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
                id: stopDialog
                titleText: qsTr("Stop download?");
                acceptButtonText: qsTr("Yes")
                rejectButtonText: qsTr("No")
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
}


