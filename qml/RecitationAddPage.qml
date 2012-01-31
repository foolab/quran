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

        Flickable {
                id: flick
                width: parent.width

                clip: true

                anchors.top: title.bottom
                anchors.topMargin: 16
                anchors.bottom: toolBar.top
                anchors.bottomMargin: 16
                anchors.left: parent.left
                anchors.leftMargin: 16
                anchors.right: parent.right
                anchors.rightMargin: 16

                contentHeight: col.height

                Column {
                        id: col
                        width: parent.width

                        property string __path: _about.platform == "Maemo5" ? ".n900-quran/recitations/" : ".n9-quran/recitations/"

                        Label {
                                id: help
                                width: parent.width
                                text: "<b>To install recitation:</b><ul><li>Go to <a href='http://zekr.org/resources.html#recitation'>recitations download page</a> and download an offline recitation</li>                <li>Connect your phone to your PC in <i>mass storage mode</i></li><li>Copy the downloaded zip file to " + parent.__path + "</i><li>Restart this application.</li></ul><br />"

                                onLinkActivated: Qt.openUrlExternally(link);
                                color: colors.textColor
                        }

                        Label {
                                id: helpUnzipped
                                width: parent.width
                                text: "<b>To install an unzipped recitation:</b><ul><li>Go to <a href='http://zekr.org/resources.html#recitation'>recitations download page</a> and download an offline recitation</li>                <li>Extract the downloaded zip file. It will create a <i>recitation.properties</i> file and a directory</li><li>Connect your phone to your PC in <i>mass storage mode</i></li><li>Create a folder on the device underneath <i>" + parent.__path + "</i></li><li>Copy the result of the zip file extraction to the newly created directory</li><li>Restart this application.</li></ul><br />"

                                onLinkActivated: Qt.openUrlExternally(link);
                                color: colors.textColor
                        }

                        Label {
                                id: helpAdvanced
                                width: parent.width
                                text: "<b>Create a recitation:</b><ul><li>Follow the zekr recitations naming convention and format</li><li>All mp3 files should be placed in a directoryy underneath <i>" + parent.__path + "</i></li><li>Put a file called <i>info.ini</i> with the mp3 files and include the following content:<br />[recitation]<br />name=&lt;a unique name to identify the recitation&gt;</li><li>Restart the application</li></ul>"
                                color: colors.textColor
                        }
                }
        }

        ToolBar {
                id: toolBar
                ToolBarLayout {
                        ToolButton { icon: theme.pageBack; onClicked: pageStack.pop(); }
		        }
		}
}
