// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
        allowedOrientations: settings.orientation == 1 ? Orientation.Portrait | Orientation.PortraitInverted : settings.orientation == 2 ? Orientation.Landscape | Orientation.LandscapeInverted : Orientation.All
}
