// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.0

QuranBackgroundItem {
    id: topItem
    property list<MenuAction> actions
    default property alias content: item.data
    property Item _menu
    width: ListView.view.width
    height: contentHeight
    property alias contentHeight: item.height

    property Component _menuComponent: Component {
        ContextMenu {
            id: menu
            Repeater {
                model: topItem.actions
                MenuItem {
                    text: modelData.text
                    visible: modelData.visible === true
                    onClicked: { menu.close(); modelData.clicked() }
                }
            }
        }
    }

    onPressAndHold: {
        if (actions.length == 0) {
            return
        }

        if (!_menu) {
            _menu = _menuComponent.createObject(topItem)
        }

        _menu.open()
    }

    Item {
        id: item

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
