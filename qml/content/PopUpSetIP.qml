import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    property int opacitypopupitem: 0
    property string title : "Unknown title"
    property string description : "Unknown title"
    property string ipAddrSet: "000.000.000.000"
    property string ipNetMaskSet: "255.000.000.000"

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

                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            focus = true;
                            console.log("Mouse Are POP UP click")
                        }
                    } // end Mouse
                }

                //        Controls.HorizontalLine { }

                Text {
                    id: descText
                    text: qsTr(description)
                    //            font.family: fontSystem.getContentFont.name
                    //            font.pixelSize: appStyle.paragraph
                    font.weight: Font.Light

                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            focus = true;
                            console.log("Mouse Are POP UP click")
                        }
                    } // end Mouse
                }

                //        Controls.VerticalSpacer { }

                TextInput
                {
                    id: myIpEdit
                    // anchors.top: descText.bottom
                    cursorVisible: true
                    focus: true

                    // validator: RegExpValidator{regExp: /^(([01]?[0-9]?[0-9]|2([0-4][0-9]|5[0-5]))\.){3}([01]?[0-9]?[0-9]|2([0-4][0-9]|5[0-5]))$/}
                    inputMask: "000.000.000.000; "

                    selectByMouse: true
                }

                TextInput
                {
                    id: myNetMaskEdit

                    cursorVisible: true
                    focus: true

                    // validator: RegExpValidator{regExp: /^(([01]?[0-9]?[0-9]|2([0-4][0-9]|5[0-5]))\.){3}([01]?[0-9]?[0-9]|2([0-4][0-9]|5[0-5]))$/}
                    inputMask: "000.000.000.000;_"

                    selectByMouse: true
                }

                RowLayout{
                    // anchors.top: myIpEdit.bottom
                    Button {
                        id: enableDHCPBtn
                        text: qsTr("SET IP");
                        onClicked: {
                            ipAddrSet= myIpEdit.text
                            ipNetMaskSet: myNetMaskEdit
                            opacitypopupitem = 0;
                            popup.close();

                            console.log("IP Set: " + ipAddrSet)

                        }
                    }
                    Button {
                        id: disableDHCPBtn
                        text: qsTr("RESET IP");
                        onClicked: {
                            ipAddrSet = "000.000.000.000"
                            ipNetMaskSet = "255.000.000.000"
                            opacitypopupitem = 0;
                            popup.close();

                            console.log("IP Reset: " + ipAddrSet)

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
