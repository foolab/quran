// -*- qml-mode -*-
import QtQuick 1.0

Grid {
        property variant suras: []
        rows: 1
        columns: suras.length
        spacing: 20

        Repeater {
                model: suras
                Label {
                        // Hack: e need to reverse them
                        Component.onCompleted: text = suras[suras.length - index - 1];
                }
        }
}
