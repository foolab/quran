import QtQuick 2.0
import Sailfish.Silica 1.0

PullDownMenu {
        property list<MenuAction> actions
        property variant view
        parent: view
        Repeater {
                model: actions

                MenuItem {
                        text: modelData.text
                        onClicked: modelData.clicked()
                }
        }
}
