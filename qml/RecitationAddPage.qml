// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranPage {
	    id: recitationInstallPage

        Component {
                id: recitationssDelegate
                // TODO: Context menu
                QuranListDelegate {
                        contentHeight: quranTheme.itemSizeLarge
                        onClicked: toggleRecitation(recitationId, installed)
                        Item {
                                anchors.fill: parent
                                QuranStatusIndicator {
                                        id: indicator
                                        anchors.right: parent.right
                                        anchors.verticalCenter: parent.verticalCenter
                                        isInstalled: installed
                                }

                                QuranLabel {
                                        text: qsTr("%1 (%2)").arg(name).arg(quality)
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

        function toggleRecitation(rid, installed) {
                if (installed) {
                        _recitations.disableInstallable(rid)
                }
                else {
                        _recitations.enableInstallable(rid)
                }
        }

        QuranListView {
                anchors.fill: parent

                model: InstallableRecitationsModel {
                        recitations: _recitations
                }

                delegate: recitationssDelegate

                header: QuranPageHeader {
                        width: parent.width
                        title: qsTr("Enable online recitations")
                }

        }
}
