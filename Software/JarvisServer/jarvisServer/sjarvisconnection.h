#ifndef SJARVISCONNECTION_H
#define SJARVISCONNECTION_H

#include  <QObject>
#include "sjarvisnode.h"

#include "qtimer.h"


class sJarvisConnection : public QObject
{
    Q_OBJECT
public:
    explicit sJarvisConnection(QObject *parent = 0);
    void operator =(const sJarvisConnection* conn)
    {
        m_enabled   = conn->m_enabled;
        m_id        = conn->m_id;

        m_delay     = conn->m_delay;

        m_senderId          = conn->m_senderId;
        m_senderComponent   = conn->m_senderComponent;
        m_senderObj         = conn->m_senderObj;
        m_senderEvent       = conn->m_senderEvent;

        m_destId         = conn->m_destId;
        m_destComponent  = conn->m_destComponent;
        m_destObj        = conn->m_destObj;
        m_destAction     = conn->m_destAction;
        m_destActionArgs = conn->m_destActionArgs;
        emit updated();
    }

    void operator =(const sJarvisConnection& conn)
    {
        operator =(&conn);
    }

    sJarvisConnection(const sJarvisConnection& conn)
    {
        operator =(&conn);
    }

    void addSenderEvent(QString senderID, QString senderComponent, jarvisEvents event , sJarvisNodeComponent *senderObj = 0);
    void addSenderEvent(QString senderID, QString senderComponent     , QString event , sJarvisNodeComponent *senderObj = 0);
    void addDestAction (QString destID  , QString destComponent, jarvisActions action , QString actionArgs = QString(), sJarvisNodeComponent *destObj = 0);
    void addDestAction (QString destID  , QString destComponent,       QString action , QString actionArgs = QString(), sJarvisNodeComponent *destObj = 0);

    void removeSenderEvent(int index);
    void removeDestAction(int index);

    void setId(QString id)
    {
        m_id = id;
        emit updated();
    }

    QString id()            {return m_id;}

    void    setDelay(quint16 delayms);
    quint16 getDelay()              {return m_delay;}
    bool    isValid();
    void    setEnabled(bool en);

    QVector<QString>    senderIds()     {return m_senderId;}
    QVector<QString>    senderComp()    {return m_senderComponent;}
    QVector<QString>    senderEvents()  {return m_senderEvent;}

    QVector<QString>    destIds()       {return m_destId;}
    QVector<QString>    destComp()      {return m_destComponent;}
    QVector<QString>    destActions()   {return m_destAction;}

private:
    bool                    m_enabled;
    QString                 m_id;

    quint16                 m_delay;
    QTimer                  m_delayTimer;

    QVector<QString>                 m_senderId;
    QVector<QString>                 m_senderComponent;
    QVector<sJarvisNodeComponent*>   m_senderObj;
    QVector<QString>                 m_senderEvent;

    QVector<QString>                 m_destId;
    QVector<QString>                 m_destComponent;
    QVector<sJarvisNodeComponent*>   m_destObj;
    QVector<QString>                 m_destAction;
    QVector<QString>                 m_destActionArgs;


signals:
    void activated();
    void updated();
    void actuallyDoAction(QStringList args = QStringList());

private slots:
    void doAction(QStringList args = QStringList());
public slots:
    void enable();
    void disable();
    void registerNode(sJarvisNode* node);
    void deRegisterComp(QObject* comp);

};

#endif // SJARVISCONNECTION_H
