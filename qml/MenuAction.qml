// -*- qml -*-
import QtQuick 2.0

QtObject {
    property string text
    signal clicked
    property bool visible: true

    // Workaround: needed for Harmattan's default SelectionDialog delegate
    property string name: text
}
