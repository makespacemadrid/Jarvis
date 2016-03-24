#include "sjarvisnodeserver.h"

sJarvisNodeServer::sJarvisNodeServer(QObject *parent) :
    QTcpServer(parent)
{
    connect(this,SIGNAL(newConnection()),this,SLOT(handleNewConn()));
}

void sJarvisNodeServer::handleNewConn()
{
    QTcpSocket* s = this->nextPendingConnection();
    validateClient(s);
}

void sJarvisNodeServer::addNode(sJarvisNode *nNode)
{
    for(int i = 0 ; i < m_nodes.count() ; i++)
    {
        if(m_nodes[i]->getId() == nNode->getId())
        {
            qDebug() << "sJarvisNodeServer::addNode -> Reconnecting node" << nNode->getId();
            m_nodes[i]->setTcpClient(nNode->releaseTcpClient());
            nNode->deleteLater();
            return;
        }
    }
    m_nodes.append(nNode);
    connect(nNode,SIGNAL(destroyed(QObject*)),this,SLOT(removeNode(QObject*)));
    emit new_node(nNode);
}

void sJarvisNodeServer::connectNode(QString host, qint16 port)
{
    QTcpSocket* c = new QTcpSocket(this);
    c->connectToHost(host,port);
    validateClient(c);
}

void sJarvisNodeServer::validateClient(QTcpSocket *client)
{
    qDebug() << "[D]sJarvisNodeServer::validateClient-> New client!";
    sJarvisNode* n = new sJarvisNode(client,this);
    connect(n,SIGNAL(ready()),this,SLOT(nodeReady()));
    connect(n,SIGNAL(disconnected()),n,SLOT(deleteLater()));
}

void sJarvisNodeServer::nodeReady()
{
    sJarvisNode* n = qobject_cast<sJarvisNode*>(sender());
    disconnect(n,SIGNAL(ready()),this,SLOT(nodeReady()));
    disconnect(n,SIGNAL(disconnected()),n,SLOT(deleteLater()));
    addNode(n);
}

void sJarvisNodeServer::removeNode(QObject *obj)
{
    m_nodes.removeAll((sJarvisNode*)obj);
}
