// -*- qml-mode -*-
import QtQuick 1.0

Dialog {
        id: pageDialog
        faderOpacity: 0.3

        InfoBanner {
                id: pageError
                text: qsTr("Please choose a page between 1 and 604");
                // TODO: something wrong with the animation
                parent: parent ? parent : pageDialog
        }

        title: Rectangle {
                id: pageDialogTitle

                width: 300
                height: label.height

                anchors.horizontalCenter: parent.horizontalCenter
                color: "steelblue"

                Label {
                        id: label
                        width: parent.width
                        color: "white";
                        x: 10
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("Choose a page");
                }
        }

        content: Rectangle {
                width: pageDialogTitle.width
                height: 50
                color: "white"
                anchors.horizontalCenter: parent.horizontalCenter
                radius: 20
                border.width: 2
                border.color: "steelblue"
                smooth: true

                Component.onCompleted: parent.anchors.topMargin = 0; // HACK

                Label {
                        id: pageDialogLabel

                        property string value: ""
                        width: parent.width - 10
                        height: parent.height
                        anchors.horizontalCenter: parent.horizontalCenter
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: _formatter.number(value);
                }
        }

        buttons: Column {
                width: pageDialogTitle.width
                Component.onCompleted: parent.anchors.topMargin = 0; // HACK
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 10

                NumPad {
                        id: numPad
                        width: pageDialogTitle.width
                        anchors.horizontalCenter: parent.horizontalCenter
                }

                DialogButton {
                        text: qsTr("Go");
                        width: pageDialogTitle.width
                        anchors.horizontalCenter: parent.horizontalCenter
                        onClicked: {
                                if (!_data.hasPage(pageDialogLabel.value - 1)) {
                                        pageError.show();
                                        return;
                                }

                                _settings.pageNumber = pageDialogLabel.value - 1;

                                pageDialogLabel.value = "";
                                pageDialog.accept();
                                pageStack.pop();
                        }
                }
        }

        Connections {
                target: numPad
                onInsert: {
                        pageDialogLabel.value += val;
                }

                onBackspace: {
                        var len = pageDialogLabel.value.length;
                        if (len == 0) {
                                return;
                        }

                        var val = pageDialogLabel.value.toString().substr(0, len - 1);

                        pageDialogLabel.value = val;
                }
        }
}