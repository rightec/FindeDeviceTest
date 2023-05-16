/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>

#include "tecnaProtocol.h"
#include "tecna_master.h"


Tecna_Master::Tecna_Master(QWidget *parent) : QWidget(parent)
{
    m_comboNetwork = new QComboBox();
    m_SetBindAddress = new QComboBox();
    QList<QNetworkInterface> l_nic = QNetworkInterface::allInterfaces();

    if (l_nic.size() > 0) {
        int l_cnt = l_nic.size();
        for (int i=0; i< l_cnt; i++){
            QNetworkInterface l_network = l_nic.at(i);
            qDebug() << l_network.name();
            qDebug() << l_network.humanReadableName();
            qDebug() << l_network.hardwareAddress();
            m_comboNetwork->addItem(l_network.humanReadableName());
        }
    }

    receivedLabel = new QLabel(tr("Waiting for bradcast answer..."));
    statusLabel = new QLabel(tr("Ready to broadcast datagrams on port 3250"));
    statusLabel->setWordWrap(true);

    m_CurrentSetDevice = new QLabel();
    m_setIpLabel =  new QLabel(tr("Set the IP Address for the device"));
    m_ipAddress = new QTextEdit("192.12.12.12");
    m_ipAddress->setMaximumHeight(25);
    m_setNetMAskLabel = new QLabel(tr("Set the net msk for the device"));
    m_NetMask =  new QTextEdit("255.255.255.0");
    m_NetMask->setMaximumHeight(25);
    m_setGWLabel = new QLabel(tr("Set the Gateway IP for the device"));
    m_Gateway =  new QTextEdit("129.12.12.1");
    m_Gateway->setMaximumHeight(25);
    m_PriDNSLabel = new QLabel(tr("Set the Primary DNS IP for the device"));
    m_PriDNS =  new QTextEdit("3.33.3.3");
    m_PriDNS->setMaximumHeight(25);
    m_SecDNSLabel = new QLabel(tr("Set the Secondary DNS IP for the device"));
    m_SecDNS =  new QTextEdit("4.44.4.4");
    m_SecDNS->setMaximumHeight(25);
    m_DHCPLabel = new QLabel(tr("Enable DHCP for this device"));
    m_DHCP =  new QCheckBox();
    m_DHCP->setMaximumHeight(25);
    m_HostNameLabel = new QLabel(tr("Set Host Name for this device"));
    m_HostName =  new QTextEdit();

    startButton = new QPushButton(tr("&Scan"));
    m_setIPButton = new QPushButton(tr("&Set"));
    m_warnButton = new QPushButton(tr("&Warn"));
    auto quitButton = new QPushButton(tr("&Quit"));

    auto buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(m_setIPButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(m_warnButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    udpSocket = new QUdpSocket(this);

    connect(startButton, &QPushButton::clicked, this, &Tecna_Master::startBroadcasting);
    connect(quitButton, &QPushButton::clicked, this, &Tecna_Master::close);
    connect(m_setIPButton, &QPushButton::clicked, this, &Tecna_Master::setCmd);
    connect(m_warnButton, &QPushButton::clicked, this, &Tecna_Master::warnCmd);
    connect(&m_timer, &QTimer::timeout, this, &Tecna_Master::processPendingDatagrams);
    connect(&m_viewTimer, &QTimer::timeout, this, &Tecna_Master::showScanList);
    connect(&m_refreshTimer, &QTimer::timeout, this, &Tecna_Master::enableDeviceProp);
    connect(m_comboNetwork, SIGNAL(currentIndexChanged(int)),this, SLOT(onComboCurrentChanged(int)));
    connect(m_SetBindAddress, SIGNAL(currentIndexChanged(int)),this, SLOT(onSetBindComboCurrentChanged(int)));
    connect(&m_ModelView,SIGNAL(m_listCleared(int)),this,SLOT(onListCleared(int)));


    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_comboNetwork);
    mainLayout->addWidget(m_SetBindAddress);
    mainLayout->addWidget(statusLabel);

    mainLayout->addWidget(m_CurrentSetDevice);
    mainLayout->addWidget(m_setIpLabel);
    mainLayout->addWidget(m_ipAddress);
    mainLayout->addWidget(m_setNetMAskLabel);
    mainLayout->addWidget(m_NetMask);
    mainLayout->addWidget(m_setGWLabel);
    mainLayout->addWidget(m_Gateway);
    mainLayout->addWidget(m_PriDNSLabel);
    mainLayout->addWidget(m_PriDNS);
    mainLayout->addWidget(m_SecDNSLabel);
    mainLayout->addWidget(m_SecDNS);
    mainLayout->addWidget(m_DHCPLabel);
    mainLayout->addWidget(m_DHCP);
    mainLayout->addWidget(m_HostNameLabel);
    mainLayout->addWidget(m_HostName);

    mainLayout->addWidget(buttonBox);
    mainLayout->addWidget(receivedLabel);
    setLayout(mainLayout);

    setWindowTitle(tr("TECNA Device Finder Master"));

    m_stringAnswer[TEC_PROT_ANNOUNCE_MSG] = TEC_CMD_SCAN_MODULE_STN;
    m_stringAnswer[TEC_PROT_MASTER_RX_CODE] = TEC_CMD_WARN_CODE_RCVD_STN;
    m_stringAnswer[TEC_PROT_MASTER_RX_INFO] = TEC_CMD_DEVDISCOVERED_STN;
    m_stringAnswer[TEC_PROT_UNKWN_MSG_ERR] = TEC_CMD_PROT_ERR_STN;
    m_stringAnswer[TEC_PROT_MSG_NOT_PARSED] = TEC_CMD_PROT_MASTER_STN;

    m_refreshTimer.start(100);

}

Tecna_Master::~Tecna_Master()
{
    qDebug() << "Tecna_Master::~Tecna_Master() is destructure";
}

void Tecna_Master::warnCmd()
{
    uint8_t l_macAddr[6] = {0x0,0x0,0x0,0x0,0x0,0x0};
    int l_pos = m_ModelView.itemSelected();
    QByteArray datagram;

    if (TP_GetMacFromPosition(l_pos, &l_macAddr[0]) != -1){

        memcpy((void*)&m_protocol.m_DestMacAddress[0],(void*)&l_macAddr[0],6);
        datagram.clear();

        // Sending unicast is only TECNA protocol
        TP_SetType(TEC_PROT_CHAR_ID_LSB_TECNA);

        uint8_t l_cmdPayload[255];
        memset ((void*)&l_cmdPayload[0],0,255 );;
        uint8_t l_len = TP_prepareCmd(TEC_PROT_CMD_WARN_ID,
                                      (uint8_t* ) &m_protocol.m_DestMacAddress[0],
                NULL,
                &l_cmdPayload[0] );

        for (int l_i = 0; l_i < l_len; l_i++){
            datagram.append(l_cmdPayload[l_i]);
        }
        udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, 3250);
    } // end else
}

void Tecna_Master::setCmd()
{
    uint8_t l_macAddr[6] = {0x0,0x0,0x0,0x0,0x0,0x0};
    uint8_t l_deviceCode[4] = {0x0,0x0,0x0,0x0};
    int l_pos = m_ModelView.itemSelected();
    QByteArray l_datagram;
    QString l_address;
    QString l_hostNameSet;
    tecProtocolMSetDataFrame_t l_payload;
    QByteArray l_hostName;
    uint8_t l_hostNameLen;
    uint8_t l_hostLenPassed;

    l_datagram.clear();

    if (TP_GetMacFromPosition(l_pos, &l_macAddr[0]) != -1){

        memcpy((void*)&m_protocol.m_DestMacAddress[0],(void*)&l_macAddr[0],6);

        l_address = m_ipAddress->toPlainText();
        m_protocol.setIPFromString(l_address, TEC_PROT_DATA_IPADDR_ID);
        l_address = m_NetMask->toPlainText();
        m_protocol.setIPFromString(l_address, TEC_PROT_DATA_NETMASK_ID);
        l_address = m_Gateway->toPlainText();
        m_protocol.setIPFromString(l_address, TEC_PROT_DATA_DEFGWAY_ID);
        l_address = m_PriDNS->toPlainText();
        m_protocol.setIPFromString(l_address, TEC_PROT_DATA_PRIDNS_ID);
        l_address = m_SecDNS->toPlainText();
        m_protocol.setIPFromString(l_address, TEC_PROT_DATA_SECDNS_ID);
        l_hostNameSet = m_HostName->toPlainText();
        m_protocol.setHostName(l_hostNameSet);

        TP_GetDevCodeFromPosition(l_pos, &l_deviceCode[0]);



        for (int i = 0; i<TEC_PROT_DEVICE_CODE_LEN; i++){
            l_payload.s_DeviceCode[i] = l_deviceCode[i];
        }

        for (int i = 0; i<TEC_PROT_DATA_IPADDR_LEN; i++){
            l_payload.s_IpAddrData[i] = m_protocol.m_ipAddress[i];
        }

        for (int i = 0; i<TEC_PROT_DATA_NETMASK_LEN; i++){
            l_payload.s_IpNetMaskData[i] = m_protocol.m_netMask[i];
        }

        for (int i = 0; i<TEC_PROT_DATA_DEFGWAY_LEN; i++){
            l_payload.s_IpDefGWData[i] = m_protocol.m_gateway[i];
        }

        for (int i = 0; i<TEC_PROT_DATA_PRIDNS_LEN; i++){
            l_payload.s_IpPriDNSData[i] = m_protocol.m_priDns[i];
        }

        for (int i = 0; i<TEC_PROT_DATA_SECDNS_LEN; i++){
            l_payload.s_IpSecDNSData[i] = m_protocol.m_secDns[i];
        }


        l_hostName = m_protocol.m_HostName.toUtf8();
        l_hostNameLen = l_hostName.length();


        l_hostLenPassed = (l_hostNameLen < TEC_PROT_DATA_HOSTNAME_LEN) ? l_hostNameLen : TEC_PROT_DATA_HOSTNAME_LEN;

        if (l_hostLenPassed != 0){
            for (int i = 0; i<l_hostLenPassed; i++){
                l_payload.s_IpHostNameData[i] = l_hostName[i];
            }
        } /**/
        l_payload.s_IpHostNameData[l_hostLenPassed] = TEC_PROT_DATA_END_OF_STRING;

        l_payload.s_IpDHCPData = m_DHCP->checkState() > 0 ? 1: 0;


        // Sending unicast is only TECNA protocol
        TP_SetType(TEC_PROT_CHAR_ID_LSB_TECNA);


        uint8_t l_cmdPayload[255];
        memset ((void*)&l_cmdPayload[0],0,255 );
        uint8_t l_len = TP_prepareCmd(TEC_PROT_CMD_SET_ID,
                                      (uint8_t* ) &m_protocol.m_DestMacAddress[0],
                (uint8_t* )&l_payload,
                &l_cmdPayload[0] );

        for (int l_i = 0; l_i < l_len; l_i++){
            l_datagram.append(l_cmdPayload[l_i]);
        }
        udpSocket->writeDatagram(l_datagram, QHostAddress::Broadcast, 3250);
    } // end else
}

void Tecna_Master::startBroadcasting()
{
    qDebug() << "Tecna_Master::startBroadcasting()";
    // reset the device
    TP_ResetSlaveDevice();
    m_ModelView.ClearList();
    m_ModelView.hide();
}

void Tecna_Master::onListCleared(int _size)
{
    qDebug() << "Tecna_Master::onListCleared: " << _size;
    if (_size == 0){
         broadcastDatagram();
    }
}

void Tecna_Master::broadcastDatagram()
{
    statusLabel->setText(tr("Now Discovering"));

    QString l_macAddr = "0xff:0xff:0xff:0xff:0xff:0xff";
    m_protocol.setMacAddressFromString(l_macAddr,1);


    uint8_t l_cmdPayload[255];
    uint8_t l_len = 0;
    QByteArray datagram;
    QByteArray datagram_2;
    quint64 l_bytes;

    // Sending Multicast is for bot HMS and TECNA protocol
    TP_SetType(TEC_PROT_CHAR_ID_LSB_HMS);


    memset ((void*)&l_cmdPayload[0],0,255 );
    l_len = TP_prepareCmd(TEC_PROT_CMD_MAX_ID,
                       NULL,
                       NULL,
                       &l_cmdPayload[0] );

    datagram.clear();

    for (int l_i = 0; l_i < l_len; l_i++){
        datagram.append(l_cmdPayload[l_i]);
    }

    m_timer.start(100);

    l_bytes = udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, 3250);

    qDebug() << "Sent bytes: " <<l_bytes;

    memset ((void*)&l_cmdPayload[0],0,255 );
    l_len = TP_prepareCmd(TEC_PROT_CMD_SCAN_ID,
                       (uint8_t* ) &m_protocol.m_DestMacAddress[0],
                       NULL,
                       &l_cmdPayload[0] );


    datagram_2.clear();

    for (int l_i = 0; l_i < l_len; l_i++){
        datagram_2.append(l_cmdPayload[l_i]);
    }

    l_bytes = udpSocket->writeDatagram(datagram_2, QHostAddress::Broadcast, 3250);

     qDebug() << "Sent bytes: " <<l_bytes;

}

void Tecna_Master::onComboCurrentChanged(int _index)
{
    Q_UNUSED(_index);
    qDebug() << m_comboNetwork->currentText();
    QList<QNetworkInterface> l_nic = QNetworkInterface::allInterfaces();
    int l_cnt = 0;
    int i = 0;
    QNetworkInterface l_network;

    if (l_nic.size() > 0) {
        l_cnt = l_nic.size();
        for (i=0; i< l_cnt; i++){
            l_network = l_nic.at(i);
            if( l_network.humanReadableName() == m_comboNetwork->currentText()){
                qDebug() << "Tecna_Master::onComboCurrentChanged: MAC address" << l_network.hardwareAddress();
                m_SetBindAddress->clear();
                QList<QHostAddress> l_listHost = l_network.allAddresses();
                bool l_isIPV4 = false;
                for (int l_iList = 0; l_iList < l_listHost.size(); l_iList++){
                    l_listHost.at(l_iList).toIPv4Address(&l_isIPV4);
                    if (l_isIPV4 == true){
                        if (l_listHost.at(l_iList).isLoopback() == false){
                            QString l_HostAdd = l_listHost.at(l_iList).toString();
                            qDebug() << "Tecna_Master::onComboCurrentChanged. IPV4 is: " << l_HostAdd;
                            m_SetBindAddress->addItem(l_HostAdd);
                        }
                    }
                }
                m_protocol.setMacAddressFromString(l_network.hardwareAddress(),0);
                i = l_cnt;
            } // end if
        } // end for
    } // end if
}

void Tecna_Master::onSetBindComboCurrentChanged(int _index)
{
    Q_UNUSED(_index);

    QString l_HostAdd = m_SetBindAddress->currentText();
    udpSocket->disconnectFromHost();
    udpSocket->bind(QHostAddress(l_HostAdd),3250);
    qDebug() << "Tecna_Master::onSetBindComboCurrentChanged. IPV4 selected is: " << l_HostAdd;
}


void Tecna_Master::enableDeviceProp()
{
    QString l_sItemSel = QString::number(m_ModelView.itemSelected(),10);

    if ((m_ModelView.itemSelected() == -1) || m_ModelView.isHidden() == true){
        m_ipAddress->setEnabled(false);
        m_NetMask->setEnabled(false);
        m_Gateway->setEnabled(false);
        m_PriDNS->setEnabled(false);
        m_SecDNS->setEnabled(false);
        m_DHCP->setEnabled(false);
        m_HostName->setEnabled(false);
    } else {
        m_ipAddress->setEnabled(true);
        m_NetMask->setEnabled(true);
        m_Gateway->setEnabled(true);
        m_PriDNS->setEnabled(true);
        m_SecDNS->setEnabled(true);
        m_DHCP->setEnabled(true);
        m_HostName->setEnabled(true);
    }

    m_CurrentSetDevice->setText(tr("Current device is: ") + l_sItemSel);


}

void Tecna_Master::showScanList()
{
    int iCount = -1;
    int hostNameSize = -1;
    uint8_t hostName[256];

    iCount = TP_GetNumberOfItemInList();
    for (int i = 0; i< iCount; i++){
        hostNameSize = TP_GetHostName(&hostName[0], i);
        QString l_stringToAdd;
        QByteArray l_ba;
        for (int i=0; i< hostNameSize; i++){
            l_ba.append(hostName[i]);
        }
        l_stringToAdd = QString::fromUtf8(l_ba);
        m_ModelView.AddItemToList(l_stringToAdd);
    }
    m_ModelView.show();
    m_viewTimer.stop();
}

void Tecna_Master::processPendingDatagrams()
{
    QByteArray l_datagram;
    int l_size=0;

    uint8_t l_ipAddress[4];
    QString l_ipString;
    QStringList l_ipAddrList;
    int iCount = -1;
    quint32  l_SenderIp4 = 0;


    while (udpSocket->hasPendingDatagrams()) {
        l_datagram.resize(int(udpSocket->pendingDatagramSize()));
        l_size = l_datagram.size();
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(l_datagram.data(), l_size,&sender, &senderPort);
        l_SenderIp4 = sender.toIPv4Address(nullptr);
        sender.setAddress(l_SenderIp4);
        l_ipString = sender.toString();
        l_ipAddrList = l_ipString.split(".");
        iCount =  l_ipAddrList.count();
        if (iCount == 4){
            for (int i=0; i< iCount; i++){
                l_ipAddress[i] = l_ipAddrList.at(i).toUInt(nullptr,10);
            }
        } //

        m_protocol.m_udprxMesgSize =l_size;
        for (int i=0; i< l_size; i++){
            m_protocol.m_ReceivedUdp[i] = l_datagram.at(i);
        }


        qDebug() << "Tecna_Master::processPendingDatagrams()";

        int l_ParsedRes = TP_ParseMessage(&m_protocol.m_ReceivedUdp[0], l_size, &l_ipAddress[0]);

        switch(l_ParsedRes){
        case TEC_PROT_MASTER_RX_INFO:
            m_viewTimer.start(3000);
            break;
        case TEC_PROT_ANNOUNCE_MSG:
             qDebug() << "Tecna_Master::processPendingDatagrams() _ Message not managed: " << l_ParsedRes;
            break;
        default:
            qDebug() << "Tecna_Master::processPendingDatagrams() _ Parse message fails";
            break;
        }

        qDebug() << "processPendingDatagrams(): Received Bytes: " <<m_protocol.m_udprxMesgSize;

        if (l_ParsedRes != -1){
            receivedLabel->setText(m_stringAnswer[l_ParsedRes]);
            qDebug() << m_stringAnswer[l_ParsedRes];
        }
    }

    // qDebug() << "processPendingDatagrams(): Exit ";

}

void Tecna_Master::closeEvent(QCloseEvent *event)
{
    qDebug() << "Event is: " << event->type();
    if (event->type() == QEvent::Close){
        m_ModelView.close();
    }
}
