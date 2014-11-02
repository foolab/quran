// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranPage {
    menu: pageMenu

    QuranPageMenu {
        id: pageMenu
        view: flick
        actions: [
            MenuAction {
                text: qsTr("Manage translations")
                onClicked: pushAnimated(Qt.resolvedUrl("TranslationAddPage.qml"))
            }
        ]
    }

    QuranListView {
        id: view
        anchors.fill: parent

        header: QuranPageHeader {
            width: parent.width
            title: qsTr("Choose translation")
        }

        model: _translations

        delegate: ListDelegate {
            id: item
            visible: translation.status == Translation.Installed

            onClicked: {
                if (_translations.loadTranslation(translation.uuid)) {
                    settings.defaultTranslation = translation.uuid
                    popPage()
                } else {
                    banner.showMessage(qsTr("Failed to load translation"))
                }
            }

            QuranLabel {
                visible: item.visible
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
