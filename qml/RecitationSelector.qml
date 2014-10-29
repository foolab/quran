// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranPage {
    QuranListView {
        anchors.fill: parent

        header: QuranPageHeader {
            width: parent.width
            title: qsTr("Choose recitation")
        }

        model: _recitations

        delegate: ListDelegate {
            id: item
            visible: recitation.status == Recitation.Installed

            onClicked: {
                if (_recitations.loadRecitation(recitation.uuid)) {
                    settings.defaultRecitation = recitation.uuid
                    popPage()
                } else {
                    banner.showMessage(qsTr("Failed to load recitation"))
                }
            }

            QuranLabel {
                visible: item.visible
                width: parent.width
                height: item.visible ? quranTheme.itemSizeLarge : 0
                truncateText: true
                text: qsTr("%1 %2").arg(recitation.name).arg(recitation.quality)
                verticalAlignment: Text.AlignVCenter
                color: recitation.loaded ? quranTheme.highlightColor : quranTheme.primaryColor
            }
        }
    }
}
