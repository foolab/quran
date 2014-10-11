// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

RemorsePopup {
        signal confirmed

        function confirm(message) {
                execute(message, confirmed)
        }
}
