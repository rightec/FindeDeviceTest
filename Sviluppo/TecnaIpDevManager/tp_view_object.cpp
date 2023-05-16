#include "tp_view_object.h"
#include "QDebug"

TP_VIEW_OBJECT::TP_VIEW_OBJECT(QObject *parent)
    : QObject{parent}
{

}

TP_VIEW_OBJECT::TP_VIEW_OBJECT(const QString &ipAddr, QObject *parent)
    : QObject(parent)
{
    initObject();
    setIpAddress(ipAddr);
}

TP_VIEW_OBJECT::~TP_VIEW_OBJECT()
{

}

void TP_VIEW_OBJECT::initObject()
{
    m_ipAddress = "0.0.0.0";
    m_NetMask =  "255.255.255.0";
    m_Gateway =  "0.0.0.0";
    m_PriDNS =  "0.0.0.0";
    m_SecDNS =  "0.0.0.0";
    m_DHCP =  "Unknown";
    m_HostName =  "No Name";
    m_MacAddress = "00:00:00:00:00:00";
}

void TP_VIEW_OBJECT::setMacAddress(const QString &name)
{
    if (name != m_MacAddress) {
        m_MacAddress = name;
        emit macAddressChanged();
    }
}


void TP_VIEW_OBJECT::setIpAddress(const QString &name)
{
    if (name != m_ipAddress) {
        m_ipAddress = name;
        emit ipAddressChanged();
    }
}

void TP_VIEW_OBJECT::setNetMask(const QString &name)
{
    if (name != m_NetMask) {
        m_NetMask = name;
        emit netMaskChanged();
    }
}

void TP_VIEW_OBJECT::setDefGw(const QString &name)
{
    if (name != m_Gateway) {
        m_Gateway = name;
        emit defGwChanged();
    }
}
void TP_VIEW_OBJECT::setPriDNS(const QString &name)
{
    if (name != m_PriDNS) {
        m_PriDNS = name;
        emit priDNSChanged();
    }
}

void TP_VIEW_OBJECT::setSecDNS(const QString &name)
{
    if (name != m_SecDNS) {
        m_SecDNS = name;
        emit secDNSChanged();
    }
}

void TP_VIEW_OBJECT::setDHCPUsed(const QString &name)
{
    if (name != m_DHCP) {
        m_DHCP = name;
        emit dhcpChanged();
    }
}

void TP_VIEW_OBJECT::setHostName(const QString &name)
{
    if (name != m_HostName) {
        m_HostName = name;
        emit hostNameChanged();
    }
}

