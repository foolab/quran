// -*- qml -*-
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
                        text: qsTr("Reset")
                        onClicked: deleter.confirm(qsTr("Resetting"), qsTr("Reset settings?"))
                },
                MenuAction {
                        text: qsTr("Manage recitations")
                        onClicked: pushAnimated(Qt.resolvedUrl("RecitationsListPage.qml"))
                },
                MenuAction {
                        text: qsTr("Manage translations")
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
                                color: quranTheme.backgroundColor

                                QuranLabel {
                                        id: previewLabel
                                        width: parent.width
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        horizontalAlignment: Text.AlignHCenter
                                        font.family: settings.fontFamily
                                        font.pixelSize: settings.fontSize
                                        color: quranTheme.textColor

                                        Component.onCompleted: populate();

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
                                                onNumberFormatChanged: previewLabel.populate();
                                                onTextTypeChanged: previewLabel.populate();
                                        }
                                }
                        }

                        QuranSlider {
                                width: parent.width
                                label: qsTr("Text size")
                                minimumValue: quranTheme.minimumFontSize
                                maximumValue: quranTheme.maximumFontSize
                                valueText: value
                                stepSize: 1
                                value: settings.fontSize
                                onValueChanged: settings.fontSize = value
                        }

                        QuranSlider {
                                width: parent.width
                                label: qsTr("Translation text size");
                                minimumValue: quranTheme.minimumTranslationFontSize
                                maximumValue: quranTheme.maximumTranslationFontSize
                                valueText: value
                                stepSize: 1
                                value: settings.translationFontSize
                                onValueChanged: settings.translationFontSize = value
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
                                actions: [
                                        MenuAction { text: qsTr("Automatic") },
                                        MenuAction { text: qsTr("Portrait") },
                                        MenuAction { text: qsTr("Landscape") }
                                ]

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
                                actions: [
                                        MenuAction { text: qsTr("Automatic") },
                                        MenuAction { text: qsTr("Center") }
                                ]

                                label: qsTr("Text alignment");
                                currentIndex: settings.centerText ? 1 : 0;
                                onCurrentIndexChanged: settings.centerText = (currentIndex == 1);
                        }

                        QuranTextSwitch {
                                text: qsTr("Enable recitations")
                                checked: settings.recitationMode == 1
                                onCheckedChanged: settings.recitationMode = checked ? 1 : 0
                                enabled: _recitations.installedCount > 0
                        }

                        QuranTextSwitch {
                                text: qsTr("Flip phone to stop recitation")
                                checked: settings.flipToStopRecitation
                                onCheckedChanged: settings.flipToStopRecitation = checked
                        }
                }
        }
}
