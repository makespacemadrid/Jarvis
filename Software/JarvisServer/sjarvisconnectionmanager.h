#ifndef SJARVISCONNECTIONMANAGER_H
#define SJARVISCONNECTIONMANAGER_H

#include <QObject>
#include "sjarvisconnection.h"
#include "sjarvisnodeserver.h"

class sJarvisConnectionManager : public QObject
{
    Q_OBJECT
public:
    explicit sJarvisConnectionManager(sJarvisNodeServer* nodeServer, QObject *parent = 0);
    void addConnection(sJarvisNodeComponent* sender, jarvisEvents event, sJarvisNodeComponent* dest,jarvisActions act, u_int16_t delayms = 0);
    void addConnection(sJarvisNodeComponent* sender, QString      event, sJarvisNodeComponent* dest,QString       act, u_int16_t delayms = 0);
private:
    QVector<sJarvisConnection*> m_conns;
    sJarvisNodeServer*          m_nodeServer;
signals:

public slots:

};

#endif // SJARVISCONNECTIONMANAGER_H
