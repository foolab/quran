// -*- qml -*-
import QtQuick 2.2
import QtQuick.Controls 1.2

Rectangle {
    property bool downloading: false
    property bool isInstalled: false
    property bool error: false
    property bool _dimmed: !(isInstalled || downloading || error)
    color: downloading ? quranTheme.statusDownloading : error ? quranTheme.statusError : quranTheme.statusInstalled
    width: quranTheme.itemSizeSmall / 2
    height: width
    radius: width / 3
    opacity: _dimmed ? 0.3 : 1.0
    smooth: true
}
