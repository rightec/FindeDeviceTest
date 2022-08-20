import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    property int opacitypopupitem: 0
    property string title : "Unknown title"
    property string description : "Unknown title"
    property string dhcpUsed: "Unset"

    Popup {
            id: popup
            x: 100
            y: 100
            // width: 200
            height: 300
            modal: true
            focus: true
            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

            contentItem : Pane {
                    id: pane
                    // Material.elevation: 16


                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        layoutDirection: Qt.RightToLeft

                        Text {
                            text: qsTr(title)
                            //            font.family: fontSystem.getTitleFont.name
                            //            font.pixelSize: appStyle.h6
                            font.bold: true
                            //            color: {
                            //                if(type=="info") {
                            //                    appStyle.primary
                            //                } else if (type=="warning") {
                            //                    appStyle.warning
                            //                } else if (type=="error") {
                            //                    appStyle.error
                            //                } else if (type=="success") {
                            //                    appStyle.success
                            //                } else {
                            //                    appStyle.primary
                            //                }
                            //            }
                        }

                        //        Controls.HorizontalLine { }

                        Text {
                            text: qsTr(description)
                            //            font.family: fontSystem.getContentFont.name
                            //            font.pixelSize: appStyle.paragraph
                            font.weight: Font.Light
                        }

                        //        Controls.VerticalSpacer { }

                        RowLayout{
                                Button {
                                    id: enableDHCPBtn
                                    text: qsTr("Enable DHCP");
                                    onClicked: {
                                        dhcpUsed = "Yes"
                                        opacitypopupitem = 0;
                                        popup.close();

                                        console.log("Enable DHCP")

                                    }
                                }
                                Button {
                                    id: disableDHCPBtn
                                    text: qsTr("Disable DHCP");
                                    onClicked: {
                                        dhcpUsed = "No"
                                        opacitypopupitem = 0;
                                        popup.close();

                                        console.log("Disable DHCP")

                                    }
                                }
                        }
                    }
        } // end Pane
    } // end Popup

    function open()
    {
        popup.open()
    }

} // end item
