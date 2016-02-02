#include "sjarvistcpserver.h"

sJarvisTcpServer::sJarvisTcpServer(QObject *parent) :
    QTcpServer(parent)
{
    m_connection_attemps = 0;
    m_connection_success = 0;
    m_connection_failed  = 0;

}

void sJarvisTcpServer::incomingConnection(int socketDescriptor)
{
    qDebug() << "[INFO]sJarvisTcpServer::incomingConnection-> incoming:" << socketDescriptor ;
    m_connection_attemps++;
    sJarvisTcpClient *client = new sJarvisTcpClient(this);
    if (client->setSocketDescriptor(socketDescriptor))
    {
        m_connection_success++;
        m_clients.append(client);
        connect(client,SIGNAL(destroyed(QObject*)),this,SLOT(remove_client(QObject*)));
        connect(client,SIGNAL(tx()),this,SIGNAL(tx()));
        connect(client,SIGNAL(rx()),this,SIGNAL(rx()));
        emit new_TCPclient(client);
    }
    else
    {
        delete client;
        m_connection_failed++;
    }
}


void sJarvisTcpServer::remove_client(QObject * obj)
{
    sJarvisTcpClient* client = (sJarvisTcpClient*) obj;
    m_clients.removeAll(client);
}
