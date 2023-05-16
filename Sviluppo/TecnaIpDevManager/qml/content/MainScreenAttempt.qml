/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/

import QtQuick 2.9
import QtQuick.Controls 2.15
import QtQuick
import QtQuick.Layouts
// import io.qt.tp_view_object_helper 1.0
// import io.qt.tp_view_object 1.0
// import io.qt.awi_helper_config_awi 1.0

ApplicationWindow {
    id:window
    visible: true
    minimumWidth: 1000
    minimumHeight: 600
    title: qsTr("TECNA IP Config")
    property int opacitypopup: 0
    property int showList:  0
    property string fontFamily: "Arial"
    property string swVersion: "1.0.1"
    property int pointSize:10

    property var tp_view_object_helper_prop: (tp_view_object_helper != null) ?
                                                 tp_view_object_helper : null
    property var net_if: (tp_view_object_helper != null) ?
                             tp_view_object_helper.getInterfaces() : null

    property int ifIsSelectd: popUpSetIf.isSelected
    // Uncomment if you want combobox ip property int ipIsSelectd: popUpSetIf.isIPSelected

    property var tp_proc_end: (tp_view_object_helper != null) ?
                                  tp_view_object_helper.procTerminated : null

    RowLayout {
        id: mainLayout
        width: parent.width
        height: parent.height

        ToolBar {
        id: mainCmdtoolBar
        // anchors.fill: parent

        RowLayout {
            id: toolbarLayout
             anchors.fill: parent

             RowLayout {
                id: toolbarColumnLayout
                // anchors.fill: parent
                ToolButton {
                    id: scantoolButton
                    // x: 0
                    // y: 2
                    width: 150
                    height: 50
                    text: qsTr("Scan Network")
                    font.family: fontFamily
                    font.pointSize: pointSize + 1
                    flat: true
                    // highlighted: false

                    onClicked: {
                        if (tp_view_object_helper_prop !== null){
                            if (tp_view_object_helper_prop.socketState() === true){
                                startBroadcast()
                                prepareTimer.start()
                            } else {
                                openSetUpIf()
                            }
                        } // null
                    } // OnClicked
                } // end scantoolButton

                ToolButton {
                    id: selectLANButton
                    // anchors.left: scantoolButton.right  // No anchors if in layout
                    width: 150
                    height: 50
                    text: qsTr("LAN SELECT")
                    font.family: fontFamily
                    font.pointSize: pointSize + 1
                    flat: true
                    visible: false

                    onClicked: {
                          openSetUpIf()
                    } // OnClicked
                 } // end selectLANButton

            } // toolbarColumnLayout
            ToolButton {
                id: settingBtn
                // anchors.top: parent.top      // No anchors if in layout
                // anchors.topMargin: 10        // No anchors if in layout
                // anchors.right: parent.right  // No anchors if in layout
                Layout.alignment: Qt.AlignRight
                Layout.fillHeight: true
                width: 50
                height: 40
                // text: qsTr("SETTINGS")       // If you want text
                Image {
                        source: "qrc:/qml/content/Risorse/setUpIcon.png"
                        fillMode: Image.PreserveAspectFit // For not stretching image (optional)
                        anchors.fill: parent
                        anchors.margins: 2 // Leaving space between image and borders (optional)
                        anchors.centerIn: parent
                        Component.onCompleted: {
                            // console.log ("Image completed")
                            // console.log  (source)
                        }
                } // Image
                onClicked: {
                    if (selectLANButton.visible === false){
                        selectLANButton.visible = true;
                    } else {
                        selectLANButton.visible = false;
                    }
                } // end onClicked
            } // end settingBtn

        } // end toolbarLayout

        } // End toolbar header


        Rectangle {
            id: mainRect
            // anchors.fill: parent
            border.width: 5
            border.color: "red"  // Debug line

            Rectangle {
                id: listRect
                x: parent.x
                y: parent.y + 2
                // anchors.top: mainRect.bottom
                // anchors.margins: 2
                width:  parent.width // 700
                height: 35
                border.width: 2
                border.color: "blue"  // Debug line


                RowLayout { // The "Row" type lays out its child items in a horizontal line
                    id: dataTitleRow
                    anchors.fill: parent
                    Rectangle {
                        id: devNameTitleRect;
                        Layout.fillWidth: true
                        Layout.minimumWidth: 50
                        Layout.preferredWidth: 10 * pointSize
                        Layout.maximumWidth: 300
                        Layout.minimumHeight: 20
                        Text {
                            id: devNameTitleText;
                            anchors.centerIn: parent
                            color: "black";
                            text: "Device Name"
                            font.family: fontFamily
                            font.pointSize: pointSize
                        }
                    } // end devNameTitleRect
                    Rectangle {
                        id: ipAddrTitleRect;
                        Layout.fillWidth: true
                        Layout.minimumWidth: 50
                        Layout.preferredWidth: 10 * pointSize
                        Layout.maximumWidth: 300
                        Layout.minimumHeight: 20
                        Text {
                            id: ipAddrTitleText;
                            anchors.centerIn: parent
                            color: "black";
                            text: "IP Address";
                            font.family: fontFamily
                            font.pointSize: pointSize
                        }
                    } // end ipAddrTitleRect
                    Rectangle {
                        id: macAddrTitleRect;
                        Layout.fillWidth: true
                        Layout.minimumWidth: 50
                        Layout.preferredWidth: 10 * pointSize
                        Layout.maximumWidth: 300
                        Layout.minimumHeight: 20
                        Text {
                            id: macAddrTitleText;
                            anchors.centerIn: parent
                            color: "black";
                            text: "MAC Address";
                            font.family: fontFamily
                            font.pointSize: pointSize
                        }
                    } // end macAddrTitleRect
                    Rectangle {
                        id: dhcpTitleRect;
                        Layout.fillWidth: true
                        Layout.minimumWidth: 50
                        Layout.preferredWidth: 10 * pointSize
                        Layout.maximumWidth: 300
                        Layout.minimumHeight: 20
                        Text {
                            id: dhcpTitleText;
                            anchors.centerIn: parent
                            color: "black";
                            text: "DHCP Enabled";
                            font.family: fontFamily
                            font.pointSize: pointSize
                        }
                    } // end dhcpTitleRect
                } // end dataTitleRow
            }


        }
    }
    footer: Rectangle {
        id: footerRect
        width: parent.width // 700
        height: 20
        color: "gray"
        border.width: 1

        Text {
            id: footerText;
            anchors.centerIn: parent
            // anchors.fill: parent
            color: "black";
            text: "TECNA IP CONFIG Version: " + swVersion;
            font.family: fontFamily
            font.pointSize: pointSize + 1
        }
    }

    Component.onCompleted: {
        console.log (window.contentItem)
    }

/*
    RowLayout {
        anchors.top: parent.header.bottom
        id: mainLayout
        width: parent.width
        height: parent.height

*/
        /*
        Rectangle {
            id: mainRect
            width: mainLayout.implicitWidth
            height: mainLayout.implicitHeight
            // Layout.fillWidth: true
            // Layout.fillHeight: true
            // Layout.minimumWidth: 60 * pointSize // dataTitleRow.width
            // Layout.preferredHeight:  scrollingListView.count !== 0 ?scrollingListView.count*(pointSize + 10) : 400

            Layout.minimumHeight: 400
            // Layout.alignment: Qt.AlignTop;
            border.width: 5
            border.color: "red"  // Debug line
*/

            /*
            Rectangle {
                id: mainCmdtoolBarRect
                property int toolbarHeight: 50
                x: 0
                y: 5
                width: parent.width
                height: toolbarHeight
                anchors.top: parent.top
                // maximumHeight: toolbarHeight
                // minimumWidth: toolbarHeight
                border.color: "yellow" // Debug line
                border.width: 5 //Debug Line
                */


                /*
                ToolBar {
                    id: mainCmdtoolBar
                    x: 0
                    y: 5
                    width: parent.width
                    height: 50
                    anchors.top: parent.top



                    ToolButton {
                        id: scantoolButton
                        x: 0
                        y: 2
                        width: 150
                        height: 50
                        text: qsTr("Scan Network")
                        font.family: fontFamily
                        font.pointSize: pointSize + 1
                        flat: true
                        // highlighted: false

                        onClicked: {
                            if (tp_view_object_helper_prop !== null){
                                if (tp_view_object_helper_prop.socketState() === true){
                                    startBroadcast()
                                    prepareTimer.start()
                                } else {
                                    openSetUpIf()
                                }
                            } // null
                        } // OnClicked
                    } // end scantoolButton

                    ToolButton {
                        id: selectLANButton
                        anchors.left: scantoolButton.right
                        width: 150
                        height: 50
                        text: qsTr("LAN SELECT")
                        font.family: fontFamily
                        font.pointSize: pointSize + 1
                        flat: true
                        // highlighted: false
                        visible: false

                        onClicked: {
                              openSetUpIf()
                        } // OnClicked
                     } // end selectLANButton

                    ToolButton {
                        id: settingBtn
                        anchors.top: parent.top
                        anchors.topMargin: 10
                        anchors.right: parent.right
                        width: 50
                        height: 40
                        // text: qsTr("SETTINGS")
                        Image {
                                source: "qrc:/qml/content/Risorse/setUpIcon.png"
                                fillMode: Image.PreserveAspectFit // For not stretching image (optional)
                                anchors.fill: parent
                                anchors.margins: 2 // Leaving space between image and borders (optional)
                                anchors.bottomMargin:10 // Leaving space for text in bottom
                                anchors.centerIn: parent
                                Component.onCompleted: {
                                    // console.log ("Image completed")
                                    // console.log  (source)
                                }
                        } // Image
                        onClicked: {
                            if (selectLANButton.visible === false){
                                selectLANButton.visible = true;
                            } else {
                                selectLANButton.visible = false;
                            }
                        } // end onClicked
                    } // end settingBtn

                }
                */

            /*}*/ // end Rectangle mainRect

            /*
            Rectangle {
                id: spacerRect
                anchors.top: mainCmdtoolBar.bottom
                width: parent.width // 700
                height: 0
            }

            Rectangle {
                id: listRect
                anchors.top: spacerRect.bottom
                anchors.topMargin: 0
                width:  parent.width // 700
                height: 35
                border.width: 0
                RowLayout { // The "Row" type lays out its child items in a horizontal line
                    id: dataTitleRow
                    anchors.fill: parent
                    Rectangle {
                        id: devNameTitleRect;
                        Layout.fillWidth: true
                        Layout.minimumWidth: 50
                        Layout.preferredWidth: 10 * pointSize
                        Layout.maximumWidth: 300
                        Layout.minimumHeight: 20
                        Text {
                            id: devNameTitleText;
                            anchors.centerIn: parent
                            color: "black";
                            text: "Device Name"
                            font.family: fontFamily
                            font.pointSize: pointSize
                        }
                    } // end devNameTitleRect
                    Rectangle {
                        id: ipAddrTitleRect;
                        Layout.fillWidth: true
                        Layout.minimumWidth: 50
                        Layout.preferredWidth: 10 * pointSize
                        Layout.maximumWidth: 300
                        Layout.minimumHeight: 20
                        Text {
                            id: ipAddrTitleText;
                            anchors.centerIn: parent
                            color: "black";
                            text: "IP Address";
                            font.family: fontFamily
                            font.pointSize: pointSize
                        }
                    } // end ipAddrTitleRect
                    Rectangle {
                        id: macAddrTitleRect;
                        Layout.fillWidth: true
                        Layout.minimumWidth: 50
                        Layout.preferredWidth: 10 * pointSize
                        Layout.maximumWidth: 300
                        Layout.minimumHeight: 20
                        Text {
                            id: macAddrTitleText;
                            anchors.centerIn: parent
                            color: "black";
                            text: "MAC Address";
                            font.family: fontFamily
                            font.pointSize: pointSize
                        }
                    } // end macAddrTitleRect
                    Rectangle {
                        id: dhcpTitleRect;
                        Layout.fillWidth: true
                        Layout.minimumWidth: 50
                        Layout.preferredWidth: 10 * pointSize
                        Layout.maximumWidth: 300
                        Layout.minimumHeight: 20
                        Text {
                            id: dhcpTitleText;
                            anchors.centerIn: parent
                            color: "black";
                            text: "DHCP Used";
                            font.family: fontFamily
                            font.pointSize: pointSize
                        }
                    } // end dhcpTitleRect
                } // end dataTitleRow

            }

            Rectangle {
                id: scrollAreaRect
                width: parent.width // 700
                anchors.top: listRect.bottom
                height: parent.height -  mainCmdtoolBar.height -spacerRect.height - dataTitleRow.height - footerRect.height -10// Constants.height
                color:   "white" // "#c2c2c2";
                border.width:  2
                // border.color: "red"


                ScrollView {
                    id: scroller
                    anchors.fill: parent
                    clip: true
                    height: parent.height
                    // height:  mainRect.height

                    onHeightChanged: {
                        console.log ("Height is now: " + height)
                    }



                    // ScrollBar.vertical.policy: ScrollBar.AlwaysOn
                    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

                    ListView {
                        id: scrollingListView
                        width: parent.width
                        height: parent.height
                        opacity: showList

                        property var currentSelectedItem
                        property string currentSelectedName: ""

                        onCurrentIndexChanged: {
                            console.log("Current index now is: " + currentIndex);
                            currentSelectedItem = myDelegateModel.items.get(currentIndex);

                            console.log("Item is: " + currentSelectedItem);
                            // currentSelectedItem = myDelegateModel.items.get(currentIndex).model;
                            // console.log("Item is: " + currentSelectedItem.devName);
                            // currentSelectedName = currentSelectedItem.devName
                        }



                        model: DelegateModel {
                            id: myDelegateModel
                            model: serieModel

                            delegate:
                                ItemDelegate {
                                width: scrollingListView.width
                                id: myDelegate
                                height: pointSize * 2
                                highlighted: ListView.isCurrentItem
                                property int  currentItem :index

                                // generate the background
                                Rectangle {
                                    id: background
                                    anchors.fill: parent
                                    anchors.topMargin:    2 // 3
                                    anchors.bottomMargin: 2 // 3
                                    anchors.leftMargin:   3
                                    anchors.rightMargin:  3
                                    x: dataRow.x + 2    // 2;
                                    y: dataRow.y + 2    // 2;
                                    width: parent.width - x*2 + 3;
                                    height: parent.height - y*2 + 5 // 3
                                    border.color: "orange"
                                    radius: 3
                                    color: "lightgrey"
                                }

                                RowLayout {
                                    id: dataRow // layout
                                    anchors.fill: parent
                                    spacing: pointSize
                                    Rectangle {
                                        id: devNameRect;
                                        color: "transparent";
                                        Layout.fillWidth: true
                                        Layout.minimumWidth: 50
                                        Layout.preferredWidth: 10 * pointSize
                                        Layout.maximumWidth: 300
                                        Layout.minimumHeight: 20
                                        Text {
                                            id: devNameText;
                                            property int itemDel : (myDelegate != undefined)? myDelegate.currentItem: -1
                                            anchors.centerIn: parent
//                                            text:  (tp_view_object_helper_prop !== null)? tp_view_object_helper_prop.getHostName(itemDel):
//                                                                                          "HOST unknown"
                                            text: model.nom

                                            font.family: fontFamily
                                            font.pointSize: pointSize

                                            onTextChanged: {
                                                console.log("HostName Changed: ", devNameText.text)
                                            }
                                        }
                                        MouseDC{
                                            anchors.fill: parent
                                            privRightClick: openInfo
                                            privLeftClick: openInfo
                                            privGenClick: genericClick
                                            privIndex: index
                                            onClicked: {
                                                scrollingListView.currentIndex = index
                                            }
                                        } // end MouseDC
                                    }

                                    Rectangle {
                                        id: ipAddrRect;
                                        color: "transparent";
                                        Layout.fillWidth: true
                                        Layout.minimumWidth: 50
                                        Layout.preferredWidth: 10 * pointSize
                                        Layout.maximumWidth: 300
                                        Layout.minimumHeight: 20
                                        Text {
                                            id: ipAddrText;
                                            anchors.centerIn: parent
//                                            text:  (tp_view_object_helper_prop !== null)? tp_view_object_helper_prop.getIpAddr(myDelegate.currentItem):
//                                                                                    "IP Adress unknown"// myDelegate.currentItem
                                            text: model.selection
                                            font.family: fontFamily
                                            font.pointSize: pointSize

                                            onTextChanged:  {
                                                console.log ("Data Changed to ", ipAddrText.text)
                                                // console.log ("Data Changed to ", ipAddrText.text, tp_view_object_helper.getIpAddr(myDelegate.currentItem))
                                            }
                                        }
                                        MouseDC{
                                            anchors.fill: parent
                                            privRightClick: openInfo
                                            privLeftClick: openInfo
                                            privGenClick: genericClick
                                            privIndex: index
                                            onClicked: {
                                                scrollingListView.currentIndex = index
                                            }
                                        } // end MouseDC

                                        Component.onCompleted: {
                                            console.log("Completed ")
                                        }
                                    }

                                    Rectangle {
                                        id: macAddrRect;
                                        Layout.fillWidth: true
                                        Layout.minimumWidth: 50
                                        Layout.preferredWidth: 10 * pointSize
                                        Layout.maximumWidth: 300
                                        Layout.minimumHeight: 20
                                        color: "transparent";
                                        Text {
                                            id: macAddrText;
                                            anchors.centerIn: parent
                                            color: "black";
//                                            text:  (tp_view_object_helper_prop !== null)? tp_view_object_helper_prop.getMacAddress(myDelegate.currentItem):
//                                                                                    "MAC unknown"
                                            text: model.serie
                                            font.family: fontFamily
                                            font.pointSize: pointSize

                                            onTextChanged: {
                                                console.log("macAddrText is ", model.serie)
                                            }
                                        }
                                        MouseDC{
                                            anchors.fill: parent
                                            privRightClick: openInfo
                                            privLeftClick: openInfo
                                            privGenClick: genericClick
                                            privIndex: index
                                            onClicked: {
                                                scrollingListView.currentIndex = index
                                            }
                                        } // end MouseDC

                                    }

                                    Rectangle {
                                        id: dhcpRect;
                                        Layout.fillWidth: true
                                        Layout.minimumWidth: 50
                                        Layout.preferredWidth: 10 * pointSize
                                        Layout.maximumWidth: 300
                                        Layout.minimumHeight: 20
//                                        border.width:  10
//                                        border.color: "red"

                                        color: "transparent";
                                        Text {
                                            id: dhcpText;
                                            anchors.centerIn: parent
                                            color: "black";
//                                            text:  (tp_view_object_helper_prop !== null)? tp_view_object_helper_prop.getDHCPUsed(myDelegate.currentItem):
//                                                                                    'DHCP unknown'
                                            text: model.id
                                            font.family: fontFamily
                                            font.pointSize: pointSize

                                            onTextChanged: {
                                                console.log("dhcpText is ", model.id)
                                            }
                                        }
                                        MouseDC{
                                            anchors.fill: parent
                                            privRightClick: openInfo
                                            privLeftClick: openInfo
                                            privGenClick: genericClick
                                            privIndex: index
                                            onClicked: {
                                                scrollingListView.currentIndex = index
                                            }
                                        } // end MouseDC

                                    }

                                } // end rowLayout

                            } // end itemDelegate

                         } // end of DelegateModel


                    } //end List View
                }
            }

            Rectangle {
                id: footerRect
                anchors.top: scrollAreaRect.bottom
                width: parent.width // 700
                height: 20
                color: "gray"
                border.width: 1

                Text {
                    id: footerText;
                    anchors.centerIn: parent
                    // anchors.fill: parent
                    color: "black";
                    text: "TECNA IP CONFIG Version: " + swVersion;
                    font.family: fontFamily
                    font.pointSize: pointSize + 1
                }
            }

            */
            PopUpSetIP {
                id: popUpSetIp
                opacitypopupitem: 0
                title: "IP Setting"
                description: "IP Configuration"
                onOpacitypopupitemChanged: {
                    console.log("popUpSetIp Child opacity is: " + opacitypopupitem)

                    if (opacitypopupitem === 0){
                        console.log("PopUpSetIP disappears")
                        if (popUpSetIp.apply === 0){
                            console.log ("Cancel button pressed")
                        } else {
                            console.log ("New data saved")
                            setDataDevice(scrollingListView.currentIndex,
                                          popUpSetIp.ipAddrSet,
                                          popUpSetIp.ipNetMaskSet,
                                          popUpSetIp.ipGatewaySet,
                                          popUpSetIp.priDnsSet,
                                          popUpSetIp.secDnsSet,
                                          popUpSetIp.hostNameSet,
                                          popUpSetIp.dhcpSet)
                        }
                    } else {
                        console.log("PopUpSetIP Appears")
                    }
                }
            }

            PopUpSetInterface {
                id: popUpSetIf
                opacitypopupitem: 0
                title: "IP Setting"
                // description: "IP Configuration"
                ifModel: tp_if_combo_model
                // ipModel: tp_ip_combo_model
                onOpacitypopupitemChanged: {
                    console.log("PopUpSetInterface Child opacity is: " + opacitypopupitem)

                    if (opacitypopupitem === 0){
                        console.log("PopUpSetInterface disappears")
                        startBroadcast()
                        prepareTimer.start()
                        if (selectLANButton.visible === true){
                            selectLANButton.visible = false;
                        }
                    } else {
                        console.log("PopUpSetInterface Appears")
                    }
                }
            } // end PopUpSetInterface

            Timer {
                id: viewTimer
                interval: 1; running: false; repeat: false
                onTriggered: showListFct()
            }

            Timer {
                id: prepareTimer
                interval: 2100; running: false; repeat: false
                onTriggered: prepareFct()
            }
       /* }*/

    /*}*/ // end mainLayout


    onNet_ifChanged: {
        console.log("Net if changed", net_if);
        /** Debug Section
        var ifCount = (tp_view_object_helper != null) ?
                    tp_view_object_helper.getNumberOfIf() : -1
        // console.log("Number of if", ifCount);
        for (var  i=0; i< ifCount; i++){
            var ifName = (tp_view_object_helper != null) ?
                        tp_view_object_helper.getIfName(i) : ""
            console.log ("If: ", ifName)
        }
        */
    }

    onIfIsSelectdChanged: {
        console.log ("onIfIsSelectdChanged: ", ifIsSelectd)
        if (tp_view_object_helper_prop !== null){
            if (ifIsSelectd !== -1){
                tp_view_object_helper_prop.selectInterface(ifIsSelectd);
            } else {
                tp_view_object_helper_prop.clearSrcMacAddr()
            }
        } else {
            console.log ("onIfIsSelectdChanged: NULL pointer")
        }
    } // end onIfIsSelectdChanged:

    /* Uncomment if you want ip combobox
    onIpIsSelectdChanged: {
        console.log ("onIpIsSelectdChanged: ", ipIsSelectd)
        if (tp_view_object_helper_prop !== null){
            if (ipIsSelectd !== -1){
                tp_view_object_helper_prop.selectIP(ipIsSelectd);
            } else {
                tp_view_object_helper_prop.clearSrcIPAddr()
            }
        } else {
            console.log ("onIpIsSelectdChanged: NULL pointer")
        }
    } // end onIpIs
    */

    onTp_proc_endChanged: {
        console.log("onTp_proc_endChanged")
        if (showList == 1)
            scantoolButton.clicked()
    }

    function openInfo(){
        console.log("Open Info")
        // mainRect.height = 800
        opacitypopup = 1
        popUpSetIp.opacitypopupitem = opacitypopup
        popUpSetIp.title = scrollingListView.currentSelectedName   //.currentIndex
        console.log("Current item is: " + scrollingListView.currentIndex)

        fillDataDevice(scrollingListView.currentIndex)
        popUpSetIp.open()

    } // end function

    function genericClick(_index){
        scrollingListView.currentIndex = _index
        console.log("List Index is: " + scrollingListView.currentIndex)

    }// end function

    function openSetUpIf(){
        if (tp_view_object_helper_prop !== null)
            tp_view_object_helper_prop.getIfNames();
        console.log("Open openSetUpIf")
        popUpSetIf.opacitypopupitem = 1
        popUpSetIf.title = "Set Interface"
        popUpSetIf.open()
    } // end function

    function showListFct(){
        serieModel.listChanged()   
        var tp_listSize = (tp_view_object_helper_prop !== null) ? tp_view_object_helper_prop.getListSize() : 0
        console.log("showListFct - Scan for items: ", tp_listSize)
        if (tp_listSize !== 0){
            showList = 1
        } else {
            showList = 0
        }
    } // end function


    function startBroadcast(){
        console.log ("startBroadcast function")
         if (tp_view_object_helper_prop !== null){
             tp_view_object_helper_prop.startBroadcasting()
         } else {
             console.log ("startBroadcast not started")
         }
    } // end function

    function prepareFct(){
        console.log("prepareFct Fct")
         if (tp_view_object_helper_prop !== null){
             tp_view_object_helper_prop.getListSize()
             viewTimer.start()
         } //
    }

    function setDataDevice(_index,_ipAddrSet,_ipNetMaskSet,
                           _ipGatewaySet, _priDnsSet,_secDnsSet,
                           _hostNameSet,dhcpSet) {
        /** DEBUG OK
        console.log ("index is ", _index)
        console.log ("_ipAddrSet is ", _ipAddrSet)
        console.log ("_ipNetMaskSet is ", _ipNetMaskSet)
        console.log ("_ipGatewaySet is ", _ipGatewaySet)
        console.log ("_priDnsSet is ", _priDnsSet)
        console.log ("_secDnsSet is ", _secDnsSet)
        console.log ("_hostNameSet is ", _hostNameSet)
        console.log ("dhcpSet is ", dhcpSet)
        */
        console.log ("setDataDevice")
        if (tp_view_object_helper_prop !== null) {
            tp_view_object_helper_prop.setDataDevice(_index,_ipAddrSet,_ipNetMaskSet,
                                                     _ipGatewaySet, _priDnsSet,_secDnsSet,
                                                     _hostNameSet,dhcpSet)
        } // else

    } // end function

    function fillDataDevice(_index) {

        popUpSetIp.ipAddrSet = (tp_view_object_helper_prop !== null) ?
                    tp_view_object_helper_prop.getIpAddr(_index): "0.0.0.0"

        popUpSetIp.ipNetMaskSet = (tp_view_object_helper_prop !== null) ?
                    tp_view_object_helper_prop.getNetMask(_index): "255.255.255.0"

        popUpSetIp.ipGatewaySet = (tp_view_object_helper_prop !== null) ?
                    tp_view_object_helper_prop.getDefGw(_index): "0.0.0.0"


        popUpSetIp.priDnsSet = (tp_view_object_helper_prop !== null) ?
                    tp_view_object_helper_prop.getPriDNS(_index): "0.0.0.0"


        popUpSetIp.secDnsSet = (tp_view_object_helper_prop !== null) ?
                    tp_view_object_helper_prop.getSecDNS(_index): "0.0.0.0"


        popUpSetIp.hostNameSet = (tp_view_object_helper_prop !== null) ?
                    tp_view_object_helper_prop.getHostName(_index): "Unknown"


        popUpSetIp.dhcpSet = (tp_view_object_helper_prop !== null) ?
                    ((tp_view_object_helper_prop.getDHCPUsed(_index) === "Yes") ? 1 :0): -1

    } // end function



}


