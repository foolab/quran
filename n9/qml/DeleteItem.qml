// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.0

Item {
        signal confirmed

        function confirm(parentItem, message) {
                execute(parentItem, message, confirmed)
        }
}
