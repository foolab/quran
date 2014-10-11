// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranPage {
        id: translationsAddPage

        property int cid

        Component {
                id: delegate

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
                                text: _translations.translationName(translationId)
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

        QuranListView {
                id: view
                anchors.fill: parent
                model: TranslationCollection {
                        translations: _translations
                        language: translationsAddPage.cid
                }

                delegate: delegate
                header: QuranPageHeader {
                        width: parent.width
                        title: _translations.categoryName(translationsAddPage.cid)
                }
        }
}
