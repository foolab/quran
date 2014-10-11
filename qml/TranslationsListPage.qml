// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranPage {
        Component {
                id: translationsDelegate

                ContextMenuLabel {
                        id: contextLabel
                        property alias status: translation.status
                        // TODO:
//                        contentHeight: label.height
                        DeleteItem {
                                id: deleter
                                onConfirmed: _translations.removeTranslation(translationId)
                        }

                        actions: [
                        MenuAction {
                                text: qsTr("Download")
                                onClicked: _translations.startDownload(translationId)
                                visible: status == Translation.None || status == Translation.Error
                        },
                        MenuAction {
                                text: qsTr("Stop")
                                onClicked: _translations.stopDownload(translationId)
                                visible: status == Translation.Downloading
                        },
                        MenuAction {
                                text: qsTr("Remove")
                                onClicked: deleter.confirm(contextLabel, qsTr("Removing"))
                                visible: status == Translation.Installed
                        }
                        ]

                        content: [
                        QuranStatusIndicator {
                                id: indicator
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                isInstalled: status == Translation.Installed
                                downloading: status == Translation.Downloading
                                error: status == Translation.Error
                        },
                        QuranLabel {
                                id: label
                                text: _translations.categoryNameForTranslation(translationId) + " - " + _translations.translationName(translationId)
                                anchors {
                                        top: parent.top
                                        bottom: parent.bottom
                                        right: indicator.left
                                        rightMargin: theme.marginSmall
                                        left: parent.left
                                        leftMargin: theme.marginSmall
                                }

                                wrapMode: Text.WordWrap
                                verticalAlignment: Text.AlignVCenter

                                Translation {
                                        id: translation
                                        tid: translationId
                                        translations: _translations
                                        Component.onCompleted: init();
                                }
                        }
                ]
                }
        }

        QuranPageMenu {
                view: view

                actions: [
                MenuAction {
                        text: qsTr("Add translation")
                        onClicked: pushAnimated(Qt.resolvedUrl("TranslationLanguageList.qml"))
                }
                ]
        }

        QuranListView {
                id: view
                anchors.fill: parent
                model: ActiveTranslationsModel {
                        translations: _translations
                }

                delegate: translationsDelegate
                header: QuranPageHeader {
                        width: parent.width
                        title: qsTr("Available translations")
                }

                QuranViewPlaceholder {
                        text: qsTr("No translations. Pull down to add a translation.")
                        enabled: view.count == 0
                }
        }
}
