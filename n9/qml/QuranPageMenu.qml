// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.0

ContextMenu {
        property list<MenuAction> actions
        property variant view

        MenuLayout {
                Repeater {
                        function reverse(m) {
                                var l = m.length
                                var out = []

                                for (var x = 0; x < l; x++) {
                                        out[x] = m[l - x - 1]
                                }

                                return out
                        }

                        model: reverse(actions)
                        MenuItem {
                                text: modelData.text
                                onClicked: modelData.clicked()
                        }
                }
        }
}
