import QtQuick 2.0

ListModel {
    ListElement {
        devName: "TECNA Device xyz"
        ipAddress: "192.23.4.5"
        macAddress: "00:22:33:44:55:66"
        netMask: "255.000.000.000"
        dhcpUsed: "yes"
    }
    ListElement {
        devName: "TECNA Device abc"
        ipAddress: "192.23.4.6"
        macAddress: "00:22:33:44:55:67"
        netMask: "255.000.000.000"
        dhcpUsed: "yes"
    }
    ListElement {
        devName: "TECNA Device def"
        ipAddress: "192.23.4.7"
        macAddress: "00:22:33:44:55:68"
        netMask: "255.000.000.000"
        dhcpUsed: "yes"
    }
}
