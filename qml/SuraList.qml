// -*- qml -*-
import QtQuick 2.0

Grid {
        property variant suras: []
        rows: 1
        columns: suras.length
        spacing: quranTheme.spacing

        Repeater {
                model: suras
                QuranLabel {
                        // Hack: we need to reverse them
                        Component.onCompleted: text = suras[suras.length - index - 1];
                        color: quranTheme.textColor
                }
        }
}
