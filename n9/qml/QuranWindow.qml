// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
    Binding {
        value: settings.nightMode
        property: "inverted"
        target: theme
    }
}
