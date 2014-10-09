// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

Grid {
        property variant suras: []
        rows: 1
        columns: suras.length
        spacing: theme.spacing

        Repeater {
                model: suras
                Label {
                        // Hack: we need to reverse them
                        Component.onCompleted: text = suras[suras.length - index - 1];
                        color: theme.textColor
                        font.family: Theme.fontFamily
                }
        }
}
