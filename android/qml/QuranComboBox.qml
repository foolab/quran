// -*- qml -*-
import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Column {
    property list<MenuAction> actions
    property alias label: text.text
    property alias currentIndex: combo.currentIndex

    width: parent.width

    QuranLabel {
        id: text

        anchors {
            left: parent.left
            right: parent.right
            leftMargin: quranTheme.marginSmall
            rightMargin: quranTheme.marginSmall
        }

        font.bold: true
        font.pixelSize: quranTheme.fontSizeSmall
        color: quranTheme.primaryColor
    }

    ComboBox {
        id: combo

        style: Component {
            ComboBoxStyle {
                id: comboStyle

                background: Rectangle {
                    anchors.fill: parent
                    color: control.pressed ? quranTheme.highlightColor : quranTheme.backgroundColor
                }

                label: QuranLabel {
                    text: control.currentText
                    color: quranTheme.primaryColor
                    verticalAlignment: Text.AlignVCenter
                    renderType: comboStyle.renderType
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

        anchors {
            left: parent.left
            right: parent.right
            leftMargin: quranTheme.marginSmall
            rightMargin: quranTheme.marginSmall
        }

        textRole: "text"
        model: actions
        height: quranTheme.itemSizeSmall
    }
}
