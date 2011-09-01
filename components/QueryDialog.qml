// -*- qml-mode -*-
import QtQuick 1.0

Dialog {
        property alias titleText: titleLabel.text
        property alias message: messageLabel.text
        property alias acceptButtonText: yes.text
        property alias rejectButtonText: no.text

	    id: dialog

        title: Label {
                id: titleLabel
                font.pointSize: 32
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                width: parent.width
                //                        style: Text.Outline
                //                        styleColor: "grey"
                smooth: true
        }

// TODO: label2 ? font ?
        content: Label {
                id: messageLabel
                font.pointSize: 26
                font.bold: true
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                smooth: true
        }


        buttons: Item {
                width: parent.width
                height: col.height

                Column {
                        id: col
                        spacing: 20
                        width: parent.width

                        DialogButton {
                                id: yes
                                onClicked: dialog.accept();
                                anchors.horizontalCenter: parent.horizontalCenter
                        }

                        DialogButton {
                                id: no
                                onClicked: dialog.reject();
                                anchors.horizontalCenter: parent.horizontalCenter
                        }
                }
        }
}
