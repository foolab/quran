// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranListDelegate {
        id: contextLabel
        property int tid
        property bool showCategory

        property alias status: translation.status

        contentHeight: quranTheme.itemSizeLarge

        DeleteItem {
                id: deleter
                onConfirmed: _translations.removeTranslation(tid)
        }

        actions: [
        MenuAction {
                text: qsTr("Download")
                onClicked: _translations.startDownload(contextLabel.tid)
                visible: status == Translation.None || status == Translation.Error
        },
        MenuAction {
                text: qsTr("Stop")
                onClicked: _translations.stopDownload(contextLabel.tid)
                visible: status == Translation.Downloading
        },
        MenuAction {
                text: qsTr("Remove")
                onClicked: deleter.confirm(contextLabel, qsTr("Removing"), qsTr("Remove translation?"))
                visible: status == Translation.Installed
        }
        ]

        Item {
                anchors.fill: parent
                QuranStatusIndicator {
                        id: indicator
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        isInstalled: status == Translation.Installed
                        downloading: status == Translation.Downloading
                        error: status == Translation.Error
                }

                QuranLabel {
                        id: label
                        property string textWithCategory: qsTr("%1 - %2")
                        .arg(_translations.categoryNameForTranslation(contextLabel.tid))
                        .arg(_translations.translationName(contextLabel.tid))
                        property string textWidthoutCategory: _translations.translationName(contextLabel.tid)
                        text: showCategory ? textWithCategory : textWidthoutCategory

                        anchors {
                                top: parent.top
                                bottom: parent.bottom
                                right: indicator.left
                                rightMargin: quranTheme.marginSmall
                                left: parent.left
                        }

                        verticalAlignment: Text.AlignVCenter

                        Translation {
                                id: translation
                                tid: contextLabel.tid
                                translations: _translations
                                Component.onCompleted: init();
                        }
                }
        }
}
