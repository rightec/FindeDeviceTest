#ifndef TECNA_PROTOCOL_H
#define TECNA_PROTOCOL_H

#include <QObject>
#include <QDebug>
#include <QMutex>

#include "tecna_gui_def.h"
#include "tecnaProtocolDef.h"

class tecna_protocol : public QObject
{
    Q_OBJECT
public:
    explicit tecna_protocol(QObject *parent = nullptr);
    void setMacAddressFromString(QString _address, uint8_t _type);
    void setIPFromString(QString _address, uint8_t _type);
    void setHostName(QString _hostName);

    QString m_SrcMacAddressStn;
    QString m_DestMacAddressStn;
    uint8_t m_SrcMacAddress[6];
    uint8_t m_DestMacAddress[6];
    uint8_t m_ipAddress[4];
    uint8_t m_netMask[4];
    uint8_t m_gateway[4];
    uint8_t m_priDns[4];
    uint8_t m_secDns[4];
    uint8_t m_ReceivedUdp[TEC_PROT_MAX_UDP_DATA];
    uint8_t m_udprxMesgSize;
    uint8_t m_settingCode[4];
    QString m_HostName;
    uint8_t m_HostNameLen;

private:

signals:

};

#endif // TECNA_PROTOCOL_H
