// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranPage {
	    id: recitationsListPage
        menu: pageMenu

        Component {
                id: recitationssDelegate
                // TODO: Context menu
                ContextMenuLabel {
                        content: [
                        QuranStatusIndicator {
                                id: indicator
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                isInstalled: true
                        },
                        QuranLabel {
                                text: name
                                anchors {
                                        top: parent.top
                                        bottom: parent.bottom
                                        right: indicator.left
                                        rightMargin: theme.marginSmall
                                        left: parent.left
                                        leftMargin: theme.marginSmall
                                }

                                verticalAlignment: Text.AlignVCenter
                        }
                        ]
                }
        }

        QuranPageMenu {
                id: pageMenu
                view: view
                actions: [
                MenuAction {
                        text: qsTr("Add recitation")
                        onClicked: pushAnimated(Qt.resolvedUrl("RecitationAddPage.qml"))
                }
                ]
        }

        QuranListView {
                id: view
                anchors.fill: parent
                model: RecitationModel {
                        recitations: _recitations
                }

                delegate: recitationssDelegate

                header: QuranPageHeader {
                        width: parent.width
                        title: qsTr("Recitations")
                }

                QuranViewPlaceholder {
                        text: qsTr("No recitations. Pull down to enable a recitation.")
                        enabled: view.count == 0
                }
        }
}
