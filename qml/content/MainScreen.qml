/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/

import QtQuick 2.9
import QtQuick.Controls 2.15



Rectangle {
    id: mainRect
    width: 700
    height: 780 // Constants.height
    property int opacitypopup: 0
    property int showList: 0

    // color: Constants.backgroundColor
    border.width: 0

//    MouseArea{
//        onClicked: focus = true
//    }

    Rectangle {
        id: headerRect
        x: 0
        width: 700
        height: 210
        color: "#ffffff"
        radius: 0
        border.width: 0
        anchors.top: parent.top
        anchors.topMargin: 0

        BorderImage {
            id: logoImage
            x: 10
            y: 5
            width: 200
            height: 200
            source:  "Risorse/Logo.bmp"
        }

        Text {
            id: releaseText
            x: 270
            y: 69
            text: "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Arial'; font-size:12pt;\">Tool QML Based. Device Finder on IP Network</span></p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Arial'; font-size:12pt;\">Release: 1.0.0 </span></p>\n<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>"
            font.pixelSize: 12
            layer.enabled: true
            textFormat: Text.RichText
            font.family: "Arial"
        }

        Text {
            id: wwwText
            x: 270
            text: "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a name=\"https://www.tecna.net\"></a><a href=\"https://www.tecna.net\"><span style=\" font-family:'Arial'; font-size:12pt; text-decoration: underline; color:#007af4;\">https://www.tecna.net</span></a><span style=\" font-family:'Arial'; font-size:12pt; text-decoration: underline; color:#007af4;\"> </span></p></body></html>"
            anchors.top: releaseText.bottom
            anchors.topMargin: 1
            font.pixelSize: 12
            layer.enabled: true
            textFormat: Text.RichText
            font.family: "Arial"
            onLinkActivated: Qt.openUrlExternally(link)
        }
    }

    ToolBar {
        id: mainCmdtoolBar
        x: 0
        width: 700
        height: 50
        anchors.top: headerRect.bottom
        anchors.topMargin: -1

        ToolButton {
            id: scantoolButton
            x: 0
            y: 2
            width: 150
            height: 50
            text: qsTr("Scan Network")
            font.family: "Arial"
            font.pointSize: 10
            flat: true
            // highlighted: false

            onClicked: {
                if (showList === 0){
                    showList = 1;
                } else {
                    showList = 0;
                    enableButtons(showList)
                }

            }
        }

        ToolButton {
            id: setDHCPButton
            x: 151
            width: 150
            text: qsTr("Set DHCP")
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            enabled: false
            anchors.bottomMargin: 0
            anchors.topMargin: 0
            onClicked: {
                dhcpClicked();
            }
        }

        ToolButton {
            id: setIPButton
            x: 302
            y: 0
            width: 150
            height: 50
            text: qsTr("SET IP ")
            enabled: false
            onClicked: {
                setIpClicked();
            }
        }
    }

    Rectangle {
        id: spacerRect
        anchors.top: mainCmdtoolBar.bottom
        width: 700
        height: 5
    }

    Rectangle {
        id: listRect
        anchors.top: spacerRect.bottom
        anchors.topMargin: 0
        width: 700
        height: 35
        border.width: 0
        Row { // The "Row" type lays out its child items in a horizontal line
            id: dataTitleRow
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.topMargin: 10
            spacing: 20 // Places 20px of space between items
             Rectangle { id: devNameTitleRect; width: 80; height: 20;
                 // color: "white";
                Text { id: devNameTitleText; width: 80; height: 20; color: "black"; text: "Device Name" }
             }
            Rectangle { id: ipAddrTitleRect; width: 160; height: 20; color: "red"
                Text { id: ipAddrTitleText; width: 160; height: 20; color: "black"; text: "IP Address"; }
            }
            Rectangle { id: macAddrTitleRect; width: 200; height: 20; color: "green"
                Text { id: macAddrTitleText; width: 200; height: 20; color: "black"; text: "MAC Address"; }
            }
            Rectangle { id: dhcpTitleRect; width: 120; height: 20; color: "blue"
                Text { id: dhcpTitleText; width: 120; height: 20; color: "black"; text: "DHCP Used"; }
            }
        }

    }

    Rectangle {
        id: scrollAreaRect
        width: 700
        anchors.top: listRect.bottom
        height: parent.height - headerRect.height - mainCmdtoolBar.height -spacerRect.height - dataTitleRow.height - footerRect.height -10// Constants.height
        //color:  Constants.backgroundColor
        color: "#c2c2c2";

        border.width: 0

        ScrollView {
            id: scroller
            anchors.fill: parent
            clip: true
            height: mainRect.height
            anchors.topMargin: 25
            anchors.leftMargin: 10
            anchors.rightMargin: 10


            ScrollBar.vertical.policy: ScrollBar.AlwaysOn
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOn

            ListView {
                id: scrollingListView
                width: parent.width
                height: parent.height
                // model: DeviceModel {}
                opacity: showList

                property var currentSelectedItem
                property string currentSelectedName: ""



                onCurrentIndexChanged: {
                    console.log("Current index now is: " + currentIndex);
                    currentSelectedItem = myDelegateModel.items.get(currentIndex).model;
                    console.log("Item is: " + currentSelectedItem.devName);
                    currentSelectedName = currentSelectedItem.devName
                }

                model: DelegateModel {
                        id: myDelegateModel
                        model: DeviceModel {}




                delegate:

                    ItemDelegate {
                        width: scrollingListView.width
                        id: myDelegate
                        height: 25
                        highlighted: ListView.isCurrentItem

                        // generate the background
                            Rectangle {
                                id: background
                                x: 2; y: 2; width: parent.width - x*2; height: parent.height - y*2
                                border.color: "orange"
                                radius: 5
                                color: "lightgrey"
                            }

                            Row { // The "Row" type lays out its child items in a horizontal line
                                id: dataRow
                                // anchors.left: parent.left
                                spacing: 20 // Places 20px of space between items
                                height: 20



                                 Rectangle {
                                     id: devNameRect; width: 80; height: 25;
                                     // color: "#c2c2c2";
                                     color: "transparent";
                                     Text {
                                         id: devNameText;
                                         width: parent.width;
                                         height: parent.height;
                                         color: "black";
                                         text: devName
                                         // text: "Item " + (index + 1);
                                         // anchors.fill: parent


                                     } // end Text



                                     MouseDC{
                                        anchors.fill: parent
                                        privRightClick: openInfo
                                        privLeftClick: enableButtons
                                        privGenClick: genericClick
                                        privIndex: index
                                        onClicked: {
                                            // scrollingListView.currentIndex = index

                                        }
                                     } // end MouseDC


                                 } //end rectangle


                                Rectangle {
                                    id: ipAddrRect;
                                    width: 160;
                                    height: 20;
                                    color: "transparent";

                                    Text {
                                        id: ipAddrText;
                                        width: parent.width;
                                        height: parent.height;
                                        color: "black";
                                        text: "IP Addr: " + ipAddress;
                                    }
                                    MouseDC{
                                        anchors.fill: parent
                                        privRightClick: openInfo
                                        privLeftClick: enableButtons
                                        privGenClick: genericClick
                                        privIndex: index
                                        onClicked: {
                                            // scrollingListView.currentIndex = index
                                        }
                                    } // end MouseDC
                                }
                                Rectangle { id: macAddrRect; width: 200; height: 20;
                                    color: "transparent";
                                    Text {
                                        id: macAddrText;
                                        width: parent.width;
                                        height: parent.height;
                                        color: "black";
                                        text: "MAC Addr: " + macAddress;
                                    }

                                    MouseDC{
                                        anchors.fill: parent
                                        privRightClick: openInfo
                                        privLeftClick: enableButtons
                                        privGenClick: genericClick
                                        privIndex: index
                                        onClicked: {
                                            // scrollingListView.currentIndex = index
                                            console.log("Index is: " + index)
                                        }
                                    } // end MouseDC
                                }
                                Rectangle { id: dhcpRect; width: 120; height: 20;
                                    color: "transparent";
                                    Text { id: dhcpText;
                                        width: parent.width;
                                        height: parent.height;
                                        color: "black"; text: dhcpUsed;
                                    }
                                    MouseDC{
                                        anchors.fill: parent
                                        privRightClick: openInfo
                                        privLeftClick: enableButtons
                                        privGenClick: genericClick
                                        privIndex: index
                                        onClicked: {
                                            // scrollingListView.currentIndex = index
                                        }
                                    } // end MouseDC

                                }


                            } // end row
                    } // end itemDelegate

               }


            } //end List View
        }
    }

    Rectangle {
        id: footerRect
        anchors.top: scrollAreaRect.bottom
        width: 700
        height: 5
        color: "#c2c2c2"
    }

    Text {
        id: maintitleText
        x: 268
        y: 30
        opacity: 1
        visible: true
        color: "#000000"
        text: qsTr("TECNA DEVICE FINDER")
        layer.enabled: false
        font.styleName: "Normal"
        enabled: true
        clip: false
        font.family: "Arial"
        font.pointSize: 18
        font.weight: Font.Bold
    }

//    PopUpDevice {
//        id: popUpDev
//        opacitypopupitem: opacitypopup
//        title: "Device Info"
//    }

    PopUpInerithed {
        id: popUpDev
        opacitypopupitem: opacitypopup
        title: "Device Info"
        description: "Here we get the device info"
    }

    PopUpDHCP {
        id: popUpDHCP
        opacitypopupitem: 0
        title: "DHCP Setting"
        description: "Would you enable DHCP for this device?"
        onOpacitypopupitemChanged: {
             console.log("DHCP Child opacity is: " + opacitypopupitem)

            if (opacitypopupitem === 0){
                console.log("DHCP disappears")

                var myModel = myDelegateModel.model;
                myModel.setProperty(scrollingListView.currentIndex, "dhcpUsed",dhcpUsed)
                // console.log("DHCP is used: " + scrollingListView.currentSelectedItem.dhcpUsed)
            } else {

                console.log("DHCP Appears")
            }
        }
    }


    PopUpSetIP {
        id: popUpSetIp
        opacitypopupitem: 0
        title: "IP Setting"
        description: "Please set IP for this device"
        onOpacitypopupitemChanged: {
             console.log("SET IP Child opacity is: " + opacitypopupitem)

            if (opacitypopupitem === 0){
                console.log("SET IP disappears")

                var myModel = myDelegateModel.model;
                myModel.setProperty(scrollingListView.currentIndex, "ipAddress",ipAddrSet)
                myModel.setProperty(scrollingListView.currentIndex, "dhcpUsed","No")
                myModel.setProperty(scrollingListView.currentIndex, "netMask",ipNetMaskSet)
            } else {
                console.log("SET IP Appears")
            }
        }
    }


    function enableButtons(_enable){
        if (_enable === 1){
            console.log ("Enable buttons")
            setDHCPButton.enabled = true
            setIPButton.enabled = true
        } else {
            console.log ("Disable buttons")
            setDHCPButton.enabled = false
            setIPButton.enabled = false
        }
    } // end function

    function openInfo(){
        console.log("Open Info")
        opacitypopup = 1
        popUpDev.opacitypopupitem = opacitypopup
        popUpDev.title = scrollingListView.currentSelectedName   //.currentIndex
        console.log("Current item is: " + scrollingListView.currentSelectedItem)
        popUpDev.open()
    } // end function

    function genericClick(_index){
        scrollingListView.currentIndex = _index
        console.log("List Index is: " + scrollingListView.currentIndex)

    }// end function


    function dhcpClicked(){
        popUpDHCP.opacitypopupitem = 1
        popUpDHCP.description = "Would you enable DHCP for device: " + scrollingListView.currentSelectedName + " ?"
        popUpDHCP.open()
    } // end function

    function setIpClicked(){
        popUpSetIp.opacitypopupitem = 1
        popUpSetIp.description = "Would you like to set IP for device: " + scrollingListView.currentSelectedName + " ?"
        popUpSetIp.open()
    } // end function


}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:13;annotation:"1 //;;//  //;;//  //;;// <!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">fdfdfdfdfdfdfd</p></body></html> //;;// 1660037569";customId:""}
}
##^##*/
