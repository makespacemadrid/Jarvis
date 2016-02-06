#ifndef SJARVISNODE_H
#define SJARVISNODE_H

#include "jarvisProtocol.h"
#include "sjarvistcpclient.h"
#include "sjarvisnodecomponent.h"
#include <QObject>
#include <QString>
#include <QList>
#include <QStringList>
#include <QTimer>

class sJarvisNodeComponent;

class sJarvisNode :public QObject
{

    Q_OBJECT
public:

    sJarvisNode(sJarvisTcpClient* tcpClient = 0, QObject* parent = 0);
    ~sJarvisNode();
    QString getId() {return m_id;}
    QList<sJarvisNodeComponent*> components() {return m_components;}

    void connectTCP(QString host, quint16 port);
    void closeTCP();
    void send(QByteArray data) {emit writeData(data);}
    bool isValid()
    {
        return (m_tcpClient->isOpen() && m_valid);
    }
    quint64 txCount() {return m_tcpClient->txCount();}
    quint64 rxCount() {return m_tcpClient->rxCount();}
    int     pingTime(){return m_lastPingTime;}

protected:
    QString          m_id;
    sJarvisTcpClient *m_tcpClient;
    QString          m_rxBuffer;
    QString          m_commLog;
    QList<sJarvisNodeComponent*> m_components;
    int              m_lastPingTime;
    QTimer           m_pingTimer;
    QTime            m_pingTime;
    QTimer           m_initTimer;
    QTimer           m_initTimeout;
    QTimer           m_commTimeout;
    bool             m_initDone;
    bool             m_valid;

    void parseBuffer(QString& buf);
    void parsePacket(QString& packet);
    void parseComponent(QStringList args);
    //void parseSensor(QStringList args);
    void parseSensors(QStringList args);
    void parseEvent(QStringList args);

    static QByteArray encodeEspMsg(QStringList args);
    static QByteArray encodeNodeMsg(QStringList args);
    void sendPing();
    void sendGetID();
    void sendGetComponents();
    void sendPollSensor(QString sen,int interval = -1);
    void sendPollSensors(int interval = -1);
    void sendStopPolling();
    void sendDoAction(QString componentId,jarvisActions action,QStringList arguments = QStringList());
    void deleteComponents();
signals:
    void tx();
    void rx();
    void incomingEvent(QString component,jarvisEvents event, QStringList args = QStringList());
    void rawInput(QByteArray data);
    void newComponent(sJarvisNodeComponent* ncomp);
    void writeData(QByteArray data);
    void sensorReads(QVector<QString> fields,QVector<double> data);
    void ready();
    void disconnected();
protected slots:
    void data_rx(QByteArray data);
    void validateClient(QByteArray data);
    void initNode();
    void pong();
    void initDone();
    void initTimeout();
    void commTimedOut();
    void socketDisconected();
    void setUpdateInterval(int interval);
public slots:
    void ping();
    void doAction(QString Component, jarvisActions action, QStringList args = QStringList());
    void pollSensor(QString sen = "ALL",int interval = -1);
    void stopPollingSensors();
    void resetNode();
    void getComponents() {sendGetComponents();}

};

#endif // SJARVISNODE_H
