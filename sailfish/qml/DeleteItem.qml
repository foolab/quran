// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

RemorseItem {
        signal confirmed

        function confirm(parentItem, message, question) {
                execute(parentItem, message, confirmed)
        }
}
