import QtQuick 2.0
import QtQuick.Controls

//Item {

//    property variant privRightClick: null
//    property variant privLeftClick: null

    MouseArea {
     property variant privRightClick: null
     property variant privLeftClick: null
     property variant privGenClick: null
        property int privIndex: 0
      anchors.fill: parent
      acceptedButtons: Qt.LeftButton | Qt.RightButton
      onClicked:  (mouse)=> {
                      if (privGenClick != null){
                          privGenClick(privIndex)
                      } //

                      if(mouse.button === Qt.LeftButton){
                          if (privLeftClick !== null){
                              privLeftClick(1) //enable buttons
                              console.log("privLeftClick")
                          } else {
                              console.log("privLeftClick not implmeneted")
                          }
                      }
                      else {
                          if (privRightClick !== null){
                              privRightClick()
                              console.log("privRightClick")
                              if (privLeftClick !== null){
                                  privLeftClick(0) //disable buttons
                              } //

                          } else {
                              console.log("privRightClick not implmeneted")
                          }
                      }
                  } // end onclick
   } //end MouseArea

// }
