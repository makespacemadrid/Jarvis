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
    sJarvisNode* n = qobject_cast<sJarvisNode*>(sender());
    m_nodes.removeAll(n);
}
