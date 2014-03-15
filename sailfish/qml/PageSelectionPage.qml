// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
        id: dialog
        // TODO: copy of QuranPage
        allowedOrientations: _settings.orientation == 1 ? Orientation.Portrait | Orientation.PortraitInverted : _settings.orientation == 2 ? Orientation.Landscape | Orientation.LandscapeInverted : Orientation.All

        property int targetNumber: (hundreds.currentIndex * 100) + (tens.currentIndex * 10) + ones.currentIndex
        canAccept: targetNumber >= 1 && targetNumber <= _data.pageCount

        onDone: {
                if (dialog.result == DialogResult.Accepted) {
                    _settings.pageNumber = targetNumber - 1
                }
        }

        DialogHeader {
                width: dialog.width
                title: qsTr("Choose a page")
        }

        Rectangle {
                color: Theme.highlightColor
                width: row.width
                height: row.height
                anchors.centerIn: parent
                opacity: 0.3
                border.width: 2
                border.color: Theme.secondaryHighlightColor
        }

        Row {
                id: row
                anchors.centerIn: parent
                width: Theme.itemSizeLarge * 3
                height: Theme.itemSizeLarge
                clip: true

                SilicaListView {
                        id: hundreds
                        width: Theme.itemSizeLarge
                        height: Theme.itemSizeLarge
                        model: 7
                        snapMode: ListView.SnapToItem
                        delegate: viewDelegate
                        highlightRangeMode: ListView.StrictlyEnforceRange
                        currentIndex: parseInt((_settings.pageNumber + 1) / 100)
                }

                SilicaListView {
                        id: tens
                        width: Theme.itemSizeLarge
                        height: Theme.itemSizeLarge
                        model: 10
                        snapMode: ListView.SnapToItem
                        delegate: viewDelegate
                        highlightRangeMode: ListView.StrictlyEnforceRange
                        currentIndex: parseInt(((_settings.pageNumber + 1) % 100) / 10)
                }

                SilicaListView {
                        id: ones
                        width: Theme.itemSizeLarge
                        height: Theme.itemSizeLarge
                        model: 10
                        snapMode: ListView.SnapToItem
                        delegate: viewDelegate
                        highlightRangeMode: ListView.StrictlyEnforceRange
                        currentIndex: parseInt((_settings.pageNumber + 1) % 10)
                }
        }

        Component {
                id: viewDelegate

                NumberLabel {
                        width: Theme.itemSizeLarge
                        height: Theme.itemSizeLarge

                        number: modelData - 1
                        enableSignals: false
                }
        }
}
