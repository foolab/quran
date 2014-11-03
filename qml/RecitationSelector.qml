// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranPage {
    menu: pageMenu

    QuranPageMenu {
        id: pageMenu
        view: view
        actions: [
            MenuAction {
                text: qsTr("Manage recitations")
                onClicked: pushAnimated(Qt.resolvedUrl("RecitationAddPage.qml"))
            }
        ]
    }

    QuranListView {
        id: view
        anchors.fill: parent

        section {
            property: "online"
            delegate: QuranLabel {
                anchors {
                    right: parent.right
                    rightMargin: quranTheme.marginMedium
                    left: parent.left
                    leftMargin: quranTheme.marginMedium
                }

                color: quranTheme.highlightColor
                height: quranTheme.itemSizeSmall
                text: section == "online" ? qsTr("Online recitations") : qsTr("Offline recitations")
                verticalAlignment: Text.AlignVCenter
            }
        }

        header: QuranPageHeader {
            width: parent.width
            title: qsTr("Choose recitation")
        }

        model: recitations

        delegate: ListDelegate {
            id: item
            visible: recitation.status == Recitation.Installed

            onClicked: {
                if (recitations.loadRecitation(recitation.uuid)) {
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
