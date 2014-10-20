import QtQuick 2.2

ToolButton {
        property QuranPageMenu menu

        height: parent.height
        width: height

        onClicked: {
            if (menu) {
                menu.popup()
            }
        }

        visible: parent.visible && menu != null
        opacity: visible ? 1.0 : 0
        icon.source: highlight ? "image://icon/menu.png?" + quranTheme.buttonHighlightColor : "image://icon/menu.png?" + quranTheme.buttonNormalColor

        Behavior on opacity {
                PropertyAnimation { duration: 400 }
        }
}
