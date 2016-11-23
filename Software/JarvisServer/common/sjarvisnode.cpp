#include "sjarvisnode.h"

//hack para eliminar el F() que hay en el protocolo para que se salve en la rom del arduino
#define F(v) v

sJarvisNode::sJarvisNode(QTcpSocket* tcpClient, QObject* parent) : QObject(parent)
{
    m_txCount = 0;
    m_rxCount = 0;
    m_tcpClient = 0;
    m_validated = false;
    m_nodeSettings.magicNumber = 0;
    if(tcpClient == 0)
    {
        setTcpClient(new QTcpSocket(this));
    }
    else
    {
        setTcpClient(tcpClient);
    }

    connect(&m_pingTimer,SIGNAL(timeout()),this,SLOT(ping()));
    connect(&m_commTimeout,SIGNAL(timeout()),this,SLOT(commTimedOut()));
    connect(&m_initTimer,SIGNAL(timeout()),this,SLOT(initDone()));
    connect(&m_initTimeout,SIGNAL(timeout()),this,SLOT(initTimeout()));

    if(m_tcpClient->isOpen())
        initNode();
}

sJarvisNode::~sJarvisNode()
{
    if(m_tcpClient != 0)
        m_tcpClient->deleteLater();
    deleteComponents();
}

void sJarvisNode::setTcpClient(QTcpSocket *tcpClient)
{

    if(m_tcpClient != 0)
        m_tcpClient->deleteLater();

    m_tcpClient = tcpClient;

    connect(m_tcpClient,SIGNAL(readyRead()),this,SLOT(readSocket()));
    connect(m_tcpClient,SIGNAL(connected()),this,SLOT(initNode()));
    connect(m_tcpClient,SIGNAL(disconnected()),this,SLOT(socketDisconected()));

    if(m_validated) initDone();
}

QTcpSocket *sJarvisNode::releaseTcpClient()
{
    disconnect(m_tcpClient,SIGNAL(readyRead()),this,SLOT(readSocket()));
    disconnect(m_tcpClient,SIGNAL(connected()),this,SLOT(initNode()));
    disconnect(m_tcpClient,SIGNAL(disconnected()),this,SLOT(socketDisconected()));
    QTcpSocket* sock = m_tcpClient;
    m_tcpClient = 0;
    return sock;
}

void sJarvisNode::connectTCP(QString host, quint16 port)
{
    m_tcpClient->connectToHost(host,port);
}

void sJarvisNode::closeTCP()
{
    qDebug() << "sJarvisNode::closeTCP()" << sender();
    m_tcpClient->close();
    m_pingTimer.stop();
    //deleteComponents();
}

//protected:

void sJarvisNode::parseBuffer(QString& buf)
{
    //qDebug() << "BufferStart:" << buf;
    //qDebug() << "-PacketSeparators:" << P_PACKETSTART << P_PACKETSEPARATOR << P_PACKETTERMINATOR;
    if(buf.length() == 0) return;
    int s_index = buf.indexOf(P_PACKETSTART);
    int e_index = buf.indexOf(P_PACKETTERMINATOR);
    //saneado del buffer
    //qDebug() << "indexs:" << s_index << "indexe" << e_index;
    if(s_index < 0)
    {// si no hay inicio de paquete lo que hay en el buffer tiene que ser basura.
        //qDebug() << "Limpiando Buffer";
        buf.clear();
        return;
    }
    //extraccion de comandos
    int packetStartLength = QString(P_PACKETSTART).size();
    int packetEndLength   = QString(P_PACKETTERMINATOR).size();
    while ((s_index >= 0) && (e_index >= 0)) //Si hay inicio y fin de paquete se extrae el comando.
    {// lo que haya en el buffer hasta el inicio de paquete se descarta(basura)
        QString packet = buf.mid(s_index+packetStartLength,e_index-s_index-packetStartLength);
        parsePacket(packet);
        buf = buf.mid(e_index+packetEndLength);
        s_index = buf.indexOf(P_PACKETSTART);
        e_index = buf.indexOf(P_PACKETTERMINATOR);
    }
    //qDebug() << "Buffer:End" << m_rxBuffer;
}

void sJarvisNode::parsePacket(QString& packet)
{
    //qDebug() << "Packet:" << packet;
    QStringList args;
    args = packet.split(P_PACKETSEPARATOR);
    if(args.count() < 2) return;
    //qDebug() << "Packet:" << args;
    if (args[0] != M_JARVISMSG) return;
    args.removeFirst();
    QString arg = args[0];
    args.removeFirst();

    if      (arg == C_ID)
    {
        if(args.count() != 1) return;
        for(uint i = 0 ; (i < sizeof(m_nodeSettings.id)) && (i < args[0].length()) ; i++)
        {
            m_nodeSettings.id[i] = args[0].toLatin1().data()[i];
        }
    }else if(arg == C_PONG)
    {
        pong();
    }else if(arg == C_COMPONENT)
    {
        parseComponent(args);
    }else if(arg == C_SENSORS)
    {
        parseSensors(args);
    }else if(arg == C_CONFIG)
    {
        parseConfig(args);
    }else if(arg == E_EVENT)
    {
        parseEvent(args);
    }else
    {
        qDebug() << "[sJarvisNode::parsePacket]daFuckIsThis:"<< args;
    }
}

void sJarvisNode::parseComponent(QStringList args)
{
    if(m_initDone) return;
    m_initTimer.start(200);
    m_components.append(new sJarvisNodeComponent(this,args));
    connect(this,SIGNAL(incomingEvent(QString,jarvisEvents,QStringList)),m_components.last(),SLOT(parseEvent(QString,jarvisEvents,QStringList)));
    emit newComponent(m_components.last());
}

void sJarvisNode::parseSensors(QStringList args)
{
    if(args.count() < 2) return;
    QVector<QString>  fields;
    QVector<double>    data;
    for(int i = 0 ; i < args.count()-1 ; i++)
    {
        fields.append(args[i]);
        i++;
        data.append(args[i].toDouble());
    }
    emit sensorReads(fields,data);
}

void sJarvisNode::parseEvent(QStringList args)
{
    if(!args.count())return;
    QString component = args.first();
    args.removeFirst();
    if(!args.count())return;
    jarvisEvents event = jarvisEvents(args.first().toInt());
    args.removeFirst();
    emit incomingEvent(component,event,args);
}

void sJarvisNode::parseConfig(QStringList args)
{
    if(args.count() != sizeof(settingList))
    {
        qDebug() << "[sJarvisNode::parseConfig]Length not as expected" << args.count() << "/" <<(int)sizeof(settingList) ;
        return;
    }
    settingList s;
    char * byteStorage = (char *)&s;
    for (size_t i = 0; i < sizeof(settingList) ; i++)
      byteStorage[i] = args[i].toInt();

    //qDebug() << "[sJarvisNode::parseConfig]" << s.id << s.wifiESSID;
    m_nodeSettings = s;
}

//repuestas para el protocolo:

QByteArray sJarvisNode::encodeEspMsg(QStringList args)
{
    QByteArray result;
    if(args.isEmpty()) return result;
    result += P_PACKETSTART;
    result += M_ESPMSG;
    result += P_PACKETSEPARATOR;
    result += args[0];
    for (int i = 1 ; i < args.size() ; i++)
    {
        result += P_PACKETSEPARATOR;
        result += args[i];
    }
    result += P_PACKETTERMINATOR;
    return result;
}

QByteArray sJarvisNode::encodeNodeMsg(QStringList args)
{
    QByteArray result;
    if(args.isEmpty()) return result;
    result += P_PACKETSTART;
    result += M_NODEMSG;
    result += P_PACKETSEPARATOR;
    result += args[0];
    for (int i = 1 ; i < args.size() ; i++)
    {
        result += P_PACKETSEPARATOR;
        result += args[i];
    }
    result += P_PACKETTERMINATOR;
    return result;
}

void sJarvisNode::sendPing()
{
    send(encodeNodeMsg(QStringList(QString(C_PING))));//formula abreviada para crear una lista de argumntos con un unico elemento y mandarlo
}

void sJarvisNode::sendGetID()
{
    send(encodeNodeMsg(QStringList(QString(C_GETID))));
}

void sJarvisNode::sendGetComponents()
{
    deleteComponents();
    m_initDone = false;
    send(encodeNodeMsg(QStringList(QString(C_GETCOMPONENTS))));
}

void sJarvisNode::sendPollSensor(QString sen, int interval)
{
    QStringList args;
    args.append(C_POLLSENSOR);
    args.append(sen);
    if(interval)
        args.append(QString(interval));
    send(encodeNodeMsg(args));
}

void sJarvisNode::sendPollSensors(int interval)
{
    QStringList args;
    args.append(C_POLLSENSORS);
    if(interval >= 0)
        args.append(QString::number(interval,'f',0));
    send(encodeNodeMsg(args));
}

void sJarvisNode::sendStopPolling()
{
    send(encodeNodeMsg(QStringList(QString(C_STOP_POLLING))));
}

void sJarvisNode::sendDoAction(QString componentId,jarvisActions action, QStringList arguments)
{
    QStringList args;
    args.append(C_DOACTION);
    args.append(componentId);
    args.append(QString::number(action));
    args.append(arguments);
    send(encodeNodeMsg(args));
}


//protected Slots:
void sJarvisNode::readSocket()
{
    emit rx();
    m_rxCount++;
    QByteArray data = m_tcpClient->readAll();
    m_commLog.append(data);    
    emit rawInput(data);
    //qDebug() << "Received:" << data.count();
    //for(int i = 0 ; i < data.count() ; i++)
    //{
    //    qDebug() << (quint8)data[i];
    //}
    if(m_validated)
    {
        m_rxBuffer.append(data);
        parseBuffer(m_rxBuffer);
    }
    else
        validateClient(data);
}

void sJarvisNode::initNode()
{
    qDebug() << "sJarvisNode::initNode()";
    m_initDone =false;
    send(encodeNodeMsg(QStringList(QString(M_JARVIS_GREETING))));
    m_initTimeout.start(5000);
}


void sJarvisNode::validateClient(QByteArray data)
{
    QString greet;
    greet += P_PACKETSTART ;
    greet += M_JARVISMSG;
    greet += P_PACKETSEPARATOR;
    greet += M_NODE_GREETING;
    greet += P_PACKETTERMINATOR;
    if(data == greet)
    {
        m_validated = true;
        reloadNodeSettings();
    }
    else
    {
        qDebug() << "sJarvisNode::validateClient-> Wrong greet, rejecting";
        qDebug() << data;
        closeTCP();
        this->deleteLater();
    }
}

void sJarvisNode::ping()
{
    sendPing();
    if(!m_commTimeout.isActive())
    {
        m_commTimeout.start(m_pingTimer.interval()*3.5);
        m_pingTime.start();
    }
}

void sJarvisNode::commTimedOut()
{
    emit rawInput("Timeout, Disconnected!");
    this->closeTCP();
}

void sJarvisNode::pong()
{
    m_lastPingTime=m_pingTime.elapsed();
    qDebug() << "Pong:" <<m_pingTime.elapsed() <<"ms";
    m_commTimeout.stop();
}

void sJarvisNode::initDone()
{
    m_initTimer.stop();
    m_initTimeout.stop();
    if( (m_nodeSettings.magicNumber != 31415) || m_components.isEmpty())
    {
        qDebug() << "Incompatible client or some problem on the init stage!";
        qDebug() << "Magic number" << m_nodeSettings.magicNumber << "ID:" << m_nodeSettings.id << "Components:" << m_components.count();
        m_valid = false;
    }
    else
    {
        m_valid = true;
    }

    m_initDone = true;
    m_pingTimer.start(10000);
    emit ready();
}

void sJarvisNode::initTimeout()
{
    qDebug() << "sJarvisNode::initTimeout()";
    closeTCP();
    m_initTimeout.stop();
    emit disconnected();
}

void sJarvisNode::socketDisconected()
{
    qDebug() << "sJarvisNode::socketDisconected()";
    m_pingTimer.stop();
    m_initTimer.stop();
    m_commTimeout.stop();
    emit disconnected();
}


void sJarvisNode::deleteComponents()
{
    for(int i = 0 ; i < m_components.count() ; i++)
    {
        m_components[i]->deleteLater();
    }
    m_components.clear();
}

void sJarvisNode::setUpdateInterval(int interval)
{
    QStringList args;
    args.append(C_SET_UPDATE_INTERVAL);
    args.append(QString::number(interval));
    send(encodeNodeMsg(args));
}

//public Slots

void sJarvisNode::doAction(QString Component, jarvisActions action, QStringList args)
{
    sendDoAction(Component,action,args);
}

void sJarvisNode::pollSensor(QString sen, int interval)
{
    if(sen == "ALL")
        sendPollSensors(interval);
    else
        sendPollSensor(sen,interval);
}

void sJarvisNode::stopPollingSensors()
{
    sendStopPolling();
}

void sJarvisNode::resetNode()
{
    QStringList args;
    args.append(C_RESET);
    send(encodeNodeMsg(args));
}


void sJarvisNode::setWifiConfig(QString essid, QString passwd,bool apMode)
{
    if(apMode)
    {
        QStringList args;
        args.append(C_SETAP);
        args << essid << passwd;
        send(encodeNodeMsg(args));
    }else{
        QStringList args;
        args.append(C_SETCLIENT);
        args << essid << passwd;
        send(encodeNodeMsg(args));
    }
}

void sJarvisNode::clearEEPROM()
{
    QStringList args;
    args.append(C_CLEAR_EEPROM);
    send(encodeNodeMsg(args));
}

void sJarvisNode::saveEEPROM()
{
    QStringList args;
    args.append(C_SAVE_EEPROM);
    send(encodeNodeMsg(args));
}

void sJarvisNode::reloadNodeSettings()
{
    QStringList args;
    args.append(C_GET_CONFIG);
    send(encodeNodeMsg(args));
}

void sJarvisNode::sendConfig(settingList config)
{
    QStringList args;
    args.append(C_SET_CONFIG);

    char * byteStorage = (char *)&config;
    for (size_t i = 0; i < sizeof(settingList) ; i++)
      args.append(QString::number((quint8)byteStorage[i]));

    send(encodeNodeMsg(args));
    reloadNodeSettings();
}
