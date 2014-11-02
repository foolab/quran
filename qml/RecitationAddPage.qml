// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranPage {
    QuranListView {
        id: view
        anchors.fill: parent
        model: _recitations

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

        delegate: ListDelegate {
            id: item

            actions: [
                MenuAction {
                    text: qsTr("Enable")
                    onClicked: {
                        if (!recitation.enable()) {
                            banner.showMessage("Failed to enable recitation")
                        }
                    }

                    visible: recitation.type == Recitation.Online && recitation.status != Recitation.Installed
                },
                MenuAction {
                    text: qsTr("Disable")
                    onClicked: {
                        if (!recitation.enable()) {
                            banner.showMessage("Failed to disable recitation")
                        }
                    }

                    visible: recitation.type == Recitation.Online && recitation.status == Recitation.Installed
                }
            ]

            Item {
                width: parent.width
                height: quranTheme.itemSizeLarge

                QuranStatusIndicator {
                    id: indicator
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    isInstalled: recitation.status == Recitation.Installed
                    downloading: false
                    error: false
                }

                QuranLabel {
                    id: label
                    text: qsTr("%1 %2").arg(recitation.name).arg(recitation.quality)
                    color: quranTheme.primaryColor
                    truncateText: true
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                        right: indicator.left
                        rightMargin: quranTheme.marginSmall
                        left: parent.left
                    }

                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        header: QuranPageHeader {
            width: parent.width
            title: qsTr("Manage recitations")
        }
    }
}
