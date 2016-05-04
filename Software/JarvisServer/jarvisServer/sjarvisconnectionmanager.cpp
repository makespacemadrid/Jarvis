#include "sjarvisconnectionmanager.h"

sJarvisConnectionManager::sJarvisConnectionManager(sJarvisNodeServer *nodeServer, QObject *parent) : QObject(parent)
{
    m_nodeServer = nodeServer;
}

sJarvisConnection* sJarvisConnectionManager::addConnection()
{
    sJarvisConnection* conn = new sJarvisConnection(this);
    m_conns.append(conn);
    connect(m_nodeServer,SIGNAL(new_node(sJarvisNode*)),conn,SLOT(registerNode(sJarvisNode*)));
    return conn;
}

void sJarvisConnectionManager::addConnection(sJarvisNodeComponent* sender, jarvisEvents event, sJarvisNodeComponent* dest,jarvisActions act, u_int16_t delayms)
{
    sJarvisConnection* conn = new sJarvisConnection(this);
 //   conn->setSenderEvent(sender,event);
 //   conn->setDestAction(dest,act);
    conn->setDelay(delayms);
    connect(m_nodeServer,SIGNAL(new_node(sJarvisNode*)),conn,SLOT(registerNode(sJarvisNode*)));
    m_conns.append(conn);

}

void sJarvisConnectionManager::addConnection(sJarvisNodeComponent* sender, QString      event, sJarvisNodeComponent* dest,QString       act, u_int16_t delayms)
{
    sJarvisConnection* conn = new sJarvisConnection(this);
 //   conn->setSenderEvent(sender,event);
 //   conn->setDestAction(dest,act);
    conn->setDelay(delayms);
    connect(m_nodeServer,SIGNAL(new_node(sJarvisNode*)),conn,SLOT(registerNode(sJarvisNode*)));
    m_conns.append(conn);
}



void sJarvisConnectionManager::reconnectSignals()
{
    for(int c = 0 ; c < m_conns.count() ; c++)
        for(int n = 0 ; n < m_nodeServer->nodes().count() ; n++)
        {
            m_conns[c]->registerNode(m_nodeServer->nodes()[n]);
        }

}
