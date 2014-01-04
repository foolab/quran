// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
        id: dialog

        Connections {
                target: _translations
                onInstalledChanged: model.populate()
                onCurrentChanged: model.currentChanged()
        }

        DialogHeader {
                id: header
                title: qsTr("Choose translation")
                acceptText: title
        }

        SilicaListView {
                id: view
                model: _translations.installed

                anchors {
                        top: header.bottom
                        bottom: parent.bottom
                        right: parent.right
                        left: parent.left
                }

                delegate: BackgroundItem {
                        property int tid: _translations.installed[modelData]
                        onClicked: {
                                translationsManager.changeTranslation(tid)
                                pageStack.pop()
                        }

                        anchors {
                                left: parent.left
                                leftMargin: 16
                                right: parent.right
                                rightMargin: 16
                        }

                        height: Theme.itemSizeSmall

                        Label {
                                anchors.fill: parent
                                wrapMode: Text.WordWrap
                                truncationMode: TruncationMode.Fade
                                text: _translations.translationName(tid)
                                color: _translations.current == tid ? Theme.highlightColor : Theme.primaryColor
                        }
                }
        }
}
