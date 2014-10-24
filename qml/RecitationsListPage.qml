// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranPage {
	    id: recitationsListPage
        menu: pageMenu

        Component {
                id: recitationssDelegate
                // TODO: Context menu
                ListDelegate {
                        contentHeight: quranTheme.itemSizeLarge
                        Item {
                                width: parent.width
                                height: quranTheme.itemSizeLarge

                                QuranStatusIndicator {
                                        id: indicator
                                        anchors.right: parent.right
                                        anchors.verticalCenter: parent.verticalCenter
                                        isInstalled: true
                                }

                                QuranLabel {
                                        text: name
                                        anchors {
                                                top: parent.top
                                                bottom: parent.bottom
                                                right: indicator.left
                                                rightMargin: quranTheme.marginSmall
                                                left: parent.left
                                        }

                                        verticalAlignment: Text.AlignVCenter
                                }
                        }
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
