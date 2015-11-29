#ifndef sJarvisTcpClient_H
#define sJarvisTcpClient_H


#include <QTcpSocket>
#include <QDateTime>
#include <QTimer>

#define KEEP_ALIVE_RATE     5000


class sJarvisTcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit sJarvisTcpClient(QObject *parent = 0);
    ~sJarvisTcpClient();

    QString info();

protected:
    QDateTime             m_uptime;
    QTimer*               m_keepAliveT;
    QTime                 m_pingMeter;
    quint16               m_lastPing;
    quint64               m_tx_count;
    quint64               m_rx_count;
    static const quint16  m_keepAliveRate = KEEP_ALIVE_RATE;
signals:
    void tx();
    void rx();
    void ping_time(quint16);
    void socket_rx(const QByteArray& data);
private slots:
    void readSocket();
    void socket_tx(const QByteArray& data);
    void keep_alive();

    //protocolo:
    void ping();
    void pong();
  public slots:
    void closeSocket();
};

#endif // sJarvisTcpClient_H
