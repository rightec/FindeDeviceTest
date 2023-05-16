import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    property int opacitypopupitem: 0
    property int applyDone: 0
    property string title : "Unknown title"
    property string description : "Unknown title"
    required property var ifModel
    // required property var ipModel
    property int isSelected: interfaceCombo.currentIndex
    // Uncomment if you want combobox property int isIPSelected: ipAddrCombo.currentIndex
    x: 100
    y: 50
    height: 200


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
                                            text: qsTr("Interface configuration")
                                            // font.family: fontSystem.getContentFont.name
                                            font.pixelSize: 14
                                            Layout.alignment: Qt.AlignLeft;
                                            Layout.fillWidth: true
                                            // Layout.fillHeight: true
                                            height: headerRow.implicitHeight
                                        }
                                    }
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

                            ComboBox {
                                id: interfaceCombo
                                model: ifModel
                                textRole: "display"


                                onModelChanged: {
                                    // console.log("interfaceCombo Model is", ifModel) // Tested OK
                                } // end onModelChanged:

                            } //end combo


/*  UNCOMMENT if you want combobox
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

                            ComboBox {
                                id: ipAddrCombo
                                model: ipModel
                                textRole: "display"

                                onModelChanged: {
                                    // console.log("interface ipModel Model is", ipModel) // Tested OK
                                } // end onModelChanged:

                            } //end combo

*/
                            RowLayout{
                                id: buttonLayout
                                // anchors.top: myIpEdit.bottom
                                Button {
                                    id: applyBtn
                                    text: qsTr("Apply");
                                    onClicked: {
                                        applyDone = 1;
                                        opacitypopupitem = 0;
                                        popup.close();

                                        console.log("applyBtn ")

                                    } // end onClicked:
                                } // end Button
                                Button {
                                    id: cancelBtn
                                    text: qsTr("Cancel");
                                    onClicked: {
                                        console.log("Cancel: ")
                                         applyDone = 0;
                                        interfaceCombo.currentIndex = -1
                                        // uncomment if you want ip combo ipAddrCombo.currentIndex = -1
                                        opacitypopupitem = 0;
                                        popup.close();
                                    } // end onClicked
                                } // end Button
                            } //end RowLayout
                        } // end ColumnLayout
                    } // end Pane
            } // end Popup




    function open()
    {
        popup.open()
    }

} // end item
