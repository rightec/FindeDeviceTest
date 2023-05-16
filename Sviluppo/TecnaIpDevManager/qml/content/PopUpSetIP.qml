import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    property int opacitypopupitem: 0
    property string title : "Unknown title"
    property string description : "Unknown title"
    property string ipAddrSet: ipAddrEdit.text // "000.000.000.000"
    property string ipNetMaskSet: netMaskEdit.text // "255.000.000.000"
    property string ipGatewaySet: gatewayEdit.text //"000.000.000.000"
    property string priDnsSet: priDNSEdit.text // "000.000.000.000"
    property string secDnsSet: secDNSEdit.text // "000.000.000.000"
    property string hostNameSet: hostNameEdit.text // "Unassigned"
    property int dhcpSet: dhcpEdit.checkState // 0
    property int apply: 0
    x: 100
    y: 50
    height: 500


            Popup {
                id: popup
                // width: 200
                height: parent.height
                modal: true
                focus: true
                closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
                Overlay.modal: Rectangle {
                            color: "#c8000000"
                            /* here we use 8 bit color coding
                                       first 2 bits define opacity.
                                       the remaining six define standard color code.
                                       */
                        }
                contentItem :

                    Pane {
                        id: pane
                        ColumnLayout {
                            id: mainPopupLayout
                            anchors.fill: parent
                            anchors.margins: 0
                            layoutDirection: Qt.LeftToRight // Qt.RightToLeft
                            spacing: 0



                            ColumnLayout {
                                id: headerPopupLayout
                                // layoutDirection: Qt.RightToLeft
                                Rectangle {
                                    id: mainPopupRect
                                    // anchors.margins: 5
                                    width: headerPopupLayout.implicitWidth
                                    height: headerPopupLayout.implicitHeight
                                    Layout.fillWidth: true
                                    // Layout.fillHeight: true
                                    Layout.minimumWidth: 200
                                    // Layout.preferredWidth: dataTitleRow.width // mainLayout.implicitWidth // 700
                                    Layout.maximumWidth: 400
                                    Layout.minimumHeight: 30
                                    Layout.maximumHeight: 200
                                    // Layout.alignment: Qt.AlignTop;
                                    border.width: 0
                                    color: "gray"

                                    RowLayout {
                                        id: headerRow
                                        spacing: 1
                                        Text {
                                            id: devConfigText
                                            text: qsTr("Device configuration")
                                            // font.family: fontSystem.getContentFont.name
                                            font.pixelSize: 14
                                            Layout.alignment: Qt.AlignLeft;
                                            Layout.fillWidth: true
                                            // Layout.fillHeight: true
                                            height: headerRow.implicitHeight
                                        } //end Text
                                    } // end RowLayout

                                } // end rect
                            } // end ColumnLayout

                            Text {
                                text: qsTr(title)
                                font.bold: true
                                anchors.margins: 0
                                width: mainPopupLayout.implicitWidth
                                height: mainPopupLayout.implicitHeight

                                MouseArea{
                                    anchors.fill: parent
                                    onClicked: {
                                        focus = true;
                                        console.log("Mouse Are POP UP click")
                                    }
                                } // end Mouse
                            } // end text


                            /******************* DESCRIPTION ***************************/
                            Text {
                                id: descText
                                text: qsTr(description)
                                font.bold: true
                                width: mainPopupLayout.implicitWidth
                                height: mainPopupLayout.implicitHeight

                                MouseArea{
                                    anchors.fill: parent
                                    onClicked: {
                                        focus = true;
                                        console.log("Mouse Are POP UP click")
                                    }
                                } // end Mouse
                            } // end Text


                            /******************* DHCP SETTING ***************************/

                            CheckBox {
                                id: dhcpEdit
                                checked: (dhcpSet !== 0)
                                text: qsTr("Enable DHCP ")
                                font.italic: true
                                width: mainPopupLayout.implicitWidth
                                height: mainPopupLayout.implicitHeight

                                onCheckStateChanged: {
                                    console.log("dhcpEdit text changed", checked)
                                    dhcpSet = (checked === true) ? 1: 0
                                }
                            }

                            /******************* IP ADDRESS ***************************/

                            Text {
                                id: ipAddrText
                                text: qsTr("IP address")
                                font.italic: true
                                width: mainPopupLayout.implicitWidth
                                height: mainPopupLayout.implicitHeight

                            }

                            Rectangle {
                                id: ipAddrEditRect
                                border.width: 1
                                border.color: "black"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.maximumWidth: 300
                                Layout.minimumHeight: 15
                                Layout.maximumHeight: 20

                                TextInput
                                {
                                    id: ipAddrEdit
                                    anchors.fill: parent
                                    anchors.margins: 1
                                    focus: true
                                    selectByMouse: true

                                    validator: RegularExpressionValidator {
                                        regularExpression:  /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
                                    }

                                    onTextChanged: {
                                        console.log("ipAddrEdit text changed", text)
                                        ipAddrSet = text
                                    }
                                } // end ipAddrEdit
                            } // end ipAddrEditRect

                            /******************* NET MASK ***************************/

                            Text {
                                id: netMaskText
                                text: qsTr("Subnet Mask")
                                font.italic: true
                                width: mainPopupLayout.implicitWidth
                                height: mainPopupLayout.implicitHeight

                            }
                            Rectangle {
                                id: netMaskEditRect
                                border.width: 1
                                border.color: "black"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.maximumWidth: 300
                                Layout.minimumHeight: 15
                                Layout.maximumHeight: 20

                                TextInput
                                {
                                    id: netMaskEdit
                                    text: ipNetMaskSet
                                    anchors.fill: parent
                                    anchors.margins: 1
                                    focus: true
                                    validator: RegularExpressionValidator {
                                        regularExpression:  /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
                                    }
                                    selectByMouse: true

                                    onTextChanged: {
                                        console.log("netMaskEdit text changed", text)
                                        ipNetMaskSet = text
                                    }

                                }
                            }
                            /******************* IP GATEWAY ***************************/

                            Text {
                                id: gatewayText
                                text: qsTr("Gateway")
                                font.italic: true
                                width: mainPopupLayout.implicitWidth
                                height: mainPopupLayout.implicitHeight
                            }

                            Rectangle {
                                id: gatewayEditRect
                                border.width: 1
                                border.color: "black"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.maximumWidth: 300
                                Layout.minimumHeight: 15
                                Layout.maximumHeight: 20

                                TextInput
                                {
                                    id: gatewayEdit
                                    text: ipGatewaySet
                                    anchors.fill: parent
                                    anchors.margins: 1
                                    focus: true
                                    selectByMouse: true
                                    validator: RegularExpressionValidator {
                                        regularExpression:  /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
                                    }

                                    onTextChanged: {
                                        console.log("gatewayEdit text changed", text)
                                        ipGatewaySet = text
                                    }

                                }

                            }
                            /******************* PRIMARY DNS ***************************/

                            Text {
                                id: priDNSText
                                text: qsTr("Primary DNS")
                                font.italic: true
                                width: mainPopupLayout.implicitWidth
                                height: mainPopupLayout.implicitHeight
                            }

                            Rectangle {
                                id: priDNSEditRect
                                border.width: 1
                                border.color: "black"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.maximumWidth: 300
                                Layout.minimumHeight: 15
                                Layout.maximumHeight: 20

                                TextInput
                                {
                                    id: priDNSEdit
                                    text: priDnsSet
                                    anchors.fill: parent
                                    anchors.margins: 1
                                    focus: true
                                    selectByMouse: true
                                    validator: RegularExpressionValidator {
                                        regularExpression:  /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
                                    }

                                    onTextChanged: {
                                        console.log("priDNSEdit text changed", text)
                                        priDnsSet = text
                                    }

                                }

                            }
                            /******************* SECONDARY DNS ***************************/

                            Text {
                                id: secDNSText
                                text: qsTr("Secondary DNS")
                                font.italic: true
                                width: mainPopupLayout.implicitWidth
                                height: mainPopupLayout.implicitHeight
                            }

                            Rectangle {
                                id: secDNSEditRect
                                border.width: 1
                                border.color: "black"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.maximumWidth: 300
                                Layout.minimumHeight: 15
                                Layout.maximumHeight: 20

                                TextInput
                                {
                                    id: secDNSEdit
                                    text: secDnsSet
                                    anchors.fill: parent
                                    anchors.margins: 1
                                    focus: true
                                    selectByMouse: true
                                    validator: RegularExpressionValidator {
                                        regularExpression:  /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
                                    }

                                    onTextChanged: {
                                        console.log("secDNSEdit text changed", text)
                                        secDnsSet = text
                                    }

                                }

                            }


                            /******************* HOST NAME ***************************/

                            Text {
                                id: hostNameText
                                text: qsTr("Host Name")
                                font.italic: true
                                width: mainPopupLayout.implicitWidth
                                height: mainPopupLayout.implicitHeight
                            }

                            Rectangle {
                                id: hostNameEditRect
                                border.width: 1
                                border.color: "black"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.maximumWidth: 500
                                Layout.minimumHeight: 20
                                Layout.maximumHeight: 30

                                TextInput
                                {
                                    id: hostNameEdit
                                    text: hostNameSet
                                    focus: true
                                    selectByMouse: true
                                    inputMethodHints: Qt.ImhNone

                                    onTextChanged: {
                                        console.log("hostNameEdit text changed", text)
                                        hostNameSet = text
                                    }

                                }

                            }
                            /******************* BUTTON LAYER ***************************/


                            RowLayout{
                                id: buttonLayout
                                // anchors.top: ipAddrEdit.bottom
                                Button {
                                    id: applyBtn
                                    text: qsTr("Apply");
                                    onClicked: {
                                        console.log("Apply Button pressed")
                                        // console.log("ipAddrSet", ipAddrSet)
                                        // console.log("ipAddrEdit.text", ipAddrEdit.text)
                                        ipAddrSet= ipAddrEdit.text
                                        ipNetMaskSet = netMaskEdit.text
                                        ipGatewaySet = gatewayEdit.text
                                        priDnsSet = priDNSEdit.text
                                        secDnsSet = secDNSEdit.text
                                        hostNameSet = hostNameEdit.text
                                        dhcpSet = (dhcpEdit.checkState === Qt.Unchecked) ? 0: 1
                                        apply = 1



                                        opacitypopupitem = 0;
                                        popup.close();
                                    }
                                }
                                Button {
                                    id: cancelBtn
                                    text: qsTr("Cancel");
                                    onClicked: {
                                        console.log("Cancel Button pressed")
                                        opacitypopupitem = 0;
                                        popup.close();
                                        // ipAddrEdit.focus = true
                                        apply = 0

                                    }
                                }
                            }


                        } // end ColumnLayout

                    } // end Pane



            } // end Popup



    onIpAddrSetChanged: {
        ipAddrEdit.text = ipAddrSet;
        console.log("onIpAddrSetChanged to: ",ipAddrSet)
    }

    function open()
    {
        popup.open()
    }

} // end item
