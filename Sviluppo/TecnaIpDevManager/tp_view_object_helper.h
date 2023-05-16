#ifndef TP_VIEW_OBJECT_HELPER_H
#define TP_VIEW_OBJECT_HELPER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QAbstractListModel>
#include <QStringListModel>
#include "tp_view_object.h"
#include "tp_interface_object.h"


class TP_VIEW_OBJECT_HELPER : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<TP_VIEW_OBJECT *> myMainModel    READ getMainModel NOTIFY myModelChanged)
    Q_PROPERTY(int                     procTerminated READ getProcState NOTIFY procTerminated)


public:
    explicit  TP_VIEW_OBJECT_HELPER(QObject *parent = 0);
    Q_INVOKABLE QList<TP_VIEW_OBJECT *> getList() const;
    Q_INVOKABLE QString getIpAddr(int _index);
    Q_INVOKABLE QString getNetMask(int _index);
    Q_INVOKABLE QString getDefGw(int _index);
    Q_INVOKABLE QString getPriDNS(int _index);
    Q_INVOKABLE QString getSecDNS(int _index);
    Q_INVOKABLE QString getDHCPUsed(int _index);
    Q_INVOKABLE QString getHostName(int _index);
    Q_INVOKABLE QString getMacAddress(int _index);
    Q_INVOKABLE TP_VIEW_OBJECT * getItem(int _index);
    Q_INVOKABLE int getListSize(){return m_dataList.count();};
    Q_INVOKABLE void setDataDevice(int _index,QString _ipAddrSet,QString _ipNetMaskSet,
                                   QString _ipGatewaySet, QString _priDnsSet,QString _secDnsSet,
                                   QString _hostNameSet,int dhcpSet);

    Q_INVOKABLE  QList<QString> *getInterfaces(){return m_interface.getInterfaces();};
    Q_INVOKABLE int getNumberOfIf(){return m_interface.getNumberOfIf();};
    Q_INVOKABLE QString getIfName(int _if){return m_interface.getIfName(_if);};
    Q_INVOKABLE void getIfNames(){m_interface.getIfNames();};
    Q_INVOKABLE void selectInterface(int _if){m_interface.selectInterface(_if);};
    Q_INVOKABLE void selectIP(int _ip){m_interface.selectIP(_ip);};
    Q_INVOKABLE bool socketState(){return (m_interface.socketState());};
    Q_INVOKABLE void clearSrcMacAddr(){return m_interface.clearSrcMacAddr();};
    Q_INVOKABLE void clearSrcIPAddr(){return m_interface.clearSrcIPAddr();};
    Q_INVOKABLE void startBroadcasting(){ m_viewTimer.start(2000);
                                         m_dataList.clear();
                                         m_interface.startBroadcasting();};

    QList<TP_VIEW_OBJECT *> getMainModel(){return m_dataList;}
    void getModel(QStringListModel *_model){if (_model != nullptr) m_interface.getModel(_model);};
    void getIPModel(QStringListModel *_model){if (_model != nullptr) m_interface.getIPModel(_model);};
    int rowListCount(){return m_dataList.size();};
    int getProcState(){return m_interface.getProcState();};


private:
    Q_DISABLE_COPY(TP_VIEW_OBJECT_HELPER)
    QList<TP_VIEW_OBJECT *> m_dataList;
    TP_INTERFACE_OBJECT m_interface;
    QTimer m_viewTimer;

    void showScanList();

private slots:
    void onProcTerminated();

signals:
    void myIfsChanged();
    void myModelChanged();

    void preItemAppended();
    void postItemAppended();

    void procTerminated();
};


#endif // TP_VIEW_OBJECT_HELPER_H
