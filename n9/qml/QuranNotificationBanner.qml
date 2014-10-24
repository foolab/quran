// -*- qml -*-
import QtQuick 1.0
import com.nokia.extras 1.0

InfoBanner {
    // This is a hack. Banners shown upon startup sometimes get positioned
    // incorrectly and the topmost part is hidden so we just push the whole banner down a bit
    topMargin: quranTheme.marginMedium * 3

    function showMessage(msg) {
        text = msg
        show()
    }
}
