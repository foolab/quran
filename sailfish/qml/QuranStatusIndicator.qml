// -*- qml -*-
import Sailfish.Silica 1.0

GlassItem {
        property bool downloading: false
        property bool isInstalled: false
        property bool error: false
        dimmed: !(isInstalled || downloading || error)
        color: downloading ? quranTheme.statusDownloading : error ? quranTheme.statusError : quranTheme.statusInstalled
}
