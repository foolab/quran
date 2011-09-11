// -*- qml -*-
import QtQuick 1.0

Page {
	    id: recitationInstallPage

        tools: toolBar

        TitleLabel {
                id: title
                width: parent.width
                anchors.top: parent.top
                text: qsTr("Howto install recitations")
        }

        // TODO:
        ToolBar {
                id: toolBar
                ToolBarLayout {
                        ToolButton { icon: theme.pageBack; onClicked: pageStack.pop(); }
		        }
		}
}
