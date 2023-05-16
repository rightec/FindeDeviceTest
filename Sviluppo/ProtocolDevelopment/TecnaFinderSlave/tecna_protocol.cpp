#include "tecna_protocol.h"
#include "TP_Protocol/tecnaProtocol.h"

uint8_t g_unknownarray[32] = {0x4a, 0x2c, 0x0b, 0xcc, 0xf8, 0x90, 0xc2, 0xe2,
                              0x0a, 0xad, 0x78, 0xf8, 0x05, 0xac, 0xdc, 0x8a,
                              0x21, 0xf3, 0xf8, 0xf3, 0xda, 0x14, 0x7c, 0xc0,
                              0x82, 0x7e, 0x67, 0x49, 0x41, 0xa7, 0x50, 0xec};

tecna_protocol::tecna_protocol(QObject *parent)
    : QObject{parent}
{
    for (int i=0; i<TEC_PROT_MAX_UDP_DATA; i++){
        m_ReceivedUdp[i] = 0;
    }

    m_udprxMesgSize = 0;
}


 void tecna_protocol::setMacAddressFromString(QString _address, uint8_t _type)

 {
     std::string mac = _address.toStdString();

     static unsigned int l_macaddress[6];

     sscanf(mac.c_str(),"%x:%x:%x:%x:%x:%x",l_macaddress,l_macaddress+1,
                        l_macaddress+2,l_macaddress+3,
                        l_macaddress+4,l_macaddress+5);

     if (_type == 0){
         // Source macAddress
         for (int i=0;i<6;i++){
             m_SrcMacAddress[i] = (uint8_t)l_macaddress[i];
             qDebug() << i <<": m_SrcMacAddress is " << QString::number(m_SrcMacAddress[i], 16);
         }
         m_SrcMacAddressStn = _address;
         TP_initProtocol(TEC_PROT_ROLE_SLAVE,(uint8_t*)&m_SrcMacAddress,TEC_PROT_CHAR_ID_LSB_TECNA);
     } else {
         // Destination macAddress
         for (int i=0;i<6;i++){
             m_DestMacAddress[i] = (uint8_t)l_macaddress[i];
         }
         m_DestMacAddressStn = _address;

     }

 }

 QString tecna_protocol::getStringFromIp(uint8_t *ipBuffer)
 {
    QString l_SRetVal = "";

    if (ipBuffer != nullptr){
            for (int i=3;i>-1;i--){
                l_SRetVal.append(QString::number(ipBuffer[i],10));
                if (i > 0){
                    l_SRetVal.append(".");
                } //
            } // end for
    } // end if

     return  l_SRetVal;
 }

 void tecna_protocol::setIPFromString(QString _address, uint8_t _type)
 {
     std::string ip = _address.toStdString();

     unsigned int l_ip[4];

     sscanf(ip.c_str(),"%d.%d.%d.%d.",l_ip,l_ip+1,
                        l_ip+2,l_ip+3);

     switch((uint8_t)_type){
     case TEC_PROT_DATA_IPADDR_ID:
         for (int i=0;i<4;i++){
             m_ipAddress[i] = (uint8_t)l_ip[3-i];
         }
         break;
     case TEC_PROT_DATA_NETMASK_ID:
         for (int i=0;i<4;i++){
             m_netMask[i] = (uint8_t)l_ip[3-i];
         }
         break;
     case TEC_PROT_DATA_DEFGWAY_ID:
         for (int i=0;i<4;i++){
             m_gateway[i] = (uint8_t)l_ip[3-i];
         }
         break;
     case TEC_PROT_DATA_PRIDNS_ID:
         for (int i=0;i<4;i++){
             m_priDns[i] = (uint8_t)l_ip[3-i];
         }
         break;
     case TEC_PROT_DATA_SECDNS_ID:
         for (int i=0;i<4;i++){
             m_secDns[i] = (uint8_t)l_ip[3-i];
         }
         break;
     default:
         break;
     }
 }

 void tecna_protocol::setHostName(QString _hostName)
 {
     m_HostName = _hostName;
     m_HostNameLen = m_HostName.size();
 }


