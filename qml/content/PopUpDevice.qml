import QtQuick 2.0
import QtQuick.Controls

Item {
    property int opacitypopupitem: 0
    property string title : "Unknown title"



        Rectangle{
            id: popUpRect
            width: 400
            height: 400
            color: "green"
            opacity: opacitypopupitem
            enabled: opacitypopupitem
            Text {
                id: popUpText
                anchors
                {
                    centerIn:parent
                }
                text: qsTr("POP UP")
                font.family: "Segoe UI Light"
                font.pixelSize: 20
                color: "darkgrey"
            }

            MouseArea
            {
                anchors.fill: parent

                onClicked:
                {
                    opacitypopupitem = 0
                    console.log("opacitypopupitem = ", opacitypopupitem)
                }
            } // end MouseArea
        } // end popUpRect




} // end item
