#include "sjarvisconnectionmanager.h"

sJarvisConnectionManager::sJarvisConnectionManager(sJarvisNodeServer *nodeServer, QObject *parent) : QObject(parent)
{
    m_nodeServer = nodeServer;
    loadConfig();
}

sJarvisConnection* sJarvisConnectionManager::addConnection(sJarvisConnection* c)
{
    sJarvisConnection* conn = c;
    if(!c)
        conn = new sJarvisConnection(this);

    m_conns.append(conn);
    connect(m_nodeServer,SIGNAL(new_node(sJarvisNode*)),conn,SLOT(registerNode(sJarvisNode*)));
    return conn;
}

void sJarvisConnectionManager::addConnection(sJarvisNodeComponent* sender, jarvisEvents event, sJarvisNodeComponent* dest,jarvisActions act, u_int16_t delayms)
{
    sJarvisConnection* conn = new sJarvisConnection(this);
 //   conn->setSenderEvent(sender,event);
 //   conn->setDestAction(dest,act);
    conn->setDelay(delayms);
    connect(m_nodeServer,SIGNAL(new_node(sJarvisNode*)),conn,SLOT(registerNode(sJarvisNode*)));
    m_conns.append(conn);

}

void sJarvisConnectionManager::addConnection(sJarvisNodeComponent* sender, QString      event, sJarvisNodeComponent* dest,QString       act, u_int16_t delayms)
{
    sJarvisConnection* conn = new sJarvisConnection(this);
 //   conn->setSenderEvent(sender,event);
 //   conn->setDestAction(dest,act);
    conn->setDelay(delayms);
    connect(m_nodeServer,SIGNAL(new_node(sJarvisNode*)),conn,SLOT(registerNode(sJarvisNode*)));
    m_conns.append(conn);
}



void sJarvisConnectionManager::reconnectSignals()
{
    for(int c = 0 ; c < m_conns.count() ; c++)
        for(int n = 0 ; n < m_nodeServer->nodes().count() ; n++)
        {
            m_conns[c]->registerNode(m_nodeServer->nodes()[n]);
        }

}

void sJarvisConnectionManager::loadConfig()
{
    QFile f;
    f.setFileName("rules.conf");
    if(!f.open(QIODevice::ReadOnly))
    {
        qDebug() << "Cannot open:" << f.fileName();
        return;
    }
    QStringList rules = QString(f.readAll()).split("\n");
    if(rules.last().isEmpty())
        rules.removeLast();

    for(int r = 0 ; r < rules.count() ; r++)
    {
        sJarvisConnection* jc = new sJarvisConnection();
        QString line = rules[r];
        QStringList dataBlock = line.split("|");
        if(dataBlock.last().isEmpty())
            dataBlock.removeLast();

        for(int d = 0 ; d < dataBlock.count() ; d++)
        {
            QStringList fields = dataBlock[d].split("@");
            qDebug() << "sJarvisConnectionManager::loadConfig()->" << fields;
            if(fields[0] == "ID")
            {
                jc->setId(fields[1]);
            }
            else if(fields[0] == "TRIGGER")
            {
                jc->addSenderEvent(fields[1],fields[2],fields[3]);
            }
            else if(fields[0] == "ACTION")
            {
                jc->addDestAction(fields[1],fields[2],fields[3]);
            }
        }
        addConnection(jc);
    }
}

void sJarvisConnectionManager::saveConfig()
{
    QByteArray data;
    for(int i = 0 ; i < m_conns.count() ; i++)
    {
        sJarvisConnection* c = m_conns[i];
        data.append("ID@");
        data.append(c->id());
        for(int t = 0 ; t < c->senderIds().count() ; t++)
        {
            data.append("|");
            data.append("TRIGGER@");
            data.append(c->senderIds()[t]);
            data.append("@");
            data.append(c->senderComp()[t]);
            data.append("@");
            data.append(c->senderEvents()[t]);
        }

        for(int t = 0 ; t < c->destIds().count() ; t++)
        {
            data.append("|");
            data.append("ACTION@");
            data.append(c->destIds()[t]);
            data.append("@");
            data.append(c->destComp()[t]);
            data.append("@");
            data.append(c->destActions()[t]);
        }
        data.append("\n");
    }
    QFile f;
    f.setFileName("rules.conf");
    f.open(QIODevice::WriteOnly);
    f.write(data);
    f.close();
}
