// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.0

ContextMenu {
        property list<MenuAction> actions
        property variant view

        MenuLayout {
                Repeater {
                        model: actions
                        MenuItem {
                                text: modelData.text
                                onClicked: modelData.clicked()
                        }
                }
        }
}
