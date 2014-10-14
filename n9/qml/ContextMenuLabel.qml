// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.0

QuranBackgroundItem {
        id: label
        property list<MenuAction> actions
        property alias content: row.children
        property alias contentHeight: row.height

//        x: quranTheme.marginSmall
        width: parent ? parent.width : quranTheme.itemSizeLarge
        height: row.height

        property Item _menu
        property Component _menuComponent: Component {
                ContextMenu {
                        Repeater {
                                model: label.actions
                                MenuItem {
                                        text: modelData.text
                                        visible: modelData.visible === true
                                        onClicked: modelData.clicked()
                                }
                        }
                }
        }

        onPressAndHold: {
                if (actions.length == 0) {
                    return
                }

                if (!_menu) {
                    _menu = _menuComponent.createObject(label)
                }

                _menu.open()
        }

        Item {
                id: row
                height: quranTheme.itemSizeSmall
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: quranTheme.marginSmall
                anchors.rightMargin: quranTheme.marginSmall
        }
}
