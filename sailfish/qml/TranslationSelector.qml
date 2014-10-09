// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0
import Quran 1.0

Dialog {
        id: dialog

        DialogHeader {
                id: header
                title: qsTr("Choose translation")
                acceptText: title
        }

        SilicaListView {
                id: view
                model: InstalledTranslationsModel {
                        translations: _translations
                }

                anchors {
                        top: header.bottom
                        bottom: parent.bottom
                        right: parent.right
                        left: parent.left
                }

                delegate: BackgroundItem {
                        property int tid: translationId
                        onClicked: {
                                translationsManager.changeTranslation(tid)
                                pageStack.pop()
                        }

                        anchors {
                                left: parent.left
                                leftMargin: theme.marginMedium
                                right: parent.right
                                rightMargin: theme.marginMedium
                        }

                        height: Theme.itemSizeSmall

                        Label {
                                anchors.fill: parent
                                wrapMode: Text.WordWrap
                                truncationMode: TruncationMode.Fade
                                text: name
                                color: _translations.current == tid ? Theme.highlightColor : Theme.primaryColor
                                font.family: Theme.fontFamily
                        }
                }
        }
}
