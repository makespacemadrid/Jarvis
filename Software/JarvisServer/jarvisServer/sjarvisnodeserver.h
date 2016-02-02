#ifndef SJARVISNODESERVER_H
#define SJARVISNODESERVER_H

#include "sjarvistcpserver.h"
#include "sjarvisnode.h"

class sJarvisNodeServer : public sJarvisTcpServer
{
    Q_OBJECT
public:
    explicit sJarvisNodeServer(QObject *parent = 0);
    QList<sJarvisNode*>& nodes() {return m_nodes;}

protected:
    QList<sJarvisNode*> m_nodes;

    void addNode(sJarvisNode* nNode);

signals:
    void new_node(sJarvisNode* nNode);
public slots:
    void connectNode(QString host, qint16 port);
    void validateClient(sJarvisTcpClient* client);
    void nodeReady();
    void removeNode(QObject *obj);


};

#endif // SJARVISNODESERVER_H
