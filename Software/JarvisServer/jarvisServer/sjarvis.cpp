#include "sjarvis.h"
#include <QFile>

sJarvis::sJarvis(QObject *parent) :
    QObject(parent), m_connectionManager(&m_nodeServer)
{
        m_nodeServer.listen(QHostAddress::Any,31416);
/*
        sJarvisConnection* conn = m_connectionManager.addConnection();
        conn->addSenderEvent("MakeSwitch","switch",E_ACTIVATED);
        conn->addDestAction("ledPanel","ledPanel",A_ACTIVATE);
        conn->setId("Global On");

        sJarvisConnection* conn1 = m_connectionManager.addConnection();
        conn1->addSenderEvent("MakeSwitch","switch",E_DEACTIVATED );
        conn1->addDestAction("ledPanel","ledPanel",A_DEACTIVATE);
        conn1->setId("Global Off");
        saveSettings();
*/
        loadSettings();
}

void sJarvis::saveSettings()
{
    QString lineSep ="\n" ;
    QString fieldSep = "|";
    QFile f("settings.ini");
    QByteArray fdata;

    for(int i = 0 ; i < m_connectionManager.connections().count() ; i++)
    {
        sJarvisConnection* c = m_connectionManager.connections()[i];
        QString result = "conn";
        result.append(fieldSep);
        result.append(c->id());
        result.append(fieldSep);
        for(int a = 0 ; a < c->senderEvents().count() ; a++)
        {
            result.append(c->senderComp()[a]);
            result.append(fieldSep);
            result.append(c->senderIds()[a]);
            result.append(fieldSep);
            result.append(c->senderEvents()[a]);
            result.append(fieldSep);
        }
        result.append("DEST");
        result.append(fieldSep);
        for(int a = 0 ; a < c->destActions().count() ; a++)
        {
            result.append(c->destComp()[a]);
            result.append(fieldSep);
            result.append(c->destIds()[a]);
            result.append(fieldSep);
            result.append(c->destActions()[a]);
        }
        result.append(lineSep);
        fdata.append(result);
    }

    if(!f.open(QIODevice::WriteOnly))
    {
        qDebug() << "Cannot open:" << f.fileName();
    }
    f.write(fdata);
    f.close();
    qDebug() << "Settings saved:" << f.fileName();
}

void sJarvis::loadSettings()
{
    QString lineSep ="\n" ;
    QString fieldSep = "|";
    QFile f("settings.ini");

    if(!f.open(QIODevice::ReadOnly))
    {
        qDebug() << "Cannot open:" << f.fileName();
    }
    QString data = f.readAll();

    QStringList lines = data.split(lineSep);
    for(int i = 0 ; i < lines.count() ; i++)
    {
        QStringList fields = lines[i].split(fieldSep);
        if(fields[0] == "conn")
        {
            sJarvisConnection* conn = m_connectionManager.addConnection();
            conn->setId(fields[1]);
            bool destSeparatorSeen = false;

            for(int f = 2 ; f < fields.count(); f++)
            {
                if(fields[f] == "DEST")
                {
                    destSeparatorSeen = true;
                    f++;
                }

                if(destSeparatorSeen)
                {
                    conn->addDestAction(fields[f++],fields[f++],fields[f++]);
                }
                else
                {
                    conn->addSenderEvent(fields[f++],fields[f++],fields[f]);
                }
            }
        }
    }
}
