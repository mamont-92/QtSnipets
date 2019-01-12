import QtQuick 2.6
import QtQuick.Window 2.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Rectangle {
        width: 200
        height: 200
        Column {
            Rectangle {
                id: redRect
                color: "red"
                x:0
                y:0
                width: 50
                height: 50
                DropArea {
                    anchors.fill: parent
                    onEntered: { console.log("red-enter") }
                    onDropped: { console.log("red-released") }
                }
            }
            Rectangle {
                id: greyRect
                color: "grey"
                x:100
                y:0
                width: 50
                height: 50

                Drag.active: mousearea.drag.active
                Drag.hotSpot.x: mousearea.mouseX
                Drag.hotSpot.y: mousearea.mouseY
                MouseArea {
                    id: mousearea
                        anchors.fill: parent
                        onReleased: dargItem.Drag.drop()
                        drag.target: dargItem
                        /*Item {
                            id: dargItem
                            x: mousearea.mouseX
                            y: mousearea.mouseY
                            width: 1; height: 1
                            Drag.active: mousearea.drag.active
                            Drag.hotSpot.x: 1
                            Drag.hotSpot.y: 1
                        }*/
                        Rectangle {
                            color: "black"
                            id: dargItem
                            x: mousearea.mouseX
                            y: mousearea.mouseY
                            width: 10;
                            height: 10
                            visible: mousearea.drag.active
                            Drag.active: mousearea.drag.active
                            Drag.hotSpot.x: 1
                            Drag.hotSpot.y: 1
                        }
                }
            }
        }
    }
}
