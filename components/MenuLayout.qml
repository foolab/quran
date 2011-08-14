// -*- qml-mode -*-
import QtQuick 1.0

Column {
        id: layout
        width: parent.width
        y: parent.height

        state: parent.state

        states: [
        State {
                name: "open"
                PropertyChanges { target: layout; y: parent.height - layout.height - toolBar.height }
                },
        State {
                name: "close"
                PropertyChanges { target: layout; y: parent.height }
              }
        ]

        // We cannot use this:
        //        transitions: parent.transitions
        // QDeclarativeExpression: Expression "(function() { return parent.transitions })"
        // depends on non-NOTIFYable properties: Menu_QMLTYPE_8::transitions

        transitions: [
        Transition {
                from: "open"; to: "close"
                PropertyAnimation { properties: "y,opacity"; duration: 200 }
        },
        Transition {
                from: "close"; to: "open"
                PropertyAnimation { properties: "y,opacity"; duration: 200 }
        }
        ]
}
