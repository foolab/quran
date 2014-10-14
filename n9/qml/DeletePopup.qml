// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.0

Item {
        signal confirmed

        function confirm(message) {
                execute(message, confirmed)
        }
}
