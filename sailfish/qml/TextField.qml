// -*- qml -*-
import QtQuick 2.0

Rectangle {
        property alias text: input.text
        property alias horizontalAlignment: input.horizontalAlignment
        property string placeholderText: ""

        id: rect
        border.width: 2
        border.color: _colors.textFieldBorderColor
        color: _colors.textFieldColor
        width: 300
        height: input.height

        TextInput {
                id: input
                font.pointSize: 24
                font.family: "Nokia Sans"
                color: _colors.textFieldTextColor
                width: parent.width - 10
                height: 50
//                validator: IntValidator {}
                anchors.horizontalCenter: parent.horizontalCenter
                Label {
                        anchors.fill: parent
                        anchors.centerIn: parent
                        text: input.text || input.focus ? "" : rect.placeholderText
                        font.family: parent.font.family
//                        font.pointSize: parent.font.pointSize
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: _colors.textFieldPlaceholderTextColor
                }

                MouseArea {
                        onClicked: input.focus = true;
                }
        }
}
