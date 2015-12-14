#ifndef SJARVISNODE_H
#define SJARVISNODE_H

#include "jarvisProtocol.h"
#include "sjarvistcpclient.h"
#include "sjarvisnodecomponent.h"
#include <QObject>
#include <QString>
#include <QList>
#include <QStringList>

class sJarvisNodeComponent;

class sJarvisNode :public QObject
{

    Q_OBJECT
public:

    sJarvisNode(QObject* parent = 0);
    QString getId() {return m_id;}
    QList<sJarvisNodeComponent*> components() {return m_components;}

    void connectTCP(QString host, quint16 port);
    void send(QByteArray data) {emit writeData(data);}
protected:
    QString          m_id;
    sJarvisTcpClient m_tcpClient;
    QString          m_rxBuffer;
    QString          m_commLog;
    QList<sJarvisNodeComponent*> m_components;

    void parseBuffer(QString& buf);
    void parsePacket(QString& packet);
    void parseComponent(QStringList args);
    void parseSensor(QStringList args);
    void parseSensors(QStringList args);
    void parseEvent(QStringList args);

    static QByteArray encodeEspMsg(QStringList args);
    static QByteArray encodeNodeMsg(QStringList args);
    void sendPing();
    void sendGetID();
    void sendGetComponents();
    void sendReadSensor(QString sen);
    void sendReadSensors();
    void sendDoAction(QString componentId,jarvisActions action,QStringList arguments = QStringList());
signals:
    void tx();
    void rx();
    void incomingEvent(QString component,jarvisEvents event, QStringList args = QStringList());
    void rawInput(QByteArray data);
    void newComponent(sJarvisNodeComponent* ncomp);
    void writeData(QByteArray data);
    void sensorReads(QVector<QString> fields,QVector<double> data);
protected slots:
    void data_rx(QByteArray data);
    void initNode();
    void readSensors();
public slots:
    void doAction(QString Component, jarvisActions action, QStringList args = QStringList());

};

#endif // SJARVISNODE_H
