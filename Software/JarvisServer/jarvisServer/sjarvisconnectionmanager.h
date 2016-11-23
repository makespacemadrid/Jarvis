#ifndef SJARVISCONNECTIONMANAGER_H
#define SJARVISCONNECTIONMANAGER_H

#include <QObject>
#include <QFile>

#include "sjarvisconnection.h"
#include "sjarvisnodeserver.h"

class sJarvisConnectionManager : public QObject
{
    Q_OBJECT
public:
    explicit sJarvisConnectionManager(sJarvisNodeServer* nodeServer, QObject *parent = 0);
    sJarvisConnection*          addConnection(sJarvisConnection *c = 0);
    void                        addConnection(sJarvisNodeComponent* sender, jarvisEvents event, sJarvisNodeComponent* dest,jarvisActions act, u_int16_t delayms = 0);
    void                        addConnection(sJarvisNodeComponent* sender, QString      event, sJarvisNodeComponent* dest,QString       act, u_int16_t delayms = 0);
    QVector<sJarvisConnection*> connections()   {return m_conns;}
    sJarvisNodeServer*          nodeServer()    {return m_nodeServer;}

private:
    QVector<sJarvisConnection*> m_conns;
    sJarvisNodeServer*          m_nodeServer;
signals:

public slots:
    void reconnectSignals();
    void loadConfig();
    void saveConfig();
};

#endif // SJARVISCONNECTIONMANAGER_H
