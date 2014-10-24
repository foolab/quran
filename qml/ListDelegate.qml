// -*- qml -*-
import QtQuick 2.0

QuranBackgroundItem {
    id: topItem
    property alias actions: menu.actions
    default property alias content: item.data
    width: ListView.view.width
    height: contentHeight + menu.menuHeight
    property alias contentHeight: item.height

    onPressAndHold: menu.pressAndHold()
    onClicked: menu.clicked()

    QuranContextMenu {
        id: menu
        container: topItem
    }

    Column {
        id: item

        anchors {
            top: parent.top

            left: parent.left
            leftMargin: quranTheme.marginMedium
            right: parent.right
            rightMargin: quranTheme.marginMedium
        }
    }
}
