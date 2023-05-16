#ifndef TP_INTERFACE_OBJECT_H
#define TP_INTERFACE_OBJECT_H

#include <QObject>
#include "QUdpSocket"
#include "QNetworkInterface"
#include "QStringListModel"
#include "QTimer"
#include "TP_Protocol/tecnaProtocol.h"

enum sState{
  TP_IF_START_PROC = 0, // Procedure has started
  TP_IF_SEND_WARN  = 1, // Procedure has sended WARN command
  TP_IF_ACK_WARN   = 2, // Procedure has received WARN command from client
  TP_IF_SEND_SET   = 3, // Procedure has sended SET command
  TP_IF_ACK_SET    = 4, // Procedure has received SET command from client
  TP_IF_SEND_SCAN  = 5, // Procedure has sended SCAN command
  TP_IF_END_PROC   = 6  // Procedure has ended
};

class TP_INTERFACE_OBJECT : public QObject
{
    Q_OBJECT
public:
    explicit TP_INTERFACE_OBJECT(QObject *parent = nullptr);
    QList<QString> *getInterfaces(){return m_nicName;};
    int getNumberOfIf(){return m_nic->count();};
    QString getIfName(int _ifName){return m_nicName->at(_ifName);}
    void getModel(QStringListModel *_model);
    void getIPModel(QStringListModel *_model);
    void getIfNames();
    void selectInterface(int _if){emit interfaceChanged(_if);};
    void selectIP(int _ip){emit ipChanged(_ip);};
    bool socketState(){return (m_udpSocket->state() == QAbstractSocket::BoundState) ? true : false;};
    void clearSrcMacAddr();
    void clearSrcIPAddr();
    void startBroadcasting();

    void setIpAddress(QString _name){m_ipAddressStn = _name;};
    void setNetMask(QString _name){m_NetMaskStn = _name;};
    void setGateway(QString _name){m_GatewayStn = _name;};
    void setPriDNS(QString _name){m_PriDNSStn = _name;};
    void setSecDNS(QString _name){m_SecDNSStn = _name;};
    void setHostNameStn(QString _name){m_HostNameStn = _name;};
    void setDHCP(int _val){if ((_val == 0) || (_val == 1)) m_DHCPVal = _val;};

    int startSendCmdProc(int _index);
    int getProcState(){return m_StateProc;};

signals:
    void procTerminated();


private:
    QUdpSocket *m_udpSocket = nullptr;
    QUdpSocket *m_udpSendSocket = nullptr;
    QList<QNetworkInterface> *m_nic = nullptr;
    QList<QString> *m_nicName = nullptr;
    QList<QString> *m_ipAddr = nullptr;
    QStringListModel *m_if_model = nullptr;
    QStringListModel *m_ip_model = nullptr;
    uint8_t m_SrcMacAddress[6];
    uint8_t m_DestMacAddress[6];
    uint8_t m_ReceivedUdp[TEC_PROT_MAX_UDP_DATA];
    uint8_t m_udprxMesgSize;
    QTimer m_timer;
    QString m_HostAdd;

    // Prop to protocol
    uint8_t m_ipAddress[4];
    uint8_t m_netMask[4];
    uint8_t m_gateway[4];
    uint8_t m_priDns[4];
    uint8_t m_secDns[4];
    QString m_HostName;
    uint8_t m_HostNameLen;

    // Properties to send from Interface
    QString m_ipAddressStn;
    QString m_NetMaskStn;
    QString m_GatewayStn;
    QString m_PriDNSStn;
    QString m_SecDNSStn;
    int     m_DHCPVal;
    QString m_HostNameStn;

    // Prop to manage procedure
    int     m_StateProc = TP_IF_END_PROC;
    int     m_IndexItemProc = -1;
    QTimer  m_SimThreadTmr;
    QTimer  m_GenProcTmr;
    QTimer  m_StateProcTmr;
    bool    m_Semaphore;


   void setMacAddressFromString(QString _address, uint8_t _type);
   void broadcastDatagram();
   int setCmd(int _pos);
   int  warnCmd(int _pos);
   void setIPFromString(QString _address, uint8_t _type);
   void setHostName(QString _hostName);
   void resetProc();
   void startProc();

private slots:
    void onInterfaceChanged(int _if);
    void onIPChanged(int _ip);
    void onPendingDatagrams();
    void onPendingProcedureFSM();
    void onStoppingProcedureFSM();
    void onWarnAckArrived();
    void onSetAckArrived();
    void onScanAckArrived();
    void onReceivedPkct();


signals:
    void interfaceChanged(int _if);
    void ipChanged(int _ip);
    void warnAckArrived();
    void setAckArrived();
    void scanAckArrived();

};

#endif // TP_INTERFACE_OBJECT_H
