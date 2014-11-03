// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranPage {
    QuranListView {
        id: view
        anchors.fill: parent
        model: translations

        section {
            property: "language"
            delegate: QuranLabel {
                anchors {
                    right: parent.right
                    rightMargin: quranTheme.marginMedium
                    left: parent.left
                    leftMargin: quranTheme.marginMedium
                }

                color: quranTheme.highlightColor
                height: quranTheme.itemSizeSmall
                text: section
                verticalAlignment: Text.AlignVCenter
            }
        }

        delegate: ListDelegate {
            id: item
            property real _height: translation.status == Translation.Downloading ? quranTheme.itemSizeLarge + quranTheme.itemSizeSmall : quranTheme.itemSizeLarge
            contentHeight: _height

            actions: [
                MenuAction {
                    text: qsTr("Download")
                    onClicked: {
                        if (!translation.startDownload()) {
                            banner.showMessage("Failed to download translation")
                        }
                    }

                    visible: translation.status == Translation.None || translation.status == Translation.Error
                },
                MenuAction {
                    text: qsTr("Stop")
                    onClicked: translation.stopDownload()
                    visible: translation.status == Translation.Downloading
                },
                MenuAction {
                    text: qsTr("Remove")
                    onClicked: deleter.confirm(item, qsTr("Removing"), qsTr("Remove translation?"))
                    visible: translation.status == Translation.Installed
                }
            ]

            DeleteItem {
                id: deleter
                onConfirmed: {
                    if (!translations.removeTranslation(translation.uuid)) {
                        banner.showMessage("Failed to remove translation")
                    }
                }
            }

            Item {
                width: parent.width
                height: quranTheme.itemSizeLarge

                QuranStatusIndicator {
                    id: indicator
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    isInstalled: translation.status == Translation.Installed
                    downloading: translation.status == Translation.Downloading
                    error: translation.status == Translation.Error
                }

                QuranLabel {
                    id: label
                    text: translation.name
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

            QuranProgressBar {
                visible: translation.status == Translation.Downloading
                anchors {
                    left: parent.left
                    right: parent.right
                }

                height: quranTheme.itemSizeSmall
                minimumValue: 0
                maximumValue: translation.downloadSize
                value: translation.downloadProgress
            }
        }

        header: QuranPageHeader {
            width: parent.width
            title: qsTr("Manage translations")
        }
    }
}
