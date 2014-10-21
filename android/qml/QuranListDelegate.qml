// -*- qml -*-
import QtQuick 2.2
import QtQuick.Controls 1.2

QuranBackgroundItem {
    id: topItem
    property list<MenuAction> actions
    default property alias content: item.data
    property QtObject _menu
    width: ListView.view.width
    height: contentHeight
    property alias contentHeight: item.height

    property Component _menuComponent: Component {
        Menu {
            id: menu
            Instantiator {
                model: topItem.actions
                delegate: MenuItem {
                    text: modelData.text
                    visible: modelData.visible === true
                    onTriggered: modelData.clicked()
                }

                onObjectAdded: menu.insertItem(index, object)
                onObjectRemoved: menu.removeItem(object)
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

        _menu.popup()
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
