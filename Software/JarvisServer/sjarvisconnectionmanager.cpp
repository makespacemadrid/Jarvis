#include "sjarvisconnectionmanager.h"

sJarvisConnectionManager::sJarvisConnectionManager(sJarvisNodeServer *nodeServer, QObject *parent) : QObject(parent)
{
    m_nodeServer = nodeServer;
}

void sJarvisConnectionManager::addConnection(sJarvisNodeComponent* sender, jarvisEvents event, sJarvisNodeComponent* dest,jarvisActions act, u_int16_t delayms)
{
    sJarvisConnection* conn = new sJarvisConnection();
    conn->setSenderEvent(sender,event);
    conn->setDestAction(dest,act);
    conn->setDelay(delayms);
    m_conns.append(conn);

}

void sJarvisConnectionManager::addConnection(sJarvisNodeComponent* sender, QString      event, sJarvisNodeComponent* dest,QString       act, u_int16_t delayms)
{
    sJarvisConnection* conn = new sJarvisConnection();
    conn->setSenderEvent(sender,event);
    conn->setDestAction(dest,act);
    conn->setDelay(delayms);
    m_conns.append(conn);
}
