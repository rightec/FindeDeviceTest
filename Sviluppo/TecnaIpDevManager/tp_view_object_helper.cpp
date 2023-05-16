#include "tp_view_object_helper.h"

  TP_VIEW_OBJECT_HELPER::TP_VIEW_OBJECT_HELPER(QObject *parent):
      QObject(parent)
{
    connect(&m_viewTimer, &QTimer::timeout, this, &TP_VIEW_OBJECT_HELPER::showScanList);
    connect(&m_interface, SIGNAL(procTerminated()),this, SLOT(onProcTerminated()));

}


QList<TP_VIEW_OBJECT *> TP_VIEW_OBJECT_HELPER::getList() const
{
    return m_dataList;
}


QString TP_VIEW_OBJECT_HELPER::getIpAddr(int _index)
{
    QString l_SRet = "No Name";

    if ((_index >= 0) && (_index <= m_dataList.count())){
      TP_VIEW_OBJECT * l_item = nullptr;
      l_item = static_cast<TP_VIEW_OBJECT*>(m_dataList.at(_index));

      qDebug() << "Enter index " << _index << l_item->getIpAddr();
      l_SRet = l_item->getIpAddr();
    } //

    return l_SRet;
}

QString TP_VIEW_OBJECT_HELPER::getNetMask(int _index)
{
    QString l_SRet = "No Name";

    if ((_index >= 0) && (_index <= m_dataList.count())){
      TP_VIEW_OBJECT * l_item = nullptr;
      l_item = static_cast<TP_VIEW_OBJECT*>(m_dataList.at(_index));

      qDebug() << "Enter index " << _index << l_item->getNetMask();
      l_SRet = l_item->getNetMask();
    } //

    return l_SRet;
}

QString TP_VIEW_OBJECT_HELPER::getDefGw(int _index)
{
    QString l_SRet = "No Name";

    if ((_index >= 0) && (_index <= m_dataList.count())){
      TP_VIEW_OBJECT * l_item = nullptr;
      l_item = static_cast<TP_VIEW_OBJECT*>(m_dataList.at(_index));

      qDebug() << "Enter index " << _index << l_item->getDefGw();
      l_SRet = l_item->getDefGw();
    } //

    return l_SRet;
}

QString TP_VIEW_OBJECT_HELPER::getPriDNS(int _index)
{
    QString l_SRet = "No Name";

    if ((_index >= 0) && (_index <= m_dataList.count())){
      TP_VIEW_OBJECT * l_item = nullptr;
      l_item = static_cast<TP_VIEW_OBJECT*>(m_dataList.at(_index));

      qDebug() << "Enter index " << _index << l_item->getPriDNS();
      l_SRet = l_item->getPriDNS();
    } //

    return l_SRet;
}

QString TP_VIEW_OBJECT_HELPER::getSecDNS(int _index)
{
    QString l_SRet = "No Name";

    if ((_index >= 0) && (_index <= m_dataList.count())){
      TP_VIEW_OBJECT * l_item = nullptr;
      l_item = static_cast<TP_VIEW_OBJECT*>(m_dataList.at(_index));

      qDebug() << "Enter index " << _index << l_item->getSecDNS();
      l_SRet = l_item->getSecDNS();
    } //

    return l_SRet;
}

QString TP_VIEW_OBJECT_HELPER::getDHCPUsed(int _index)
{    
    QString l_SRet = "No Name";

    if ((_index >= 0) && (_index <= m_dataList.count())){
      TP_VIEW_OBJECT * l_item = nullptr;
      l_item = static_cast<TP_VIEW_OBJECT*>(m_dataList.at(_index));

      qDebug() << "Enter index " << _index << l_item->getDHCPUsed();
      l_SRet = l_item->getDHCPUsed();
    } //

    return l_SRet;
}

QString TP_VIEW_OBJECT_HELPER::getHostName(int _index)
{
  QString l_SRet = "No Name";

  if ((_index >= 0) && (_index <= m_dataList.count())){
    TP_VIEW_OBJECT * l_item = nullptr;
    l_item = static_cast<TP_VIEW_OBJECT*>(m_dataList.at(_index));

    qDebug() << "Enter index " << _index << l_item->getHostName();
    l_SRet = l_item->getHostName();
  } //

  return l_SRet;
}

QString TP_VIEW_OBJECT_HELPER::getMacAddress(int _index)
{
    QString l_SRet = "No Name";

    if ((_index >= 0) && (_index <= m_dataList.count())){
      TP_VIEW_OBJECT * l_item = nullptr;
      l_item = static_cast<TP_VIEW_OBJECT*>(m_dataList.at(_index));

      qDebug() << "Enter index " << _index << l_item->getMacAddress();
      l_SRet = l_item->getMacAddress();

      l_SRet =l_SRet.toUpper();
      QString newSRet;
      QStringList parts = l_SRet.split(":");
      for (int i = 0; i < parts.size(); ++i){
          // parts[i].replace(0, 1, parts[i][0].toUpper());
          QString item = parts[i];
          QString itemList = item.sliced(2);
          newSRet.append("0x");
          int itemSize = itemList.size();
          if (itemSize == 1){
              newSRet.append("0");
              newSRet.append(itemList);
          } else {
              newSRet.append(itemList);
          }
          if (i < parts.size() -1){
              newSRet.append(":");
          } //
      } // end for
      l_SRet = newSRet;


    } //

    return l_SRet;

}

Q_INVOKABLE TP_VIEW_OBJECT * TP_VIEW_OBJECT_HELPER::getItem(int _index)
{
    TP_VIEW_OBJECT *l_SRet = nullptr;

    if ((_index >= 0) && (_index <= m_dataList.count())){
      TP_VIEW_OBJECT * l_item = nullptr;
      l_item = static_cast<TP_VIEW_OBJECT*>(m_dataList.at(_index));

      qDebug() << "Enter index " << _index << l_item;
      l_SRet = l_item;
    } //

    return l_SRet;
}

void TP_VIEW_OBJECT_HELPER::showScanList()
{
    int iCount = -1;
    int hostNameSize = -1;
    uint8_t hostName[256];
    uint8_t l_macAddr[6];
    uint8_t l_ipAddr[4];
    uint8_t l_NetMask[4];
    uint8_t l_GWAddr[4];
    uint8_t l_priDNSAddr[4];
    uint8_t l_secDNSAddr[4];
    uint8_t l_dhcp = 0xFF;

    iCount = TP_GetNumberOfItemInList();
    for (int i = 0; i< iCount; i++){
        emit preItemAppended();

        TP_VIEW_OBJECT *l_item = new TP_VIEW_OBJECT("0.0.0.0");

        hostNameSize = TP_GetHostName(&hostName[0], i);
        QString l_stringToAdd;
        QByteArray l_ba;
        for (int j=0; j< hostNameSize; j++){
            l_ba.append(hostName[j]);
        }
        l_stringToAdd = QString::fromUtf8(l_ba);
        l_item->setHostName(l_stringToAdd);

        /**************MAC ADDRESS ***********************/

        TP_GetMacFromPosition(i, &l_macAddr[0]);
        l_stringToAdd.clear();

        for (int j=0; j< 6; j++){
            l_stringToAdd.append("0x");
            l_stringToAdd.append(QString::number(l_macAddr[j],16));
            if (j < 5)
                l_stringToAdd.append(":");
        } //end for

        l_item->setMacAddress(l_stringToAdd);

        /***************IP ADDRESS **********************/

        TP_GetIpAddrFromPosition(i, &l_ipAddr[0]);
        l_stringToAdd.clear();

        for (int j=3; j > -1; j--){
            l_stringToAdd.append(QString::number(l_ipAddr[j],10));
            if (j > 0)
                l_stringToAdd.append(".");
        } //end for

        l_item->setIpAddress(l_stringToAdd);

        /*************************************/

        TP_GetNetMaskFromPosition(i, &l_NetMask[0]);
        l_stringToAdd.clear();

        for (int j=3; j >-1; j--){
            l_stringToAdd.append(QString::number(l_NetMask[j],10));
            if (j > 0)
                l_stringToAdd.append(".");
        } //end for

        l_item->setNetMask(l_stringToAdd);

        /*************************************/

        TP_GetGWAddrFromPosition(i, &l_GWAddr[0]);
        l_stringToAdd.clear();

        for (int j=3; j > -1; j--){
            l_stringToAdd.append(QString::number(l_GWAddr[j],10));
            if (j > 0)
                l_stringToAdd.append(".");
        } //end for

        l_item->setDefGw(l_stringToAdd);

        /*************************************/

        TP_GetPriDNSFromPosition(i, &l_priDNSAddr[0]);
        l_stringToAdd.clear();

        for (int j=3; j > -1; j--){
            l_stringToAdd.append(QString::number(l_priDNSAddr[j],10));
            if (j > 0)
                l_stringToAdd.append(".");
        } //end for

        l_item->setPriDNS(l_stringToAdd);

       /*************************************/

        TP_GetSecDNSFromPosition(i, &l_secDNSAddr[0]);
        l_stringToAdd.clear();

        for (int j=3; j > -1; j--){
            l_stringToAdd.append(QString::number(l_secDNSAddr[j],10));
            if (j > 0)
                l_stringToAdd.append(".");
        } //end for

        l_item->setSecDNS(l_stringToAdd);

       /*************************************/

        TP_GetDHCPFromPosition(i,&l_dhcp);
        l_stringToAdd.clear();

        l_stringToAdd = (l_dhcp == 0) ? "No" : "Yes";

        l_item->setDHCPUsed(l_stringToAdd);

        /*************************************/

        m_dataList.append(l_item);
        emit postItemAppended();
    }

    if (m_viewTimer.isActive())
        m_viewTimer.stop();
}

void TP_VIEW_OBJECT_HELPER::setDataDevice(int _index,QString _ipAddrSet,QString _ipNetMaskSet,
                               QString _ipGatewaySet, QString _priDnsSet,QString _secDnsSet,
                               QString _hostNameSet,int dhcpSet)
{
    /** Prepare the command to be sent
     **/
    m_interface.setIpAddress(_ipAddrSet);
    m_interface.setNetMask(_ipNetMaskSet);
    m_interface.setGateway(_ipGatewaySet);
    m_interface.setPriDNS(_priDnsSet);
    m_interface.setSecDNS(_secDnsSet);
    m_interface.setHostNameStn(_hostNameSet);
    m_interface.setDHCP(dhcpSet);

    m_interface.startSendCmdProc(_index);
}



void TP_VIEW_OBJECT_HELPER::onProcTerminated()
{
    emit procTerminated();
    qDebug() << "TP_VIEW_OBJECT_HELPER::onProcTerminated()";
}
