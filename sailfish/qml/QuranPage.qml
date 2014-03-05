// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
        allowedOrientations: _settings.orientation == 1 ? Orientation.Portrait | Orientation.PortraitInverted : _settings.orientation == 2 ? Orientation.Landscape | Orientation.LandscapeInverted : Orientation.All
}
