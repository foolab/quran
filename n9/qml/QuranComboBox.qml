// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

ListDelegate {
        id: combo

        property QtObject model: QtObject {
                property string title: combo.label
                property string subtitle: actions[combo.currentIndex].text
        }

        property string label
        property list<MenuAction> actions
        property alias currentIndex: dialog.selectedIndex
        width: parent.width
        x: quranTheme.marginSmall

        onClicked: dialog.open()

        SelectionDialog {
                id: dialog
                model: combo.actions
                titleText: combo.label
        }
}
