import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    property int opacitypopupitem: 0
    property string title : "Unknown title"
    property string description : "Unknown title"

    Popup {
            id: popup
            x: 100
            y: 100
            width: 200
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

                        Button {
                            text: qsTr("Close");
                            onClicked: {
                                popup.close();
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
