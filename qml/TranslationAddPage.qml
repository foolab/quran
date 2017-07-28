// -*- qml -*-
/*
 * Copyright (c) 2011-2017 Mohammed Sameer <msameer@foolab.org>.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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
