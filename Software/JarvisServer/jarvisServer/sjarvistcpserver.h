#ifndef SJARVISTCPSERVER_H
#define SJARVISTCPSERVER_H

#include <QtNetwork>

#include "sjarvistcpclient.h"

class sJarvisTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit sJarvisTcpServer(QObject *parent = 0);
    quint64 get_connectionAttemps() {return m_connection_attemps;}
    quint64 get_connectionSuccess() {return m_connection_success;}
    quint64 get_connectionFailed()  {return m_connection_failed;}
    quint64 client_count()          {return m_clients.count();}
private:
    quint64 m_connection_attemps;
    quint64 m_connection_success;
    quint64 m_connection_failed;
protected:
    void incomingConnection(int socketDescriptor);
    QList<sJarvisTcpClient*> m_clients;
signals:
    void new_client(sJarvisTcpClient* n_client);
    void tx();
    void rx();
private slots:
    void remove_client(QObject *obj);
public slots:

};

#endif // SJARVISTCPSERVER_H
