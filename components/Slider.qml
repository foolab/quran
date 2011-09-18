// -*- qml -*-
import QtQuick 1.0

Rectangle {
//        color: "black" // For debugging
        property bool interactive: true
        width: 350
        height: interactive ? knob.height : 50

        id: slider

        // TODO: this is ignored.
        property bool valueIndicatorVisible
        // TODO: this is ignored.
        property int orientation: Qt.Horizontal

        property int minimumValue: 0
        property int maximumValue: 0

        // TODO: this is ignored.
        property int stepSize: 1

        property int value
        property alias pressed: mouse.pressed

        function positionForValue(val) {
                var res = (val - minimumValue) * (groove.width / (maximumValue - minimumValue));
                return res;
        }

        function valueForPosition(pos) {
                var res = (pos * (maximumValue - minimumValue)/groove.width) + minimumValue;
                return res;
        }

        MouseArea {
                anchors.fill: parent
                enabled: slider.interactive

                id: mouse
                acceptedButtons: Qt.LeftButton
                drag.target: knob
                drag.axis: Drag.XAxis
                drag.minimumX: groove.x - knob.width
                drag.maximumX: groove.x + groove.width - knob.width

                onPositionChanged: {
                        var click = mapToItem(groove, mouse.x, mouse.y).x;
                        click = Math.max(Math.min(click, groove.width), 0);
                        slider.value = valueForPosition(click);
                }
        }

        Rectangle {
                id: groove
                width: interactive ? parent.width - knob.width : parent.width * value/maximumValue
                x: knob.width/2
                anchors.verticalCenter: parent.verticalCenter
                height: 25
                color: "steelblue"
                smooth: true
                gradient: Gradient {
                        GradientStop { position: 0.0; color: "white" }
                        GradientStop { position: 0.5; color: "steelblue" }
                        GradientStop { position: 1.0; color: "white" }
                }
//        }

        Rectangle {
                id: knob
                width: slider.interactive ? 50 : 0
                height: slider.interactive ? 50 : 0
                x: positionForValue(slider.value) - width/2


//                var pos = x + width/2
//                slider.value = valueForPosition(pos);
/*
                onXChanged: {
                        if (slider.minimumValue == 0 && slider.maximumValue == 0) {
                        return;
                        }
                        var pos = x + (width/2);
                        var val = valueForPosition(pos);
*/
//                        slider.value = val;
//                        console.log("x = " + x + " pos = " + pos + " value = " + val + " min " + slider.minimumValue);
//                        }
//                onXChanged: {
//                        // We ignore step size.
//                        var pos = x + (width/2);
//                        var r = slider.maximumValue - slider.minimumValue + 1;
//                        var len = groove.x - pos;
                        //
//                }
                //
//                border.width: 1
//                border.color: "black"
                radius: 50
//                smooth: true
//                rotation: 45
                anchors.verticalCenter: groove.verticalCenter
                color: "steelblue"
//                gradient: Gradient {
//                        GradientStop { position: 0.0; color: "steelblue" }
//                        GradientStop { position: 0.5; color: "white" }
//                        GradientStop { position: 1.0; color: "steelblue" }
                }
        }
}
