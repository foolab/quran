// -*- qml -*-
import QtQuick 1.0
import Quran 1.0

ListView {
        id: view
        clip: true
        width: parent ? parent.width : undefined
        model: QuranViewModel {data: _data; page: view.page}
        highlightMoveDuration: 1
        cacheBuffer: 999999999 // insanely high number
        Component.onCompleted: positionView();

        delegate: Component {
                Loader {
                        ListView.delayRemove: true
                        width: view.width
                        source: type == QuranViewModel.Title ?
                                Qt.resolvedUrl("ChapterDelegate.qml") :
                                Qt.resolvedUrl("VerseDelegate.qml")
                }
        }

        property int page: -1

        function show() {
                state = "shown"
        }

        function hide() {
                state = "hidden"
                view.destroy(300);
        }

        state: "hidden"

        states: [
        State {
                name: "shown"
                PropertyChanges { target: view; opacity: 1.0 }
                },
        State {
                name: "hidden"
                PropertyChanges { target: view; opacity: 0.0 }
              }
        ]

        transitions: [
        Transition {
                from: "shown"; to: "hidden"
                PropertyAnimation { properties: "opacity"; duration: 250 }
        },
        Transition {
                from: "hidden"; to: "shown"
                PropertyAnimation { properties: "opacity"; duration: 250 }
        }
        ]

        Connections {
                target: pagePosition
                onChanged: positionView()
        }

        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10

        Timer {
                id: timer
                property int index
                interval: 1
                onTriggered: view.positionViewAtIndex(index, ListView.Contain);
        }

        function positionView() {
                if (pagePosition.isValid()) {
                        var index = view.model.findIndex(pagePosition.sura, pagePosition.aya);
                        if (index != -1) {
                                pagePosition.reset();
// Not working :/
//                                view.positionViewAtIndex(index, ListView.Visible);
                                timer.index = index;
                                timer.start();
                        }
                }
        }
}
