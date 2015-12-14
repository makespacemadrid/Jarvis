#ifndef SJARVISNODESERVER_H
#define SJARVISNODESERVER_H

#include "sjarvistcpserver.h"
#include "sjarvisnode.h"

class sJarvisNodeServer : public sJarvisTcpServer
{
    Q_OBJECT
public:
    explicit sJarvisNodeServer(QObject *parent = 0);

protected:
    QList<sJarvisNode*> m_nodes;

signals:
    void new_node(sJarvisNode* nNode);
public slots:

};

#endif // SJARVISNODESERVER_H
