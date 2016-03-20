#ifndef SJARVISNODE_H
#define SJARVISNODE_H

#include "jarvisProtocol.h"
#include "settings.h"
#include "sjarvisnodecomponent.h"

#include <QObject>
#include <QString>
#include <QList>
#include <QStringList>
#include <QTimer>
#include <QTime>
#include <QTcpSocket>


class sJarvisNodeComponent;

class sJarvisNode :public QObject
{

    Q_OBJECT
public:

    sJarvisNode(QTcpSocket *tcpClient = 0, QObject* parent = 0);
    ~sJarvisNode();
    void setTcpClient(QTcpSocket *tcpClient);
    QTcpSocket* releaseTcpClient();
    QString getId() {return m_nodeSettings.id;}
    QList<sJarvisNodeComponent*> components() {return m_components;}

    void connectTCP(QString host, quint16 port);
    void closeTCP();
    void send(QByteArray data)
    {
        emit tx();
        m_txCount++;
        emit rawInput(data);
        m_commLog.append(data);
        m_tcpClient->write(data);
    }

    bool isValid()
    {
        return (m_tcpClient->isOpen() && m_valid);
    }

    settingList& getNodeSettings()
    {
        return m_nodeSettings;
    }

    quint64 txCount() {return m_txCount;}
    quint64 rxCount() {return m_rxCount;}
    int     pingTime(){return m_lastPingTime;}

protected:
    QTcpSocket       *m_tcpClient;
    QString           m_rxBuffer;
    QString           m_commLog;
    QList<sJarvisNodeComponent*> m_components;
    int               m_lastPingTime;
    QTimer            m_pingTimer;
    QTime             m_pingTime;
    QTimer            m_initTimer;
    QTimer            m_initTimeout;
    QTimer            m_commTimeout;
    bool              m_initDone;
    bool              m_valid;
    bool              m_validated;
    u_int16_t         m_txCount;
    u_int16_t         m_rxCount;

    settingList       m_nodeSettings;

    void parseBuffer(QString& buf);
    void parsePacket(QString& packet);
    void parseComponent(QStringList args);
    //void parseSensor(QStringList args);
    void parseSensors(QStringList args);
    void parseEvent(QStringList args);
    void parseConfig(QStringList args);

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
    void readSocket();
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
    void setWifiConfig(QString essid, QString passwd, bool apMode = false);
    void getComponents() {sendGetComponents();}
    void clearEEPROM();
    void saveEEPROM();
    void reloadNodeSettings();
    void sendConfig(settingList config);

};

#endif // SJARVISNODE_H
