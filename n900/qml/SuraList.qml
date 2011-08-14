// -*- qml-mode -*-
import QtQuick 1.0

// TODO: debug this
/*
file:///root/q/SuraList.qml:14: Unable to assign [undefined] to QString text
file:///root/q/SuraList.qml:14: Unable to assign [undefined] to QString text
*/

Grid {
        property variant suras: []
        rows: 1
        columns: suras.length
        spacing: 20

        Repeater {
                model: suras
                Label {
                        // Hack: e need to reverse them
                        text: suras[suras.length - index - 1]
                }
        }
}
