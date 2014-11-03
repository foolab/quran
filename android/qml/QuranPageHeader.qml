// -*- qml -*-
import QtQuick 2.2
import QtQuick.Controls 1.2

Item {
    id: item
    property string title
    property Item page

    Component.onCompleted: {
        var p = parent
        while (p) {
            if (p.hasOwnProperty("__pageTitle")) {
                item.page = p
                p.__pageTitle = item.title
            }

            p = p.parent
        }
    }

    onTitleChanged: {
        if (page)
            page.__pageTitle = title
    }
}
