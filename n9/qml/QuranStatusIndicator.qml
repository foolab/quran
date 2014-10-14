// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.0

Rectangle {
        property bool downloading: false
        property bool isInstalled: false
        property bool error: false
        property bool _dimmed: !(isInstalled || downloading || error)
        color: downloading ? theme.statusDownloading : error ? theme.statusError : theme.statusInstalled
        width: 10
        height: 10
        radius: 10/2
        opacity: _dimmed ? 0.5 : 1.0
}
