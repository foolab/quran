// -*- qml -*-
import QtQuick 1.0

Page {
        id: translationsPage

        property int __tid: -1

        MassStorageQueryDialog {
                id: dlDialog
                titleText: qsTr("Download translation?");
                acceptButtonText: qsTr("Yes")
                rejectButtonText: qsTr("No")
        }

        MassStorageQueryDialog {
                id: stopDialog
                titleText: qsTr("Stop download?");
                acceptButtonText: qsTr("Yes")
                rejectButtonText: qsTr("No")
        }

        MassStorageQueryDialog {
                id: rmDialog
                titleText: qsTr("Remove translation?");
                acceptButtonText: qsTr("Yes")
                rejectButtonText: qsTr("No")
        }

        Rectangle {
                id: massStorageGuard
                anchors.top: parent.top
                anchors.bottom: toolBar.top
                anchors.left: parent.left
                anchors.right: parent.right
                z: 4000
                state: _fsmon.available ? "fsAvailable" : "fsUnavailable"
                color: "steelblue"

                states: [
                State {
                        name: "fsUnavailable"
                        PropertyChanges { target: massStorageGuard; opacity: 0.9 }
                        PropertyChanges { target: massStorageLabel; opacity: 1.0 }
                        },
                State {
                        name: "fsAvailable"
                        PropertyChanges { target: massStorageGuard; opacity: 0.0 }
                        PropertyChanges { target: massStorageLabel; opacity: 0.0 }
                        }
                ]

                transitions: [
                Transition {
                        from: "fsAvailable"; to: "fsUnavailable"
                        PropertyAnimation { properties: "opacity"; duration: 500 }
                },
                Transition {
                        from: "fsUnavailable"; to: "fsAvailable"
                        PropertyAnimation { properties: "opacity"; duration: 500 }
                }
                ]

                MouseArea {
                        id: massStorageGlass
                        enabled: massStorageGuard.state == "fsUnavailable"
                        anchors.fill: parent
                }
        }

        Label {
                id: massStorageLabel
                anchors.centerIn: massStorageGuard
                text: qsTr("Translations cannot be used in mass storage mode.");
                width: massStorageGuard.width - 60
                z: 4001
                horizontalAlignment: Text.AlignHCenter
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

        function __rmDialogAccepted() {
                rmDialog.accepted.disconnect(__rmDialogAccepted);
                rmDialog.rejected.disconnect(__rmDialogRejected);
                _translations.removeTranslation(__tid);
                pageStack.pop();
        }

        function __rmDialogRejected() {
                rmDialog.accepted.disconnect(__rmDialogAccepted);
                rmDialog.rejected.disconnect(__rmDialogRejected);
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


