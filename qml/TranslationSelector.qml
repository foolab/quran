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
    menu: pageMenu

    Component.onCompleted: {
        if (translations.installedCount == 0) {
            pushImmediate(Qt.resolvedUrl("TranslationAddPage.qml"))
        }
    }

    QuranPageMenu {
        id: pageMenu
        view: view
        actions: [
            MenuAction {
                text: qsTr("Manage translations")
                icon: "image://icon/translation-settings.png"
                onClicked: pushAnimated(Qt.resolvedUrl("TranslationAddPage.qml"))
            }
        ]
    }

    QuranListView {
        id: view
        anchors.fill: parent

        QuranViewPlaceholder {
            enabled: translations.installedCount == 0
            text: qsTr("Please install translations first.")
        }

        header: QuranPageHeader {
            width: parent.width
            title: qsTr("Choose translation")
        }

        model: InstalledFilterModel {
            Component.onCompleted: init(translations, "status", Translation.Installed, "translation")
        }

        delegate: ListDelegate {
            id: item

            onClicked: {
                if (translations.loadTranslation(translation.uuid)) {
                    settings.defaultTranslation = translation.uuid
                    popPage()
                } else {
                    banner.showMessage(qsTr("Failed to load translation"))
                }
            }

            QuranLabel {
                width: parent.width
                height: item.visible ? quranTheme.itemSizeLarge : 0
                truncateText: true
                text: translation.name
                verticalAlignment: Text.AlignVCenter
                color: translation.loaded ? quranTheme.highlightColor : quranTheme.primaryColor
            }
        }
    }
}
