// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.0

Rectangle {
        property bool downloading: false
        property bool isInstalled: false
        property bool error: false
        property bool _dimmed: !(isInstalled || downloading || error)
        color: downloading ? quranTheme.statusDownloading : error ? quranTheme.statusError : quranTheme.statusInstalled
        width: 32
        height: 32
        radius: 10
        opacity: _dimmed ? 0.5 : 1.0
}
