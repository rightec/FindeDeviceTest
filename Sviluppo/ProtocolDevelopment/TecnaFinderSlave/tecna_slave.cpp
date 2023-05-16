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

#include "TP_Protocol/tecnaProtocol.h"
#include "tecna_slave.h"


Tecna_Slave::Tecna_Slave(QWidget *parent) : QWidget(parent)
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

    m_setIpLabel =  new QLabel(tr("Set the IP Address for the device"));
    m_ipAddress = new QTextEdit("192.168.10.5");
    m_ipAddress->setMaximumHeight(25);
    m_setNetMAskLabel = new QLabel(tr("Set the net msk for the device"));
    m_NetMask =  new QTextEdit("255.255.255.0");
    m_NetMask->setMaximumHeight(25);
    m_setGWLabel = new QLabel(tr("Set the Gateway IP for the device"));
    m_Gateway =  new QTextEdit("192.168.10.1");
    m_Gateway->setMaximumHeight(25);
    m_PriDNSLabel = new QLabel(tr("Set the Primary DNS IP for the device"));
    m_PriDNS =  new QTextEdit("1.1.1.1");
    m_PriDNS->setMaximumHeight(25);
    m_SecDNSLabel = new QLabel(tr("Set the Secondary DNS IP for the device"));
    m_SecDNS =  new QTextEdit("2.2.2.2");
    m_SecDNS->setMaximumHeight(25);
    m_DHCPLabel = new QLabel(tr("Enable DHCP for this device"));
    m_DHCP =  new QCheckBox();
    m_DHCP->setMaximumHeight(25);
    m_HostNameLabel = new QLabel(tr("Set Host Name for this device"));
    m_HostName =  new QTextEdit(tr("Simulated TECNA device on Linux"));

    m_applyConfig = new QPushButton(tr("&Apply Setting"));
    auto quitButton = new QPushButton(tr("&Quit"));

    auto buttonBox = new QDialogButtonBox;
    buttonBox->addButton(m_applyConfig, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    udpSocket = new QUdpSocket(this);

    connect(m_applyConfig, &QPushButton::clicked, this, &Tecna_Slave::applyConfig);
    connect(quitButton, &QPushButton::clicked, this, &Tecna_Slave::close);
    connect(&m_timer, &QTimer::timeout, this, &Tecna_Slave::processPendingDatagrams);
    connect(m_comboNetwork, SIGNAL(currentIndexChanged(int)),this, SLOT(onComboCurrentChanged(int)));
    connect(m_SetBindAddress, SIGNAL(currentIndexChanged(int)),this, SLOT(onSetBindComboCurrentChanged(int)));

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_comboNetwork);
    mainLayout->addWidget(m_SetBindAddress);
    mainLayout->addWidget(statusLabel);

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

    setWindowTitle(tr("TECNA Device Finder Slave"));

    m_stringAnswer[TEC_PROT_ANNOUNCE_MSG] = TEC_CMD_SCAN_MODULE_STN;
    m_stringAnswer[TEC_PROT_MASTER_RX_CODE] = TEC_CMD_SCAN_MODULE_MAC_STN;
    m_stringAnswer[TEC_PROT_MASTER_RX_INFO] = TEC_CMD_DEVDISCOVERED_STN;
    m_stringAnswer[TEC_PROT_SLAVE_RX_INFO] = TEC_PROT_SLAVE_RX_INFO_STN;
    m_stringAnswer[TEC_PROT_UNKWN_MSG_ERR] = TEC_CMD_PROT_ERR_STN;
    m_stringAnswer[TEC_PROT_MSG_NOT_PARSED] = TEC_CMD_PROT_SLAVE_STN;
    m_stringAnswer[TEC_PROT_SLAVE_RX_INFO_NACK] = TEC_PROT_SLAVE_RX_INFO_NACK_STN;
    m_stringAnswer[TEC_PROT_SLAVE_RX_WARN] = TEC_PROT_SLAVE_RX_WARN_STN;
    m_stringAnswer[TEC_PROT_SLAVE_RX_SET] = TEC_PROT_SLAVE_RX_SET_STN;
    m_stringAnswer[TEC_PROT_SLAVE_HMS]= TEC_PROT_SLAVE_HMS_STN;
}

void Tecna_Slave::replyNack(uint8_t _cmdId)
{
    statusLabel->setText(tr("Reply NACK"));

    qDebug() << "Tecna_Slave::replyNack() to cmd: " << _cmdId;

    QByteArray l_datagram;
    quint64 l_bytes;
    uint8_t l_cmdPayload[255];
    uint8_t l_len = 0;


    memset ((void*)&l_cmdPayload[0],0,255 );

    l_len = TP_prepareNack(_cmdId,
                           NULL,
                       &l_cmdPayload[0] );

    l_datagram.clear();


    for (int l_i = 0; l_i < l_len; l_i++){
        l_datagram.append(l_cmdPayload[l_i]);
    }
    // l_bytes = udpSocket->writeDatagram(l_datagram, QHostAddress::Broadcast, TEC_PROT_UDP_PROT_PORT);

    l_bytes = writeMyData(&l_datagram);

    qDebug() << "Sent bytes: " <<l_bytes;

}

void Tecna_Slave::replyToSet()
{
    statusLabel->setText(tr("Reply To SET"));

    qDebug() << "Tecna_Slave::replyToSet()";

    QByteArray l_datagram;
    quint64 l_bytes;
    uint8_t l_cmdPayload[255];
    uint8_t l_len = 0;


    // First get the va

    memset ((void*)&l_cmdPayload[0],0,255 );
    l_len = TP_prepareCmd(TEC_PROT_CMD_SET_ID,
                       NULL,
                       NULL,
                       &l_cmdPayload[0] );

    l_datagram.clear();


    for (int l_i = 0; l_i < l_len; l_i++){
        l_datagram.append(l_cmdPayload[l_i]);
    }

    // l_bytes = udpSocket->writeDatagram(l_datagram, QHostAddress::Broadcast, TEC_PROT_UDP_PROT_PORT);

    l_bytes = writeMyData(&l_datagram);

    qDebug() << "Sent bytes: " <<l_bytes;
}

void Tecna_Slave::replyToWarn()
{
    statusLabel->setText(tr("Reply To WARN"));

    qDebug() << "Tecna_Slave::replyToWarn()";

    QByteArray l_datagram;
    quint64 l_bytes;
    uint8_t l_cmdPayload[255];
    uint8_t l_len = 0;

    memset ((void*)&l_cmdPayload[0],0,255 );
    l_len = TP_prepareCmd(TEC_PROT_CMD_WARN_ID,
                       NULL,
                       NULL,
                       &l_cmdPayload[0] );

    l_datagram.clear();


    for (int l_i = 0; l_i < l_len; l_i++){
        l_datagram.append(l_cmdPayload[l_i]);
    }
    // l_bytes = udpSocket->writeDatagram(l_datagram, QHostAddress::Broadcast, TEC_PROT_UDP_PROT_PORT);

    l_bytes = writeMyData(&l_datagram);

    qDebug() << "Sent bytes: " <<l_bytes;
}

void Tecna_Slave::applyConfig()
{
    qDebug() << "Tecna_Slave::applyConfig()";

    QString l_address = m_ipAddress->toPlainText();
    m_protocol.setIPFromString(l_address, TEC_PROT_DATA_IPADDR_ID);
    l_address = m_NetMask->toPlainText();
    m_protocol.setIPFromString(l_address, TEC_PROT_DATA_NETMASK_ID);
    l_address = m_Gateway->toPlainText();
    m_protocol.setIPFromString(l_address, TEC_PROT_DATA_DEFGWAY_ID);
    l_address = m_PriDNS->toPlainText();
    m_protocol.setIPFromString(l_address, TEC_PROT_DATA_PRIDNS_ID);
    l_address = m_SecDNS->toPlainText();
    m_protocol.setIPFromString(l_address, TEC_PROT_DATA_SECDNS_ID);
    QString l_hostNameSet = m_HostName->toPlainText();
    m_protocol.setHostName(l_hostNameSet);

    tecProtocolSDiscoveryFrame_t l_config;

    for (int i = 0; i<TEC_PROT_DATA_IPADDR_LEN; i++){
        l_config.s_IpAddrData[i] = m_protocol.m_ipAddress[i];
    }

    for (int i = 0; i<TEC_PROT_DATA_NETMASK_LEN; i++){
        l_config.s_IpNetMaskData[i] = m_protocol.m_netMask[i];
    }

    for (int i = 0; i<TEC_PROT_DATA_DEFGWAY_LEN; i++){
        l_config.s_IpDefGWData[i] = m_protocol.m_gateway[i];
    }

    for (int i = 0; i<TEC_PROT_DATA_PRIDNS_LEN; i++){
        l_config.s_IpPriDNSData[i] = m_protocol.m_priDns[i];
    }

    for (int i = 0; i<TEC_PROT_DATA_SECDNS_LEN; i++){
        l_config.s_IpSecDNSData[i] = m_protocol.m_secDns[i];
    }


    QByteArray l_hostName = m_protocol.m_HostName.toUtf8();
    uint8_t l_hostNameLen = l_hostName.length();


   uint8_t l_hostLenPassed = (l_hostNameLen < TEC_PROT_DATA_HOSTNAME_LEN) ? l_hostNameLen : TEC_PROT_DATA_HOSTNAME_LEN;

   if (l_hostLenPassed != 0){
       for (int i = 0; i<l_hostLenPassed; i++){
           l_config.s_IpHostNameData[i] = l_hostName[i];
       }
   } /**/
   l_config.s_IpHostNameData[l_hostLenPassed] = TEC_PROT_DATA_END_OF_STRING;
   l_config.s_IpHostNameLEN = l_hostLenPassed;

    l_config.s_IpDHCPData = m_DHCP->checkState() == 0 ? m_DHCP->checkState() : 1;

    TP_SlaveConfig(&l_config);
}

void Tecna_Slave::replyToScan()
{
    statusLabel->setText(tr("Reply To SCAN"));

    qDebug() << "Tecna_Slave::replyToScan()";

    uint8_t l_cmdPayload[255];
    uint8_t l_len = 0;
    QByteArray l_datagram;
    quint64 l_bytes;


    memset ((void*)&l_cmdPayload[0],0,255 );
    l_len = TP_prepareCmd(TEC_PROT_CMD_SCAN_ID,
                       NULL,
                       NULL,
                       &l_cmdPayload[0] );


    l_datagram.clear();

    for (int l_i = 0; l_i < l_len; l_i++){
        l_datagram.append(l_cmdPayload[l_i]);
    }

    // l_bytes = udpSocket->writeDatagram(l_datagram, QHostAddress::Broadcast, TEC_PROT_UDP_PROT_PORT);

    l_bytes = writeMyData(&l_datagram);

     qDebug() << "Sent bytes: " <<l_bytes;

}

void Tecna_Slave::onComboCurrentChanged(int _index)
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
                qDebug() << "Tecna_Slave::onComboCurrentChanged: MAC address" << l_network.hardwareAddress();
                m_SetBindAddress->clear();
                QList<QNetworkAddressEntry> l_listHost = l_network.addressEntries();
                for (int lentr = 0; lentr < l_listHost.size(); lentr++){
                    QHostAddress l_newHost = l_listHost.at(lentr).ip();
                    bool l_isIPV4 = false;
                    l_newHost.toIPv4Address(&l_isIPV4);
                     if (l_isIPV4 == true){
                         qDebug() << "Tecna_Slave::onComboCurrentChanged. IPV4 is: " << l_newHost.toString();
                          m_SetBindAddress->addItem(l_newHost.toString());
                          break;
                     } // end if
                } // end for


                /*
                m_SetBindAddress->clear();

                QList<QHostAddress> l_listHost = l_network.allAddresses();
                bool l_isIPV4 = false;
                for (int l_iList = 0; l_iList < l_listHost.size(); l_iList++){
                    l_listHost.at(l_iList).toIPv4Address(&l_isIPV4);
                    if (l_isIPV4 == true){
                        if (l_listHost.at(l_iList).isLoopback() == false){
                            QString l_HostAdd = l_listHost.at(l_iList).toString();
                            qDebug() << "Tecna_Slave::onComboCurrentChanged. IPV4 is: " << l_HostAdd;
                            m_SetBindAddress->addItem(l_HostAdd);
                        }
                    }
                }
                */
                m_protocol.setMacAddressFromString(l_network.hardwareAddress(),0);
                i = l_cnt;
            } // end if
        } // end for
        int l_temp = m_SetBindAddress->count();
        if (l_temp > 0)
            onSetBindComboCurrentChanged(0);
    } // end if
}

void Tecna_Slave::onSetBindComboCurrentChanged(int _index)
{
    Q_UNUSED(_index);

    m_timer.stop();
    QString l_HostAdd = m_SetBindAddress->currentText();
    udpSocket->disconnectFromHost();
    // udpSocket->connectToHost(QHostAddress(l_HostAdd),TEC_PROT_UDP_PROT_PORT);
// OK    udpSocket->bind(QHostAddress::AnyIPv4, TEC_PROT_UDP_PROT_PORT, QUdpSocket::ShareAddress); //bind(QHostAddress(l_HostAdd),TEC_PROT_UDP_PROT_PORT);
    udpSocket->bind(QHostAddress(l_HostAdd), TEC_PROT_UDP_PROT_PORT, QUdpSocket::ShareAddress); //bind(QHostAddress(l_HostAdd),TEC_PROT_UDP_PROT_PORT);
    // udpSocket->bind(QHostAddress::AnyIPv4, TEC_PROT_UDP_PROT_PORT;
    // udpSocket->bind(QHostAddress(l_HostAdd),TEC_PROT_UDP_PROT_PORT);
    m_timer.start(100);
    qDebug() << "Tecna_Slave::onSetBindComboCurrentChanged. IPV4 selected is: " << l_HostAdd;
}

void Tecna_Slave::processPendingDatagrams()
{
    QByteArray l_datagram;
    int l_size=0;

    uint8_t l_ipAddress[4];
    QString l_ipString = "";
    QStringList l_ipAddrList;
    int iCount = -1;
    quint32  l_SenderIp4 = 0;

    int hostNameSize = -1;
    uint8_t hostName[256];


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


        qDebug() << "Tecna_Slave::processPendingDatagrams()";

        int l_ParsedRes = TP_ParseMessage(&m_protocol.m_ReceivedUdp[0], l_size, &l_ipAddress[0]);

        switch(l_ParsedRes){
        case TEC_PROT_ANNOUNCE_MSG:
            TP_SetAnnouncedMaster(l_ipAddress);
            break;
        case TEC_PROT_MASTER_RX_INFO:
            iCount = TP_GetNumberOfItemInList();
            for (int i = 0; i< iCount; i++){
                hostNameSize = TP_GetHostName(&hostName[0], i);
                QString l_stringToAdd;
                QByteArray l_ba;
                for (int i=0; i< hostNameSize; i++){
                    l_ba.append(hostName[i]);
                }
                l_stringToAdd = QString::fromUtf8(l_ba);
            }
            break;
        case TEC_PROT_SLAVE_RX_INFO:
            /** Prepare answer to the master */
            replyToScan();
            break;
        case TEC_PROT_SLAVE_RX_WARN:
            /** Prepare answer to the master */
            replyToWarn();
            break;
        case TEC_PROT_SLAVE_RX_INFO_NACK:
            /** Prepare NACK answer to the master */
            replyNack(m_protocol.m_ReceivedUdp[TEC_PROT_CMD_ID_POS]);
            break;
        case TEC_PROT_SLAVE_RX_SET:
            /** Prepare answer to the master:
                ACK to SET Message*/
            TP_GetSlaveToSet((uint8_t*)&m_settings);
            displayNewSettings();
            replyToSet();
            break;
        default:
            qDebug() << "Tecna_Slave::processPendingDatagrams() _ Parse message fails";
            break;
        }


        qDebug() << "processPendingDatagrams(): Received Bytes: " <<m_protocol.m_udprxMesgSize;

        if (l_ParsedRes != -1){
            receivedLabel->setText(m_stringAnswer[l_ParsedRes]);
            qDebug() << m_stringAnswer[l_ParsedRes];
        }
    }

    // qDebug() << "processPendingDatagrams(): Exit ";

//! [2]
}

void Tecna_Slave::displayNewSettings()
{
   uint8_t l_tempCpy[4];
   memcpy((void*)&l_tempCpy[0],(void*)&m_settings.s_IpAddrData[0],4);
   m_ipAddress->setText(m_protocol.getStringFromIp(&l_tempCpy[0]));
   m_NetMask->setText(m_protocol.getStringFromIp(&m_settings.s_IpNetMaskData[0]));
   m_Gateway->setText(m_protocol.getStringFromIp(&m_settings.s_IpDefGWData[0]));
   m_PriDNS->setText(m_protocol.getStringFromIp(&m_settings.s_IpPriDNSData[0]));
   m_SecDNS->setText(m_protocol.getStringFromIp(&m_settings.s_IpSecDNSData[0]));
   if (m_settings.s_IpDHCPData == 1){
       m_DHCP->setCheckState(Qt::Checked);
   } else {
       m_DHCP->setCheckState(Qt::Unchecked);
   }
   QString l_sHostName = "";
   for (int i=0; i<m_settings.s_IpHostNameLEN; i++){
       QChar l_char = QChar::fromLatin1(m_settings.s_IpHostNameData[i]);
       l_sHostName.append(l_char);
   }
   m_HostName->setText(l_sHostName);

   applyConfig();
}

int Tecna_Slave::writeMyData(QByteArray *l_datagram)
{
    int l_bytes = -1;

    qDebug() << "Tecna_Slave::writeMyData";

    // l_bytes = udpSocket->writeDatagram(*l_datagram);
    l_bytes = udpSocket->writeDatagram(*l_datagram, QHostAddress::Broadcast, TEC_PROT_UDP_PROT_PORT);

//    if (udpSocket->state() != QAbstractSocket::ConnectedState) {
//        udpSocket->disconnectFromHost();
//        udpSocket->connectToHost(QHostAddress(QHostAddress::Broadcast),TEC_PROT_UDP_PROT_PORT);
//        // udpSocket->bind(TEC_PROT_UDP_PROT_PORT,QAbstractSocket::DontShareAddress);
//    }


//    if (udpSocket->isOpen() == false)
//        udpSocket->open(QIODeviceBase::ReadWrite);

    // l_bytes = udpSocket->write(*l_datagram);

    return l_bytes;
}
