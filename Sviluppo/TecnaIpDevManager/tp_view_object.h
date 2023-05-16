#ifndef TP_VIEW_OBJECT_H
#define TP_VIEW_OBJECT_H

#include <QObject>
#include "TP_Protocol/tecnaProtocolDef.h"


class Q_DECL_EXPORT TP_VIEW_OBJECT : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString ipAddress READ getIpAddr WRITE setIpAddress NOTIFY ipAddressChanged)

public:
    explicit TP_VIEW_OBJECT(QObject *parent = nullptr);
    TP_VIEW_OBJECT(const QString &ipAddr,  QObject *parent=0);
    ~TP_VIEW_OBJECT();


    void setIpAddress(const QString &name);
    void setNetMask(const QString &name);
    void setDefGw(const QString &name);
    void setPriDNS(const QString &name);
    void setSecDNS(const QString &name);
    void setDHCPUsed(const QString &name);
    void setHostName(const QString &name);
    void setMacAddress(const QString &name);


    QString getIpAddr(){return m_ipAddress;};
    QString getNetMask(){return m_NetMask;};
    QString getDefGw(){return m_Gateway;};
    QString getPriDNS(){return m_PriDNS;};
    QString getSecDNS(){return m_SecDNS;};
    QString getDHCPUsed(){return m_DHCP;};
    QString getHostName(){return m_HostName;};
    QString getMacAddress(){return m_MacAddress;};

signals:
    void ipAddressChanged();
    void netMaskChanged();
    void defGwChanged();
    void priDNSChanged();
    void secDNSChanged();
    void dhcpChanged();
    void hostNameChanged();
    void macAddressChanged();

private:
    void initPropList();
    void initObject();

    /** Attribute Section  * */
    QString m_ipAddress;
    QString m_NetMask;
    QString m_Gateway;
    QString m_PriDNS;
    QString m_SecDNS;
    QString m_DHCP;
    QString m_HostName;
    QString m_MacAddress;

};

#endif // TP_VIEW_OBJECT_H
