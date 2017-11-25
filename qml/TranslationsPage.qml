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

import QtQuick 2.2
import Quran 1.0

QuranPage {
    TranslationsModel {
        id: translationsModel
        source: translations
    }

    QuranListView {
        id: view
        anchors.fill: parent

        model: VisibilityFilterModel {
            model: translationsModel
        }

        section {
            property: "section"
            delegate: QuranLabel {
                anchors {
                    right: parent.right
                    rightMargin: quranTheme.sizes.marginMedium
                    left: parent.left
                    leftMargin: quranTheme.sizes.marginMedium
                }

                color: quranTheme.colors.secondary
                height: quranTheme.sizes.itemSmall
                text: section
                verticalAlignment: Text.AlignVCenter
            }
        }

        delegate: ListDelegate {
            id: item

            function _toggleTranslation() {
                if (translation.status == Translation.Downloading) {
                    translation.stopDownload()
                } else if (translation.status != Translation.Installed) {
                    if (!translation.startDownload()) {
                        banner.showMessage("Failed to download translation")
                    }
                } else {
                    deleter.confirm(item, qsTr("Removing"), qsTr("Remove translation?"))
                }
            }

            DeleteItem {
                id: deleter
                onConfirmed: {
                    if (!translations.removeTranslation(translation.uuid)) {
                        banner.showMessage("Failed to remove translation")
                    }
                }
            }

            Row {
                width: parent.width
                height: quranTheme.sizes.itemLarge
                spacing: quranTheme.sizes.spacing

                QuranLabel {
                    height: parent.height
                    width: parent.width - button.width - quranTheme.sizes.spacing
                    text: translation.name
                    verticalAlignment: Text.AlignVCenter
                    color: item.highlighted ? quranTheme.colors.primaryHighlight : quranTheme.colors.primary
                    truncateText: true
                }

                ToolButton {
                    id: button
                    icon: translation.status == Translation.Installed ? "image://icon/clear.png" : "image://icon/download.png"
                    onClicked: _toggleTranslation()
                }
            }

            Item {
                width: parent.width
                height: quranTheme.sizes.itemLarge
                visible: translation.status == Translation.Installed

                QuranLabel {
                    height: parent.height
                    width: parent.width - defaultButton.width
                    text: translation.language
                    verticalAlignment: Text.AlignVCenter
                    color: item.highlighted ? quranTheme.colors.primaryHighlight : quranTheme.colors.primary
                    truncateText: true
                    anchors.left: parent.left
                }

                QuranButton {
                    id: defaultButton
                    visible: settings.defaultTranslation != translation.uuid
                    anchors {
                        verticalCenter: parent.verticalCenter
                        right: parent.right
                    }
                    text: qsTr("Default")
                    onClicked: translations.loadAndSetDefault(translation.uuid)
                }
            }

            QuranProgressBar {
                anchors {
                    left: parent.left
                    right: parent.right
                }

                visible: translation.status == Translation.Downloading
                minimumValue: 0
                maximumValue: 100
                value: translation.downloadProgress
            }

            Connections {
                target: translation
                onStatusChanged: {
                    if (translation.status == Translation.None && translation.loaded) {
                        // We just removed the loaded translation. Find a replacement
                        translations.loadTranslation('')
                        var t = translations.findInstalledTranslation();
                        if (t != '') {
                            translations.loadAndSetDefault(t)
                        }
                    }

                    if (translation.status == Translation.Installed && translations.installedCount == 1) {
                        translations.loadAndSetDefault(translation.uuid)
                    }
                }
            }
        }

        header: QuranPageHeader {
            width: parent.width
            title: qsTr("Manage translations")
        }
    }
}
