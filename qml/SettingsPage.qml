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
    id: settingsPage
    menu: pageMenu

    DeletePopup {
        id: deleter
        onConfirmed: settings.reset()
    }

    QuranPageMenu {
        id: pageMenu
        view: flick
        actions: [
            MenuAction {
                text: qsTr("Manage recitations")
                icon: "image://icon/recitation-settings.png"
                onClicked: pushAnimated(Qt.resolvedUrl("RecitationAddPage.qml"))
            },
            MenuAction {
                text: qsTr("Manage translations")
                icon: "image://icon/translation-settings.png"
                onClicked: pushAnimated(Qt.resolvedUrl("TranslationAddPage.qml"))
            }
        ]
    }

    QuranFlickable {
        id: flick
        anchors.fill: parent
        contentHeight: col.height

        Column {
            id: col
            width: parent.width

            QuranPageHeader {
                width: parent.width
                title: qsTr("Settings")
            }

            Rectangle {
                width: parent.width
                height: previewLabel.height
                color: quranTheme.quranColors.background

                QuranLabel {
                    id: previewLabel
                    width: parent.width
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.family: settings.fontFamily
                    font.pixelSize: textSizeSlider.value
                    color: quranTheme.quranColors.text

                    Component.onCompleted: populate()

                    NumberFormatter {
                        id: formatter
                        format: settings.numberFormat
                        number: 1
                    }

                    function populate() {
                        text = qsTr("%1 (%2)").arg(_data.basmala).arg(formatter.formattedNumber)
                    }

                    Connections {
                        target: settings
                        onNumberFormatChanged: previewLabel.populate()
                        onTextTypeChanged: previewLabel.populate()
                    }
                }
            }

            QuranSlider {
                id: textSizeSlider
                width: parent.width
                label: qsTr("Text size")
                minimumValue: quranTheme.fonts.minimum
                maximumValue: quranTheme.fonts.maximum
                stepSize: 1
                value: settings.fontSize
            }

            Binding {
                target: settings
                value: textSizeSlider.value
                property: "fontSize"
                when: !textSizeSlider.pressed
            }

            QuranSlider {
                id: translationTextSizeSlider
                width: parent.width
                label: qsTr("Translation text size")
                minimumValue: quranTheme.fonts.minimum
                maximumValue: quranTheme.fonts.maximum
                stepSize: 1
                value: settings.translationFontSize
            }

            Binding {
                target: settings
                value: translationTextSizeSlider.value
                property: "translationFontSize"
                when: !translationTextSizeSlider.pressed
            }

            QuranTextSwitch {
                text: qsTr("Use simplified text")
                checked: settings.textType == 1
                onCheckedChanged: settings.textType = checked ? 1 : 0
            }

            QuranTextSwitch {
                text: qsTr("Use Arabic numerals")
                checked: settings.numberFormat == 1
                onCheckedChanged: settings.numberFormat = checked ? 1 : 0
            }

            QuranComboBox {
                model: ListModel {
                    ListElement { text: qsTr("Automatic") }
                    ListElement { text: qsTr("Portrait") }
                    ListElement { text: qsTr("Landscape") }
                }

                label: qsTr("Orientation")
                currentIndex: settings.orientation
                onCurrentIndexChanged: settings.orientation = currentIndex
            }

            QuranTextSwitch {
                text: qsTr("Hide translations by default")
                checked: settings.translationsHidden
                onCheckedChanged: settings.translationsHidden = checked
            }

            QuranTextSwitch {
                text: qsTr("Night mode")
                checked: settings.nightMode
                onCheckedChanged: settings.nightMode = checked
            }

            QuranComboBox {
                model: ListModel {
                    ListElement { text: qsTr("Automatic") }
                    ListElement { text: qsTr("Center") }
                }

                label: qsTr("Text alignment")
                currentIndex: settings.centerText ? 1 : 0
                onCurrentIndexChanged: settings.centerText = (currentIndex == 1)
            }

            QuranTextSwitch {
                text: qsTr("Enable recitations")
                checked: settings.recitationMode == 1
                onCheckedChanged: settings.recitationMode = checked ? 1 : 0
            }

            QuranTextSwitch {
                text: qsTr("Flip phone to stop recitation")
                checked: settings.flipToStopRecitation
                onCheckedChanged: settings.flipToStopRecitation = checked
            }

            QuranBackgroundItem {
                width: parent.width
                height: quranTheme.sizes.itemSmall
                onClicked: deleter.confirm(qsTr("Resetting"), qsTr("Reset settings?"))
                QuranLabel {
                    anchors {
                        left: parent.left
                        leftMargin: quranTheme.sizes.marginSmall
                        right: parent.right
                        top: parent.top
                        bottom: parent.bottom
                    }
                    text: qsTr("Reset settings")
                    verticalAlignment: Text.AlignVCenter
                    color: parent.highlighted ? quranTheme.colors.primaryHighlight : quranTheme.colors.primary
                }
            }

            QuranBackgroundItem {
                width: parent.width
                height: quranTheme.sizes.itemSmall
                onClicked: pushAnimated(Qt.resolvedUrl("AboutPage.qml"))
                QuranLabel {
                    anchors {
                        left: parent.left
                        leftMargin: quranTheme.sizes.marginSmall
                        right: parent.right
                        top: parent.top
                        bottom: parent.bottom
                    }
                    text: qsTr("About")
                    verticalAlignment: Text.AlignVCenter
                    color: parent.highlighted ? quranTheme.colors.primaryHighlight : quranTheme.colors.primary
                }
            }
        }
    }
}
