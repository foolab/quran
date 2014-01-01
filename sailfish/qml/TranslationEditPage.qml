// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

TranslationsPage {
        id: translationEditPage

        property int tid: 0

        tools: toolBar

        TitleLabel {
                id: title
                width: parent.width
                anchors.top: parent.top
                text: _translations.translationName(parent.tid);
        }

        DialogButton {
                text: qsTr("Delete translation");
                width: 400
                anchors.top: title.bottom
                anchors.topMargin: 30
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: askForRemoval(tid);
        }

        ToolBar {
                id: toolBar
                ToolBarLayout {
                        ToolButton { icon: theme.pageBack; onClicked: pageStack.pop(); }
		        }
		}
}
