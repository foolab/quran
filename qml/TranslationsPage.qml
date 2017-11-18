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
    QuranComboBox {
        id: defaultTranslation
        label: qsTr("Default translation")
        model: ListModel { }
        visible: model.count > 0
        onCurrentIndexChanged: translations.loadAndSetDefault(model.get(currentIndex).translationObject.uuid)

        Connections {
            target: settings
            onDefaultTranslationChanged: {
                for (var x = 0; x < defaultTranslation.model.count; x++) {
                    if (defaultTranslation.model.get(x).translationObject.uuid == settings.defaultTranslation) {
                        defaultTranslation.currentIndex = x
                        return
                    }
                }
            }
        }

        Instantiator {
            model: InstalledTranslationsModel { model: view.model }
            delegate: QtObject {
                property var translationObject: translation
                property string text: translation.name
            }

            onObjectAdded: {
                defaultTranslation.model.insert(index, object)

                if (object.translationObject.uuid == settings.defaultTranslation) {
                    defaultTranslation.currentIndex = index
                }
            }

            onObjectRemoved: defaultTranslation.model.remove(index, 1)
        }
    }

    TranslationsModel {
        id: translationsModel
        source: translations
    }

    QuranListView {
        id: view
        clip: true
        anchors {
            top: defaultTranslation.visible ? defaultTranslation.bottom : parent.top
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

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
                    color: quranTheme.colors.primary
                    truncateText: true
                }

                ToolButton {
                    id: button
                    icon: translation.status == Translation.Installed ? "image://icon/clear.png" : "image://icon/download.png"
                    onClicked: _toggleTranslation()
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
