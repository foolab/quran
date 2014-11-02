// -*- qml -*-
import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Button {
    height: quranTheme.itemSizeSmall
    style: Component {
        ButtonStyle {
            id: buttonStyle
            readonly property int renderType: Text.NativeRendering

            background: Rectangle {
                anchors.fill: parent
                color: control.pressed ? quranTheme.highlightColor : quranTheme.backgroundColor
                border.width: 2
                border.color: control.pressed ? quranTheme.primaryColor : quranTheme.highlightColor
                radius: border.width * 2
                implicitWidth: quranTheme.itemSizeLarge * 2
            }


            label: QuranLabel {
                text: control.text
                color: quranTheme.primaryColor
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                renderType: buttonStyle.renderType
                anchors {
                    top: parent.top
                    bottom: parent.bottom
                    left: parent.left
                    leftMargin: quranTheme.marginMedium
                    right: parent.right
                    rightMargin: quranTheme.marginMedium
                }
            }
        }
    }
}
