// -*- qml-mode -*-
import QtQuick 1.0
import Translations 1.0

TranslationsPage {
        id: translationEditPage

        property int tid: 0

        tools: toolBar

        Label {
                id: name
                text: _translations.translationName(tid);
                font.pointSize: 22
                width: parent.width - 20
                x: 10
                y: 10
                horizontalAlignment: Text.AlignHCenter
        }

        DialogButton {
                text: qsTr("Delete translation");
                width: 400
                anchors.top: name.bottom
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
