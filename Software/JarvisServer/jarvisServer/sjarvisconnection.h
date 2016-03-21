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

    void addSenderEvent(QString senderID, QString senderComponent, jarvisEvents event , sJarvisNodeComponent *senderObj = 0);
    void addSenderEvent(QString senderID, QString senderComponent     , QString event , sJarvisNodeComponent *senderObj = 0);
    void addDestAction (QString destID  , QString destComponent, jarvisActions action , sJarvisNodeComponent *destObj = 0);
    void addDestAction (QString destID  , QString destComponent,       QString action , sJarvisNodeComponent *destObj = 0);

    void setId(QString id)  {m_id = id;}
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


signals:
    void activated();
private slots:
    void doAction();
    void actuallyDoAction();
public slots:
    void enable();
    void disable();
    void registerNode(sJarvisNode* node);

};

#endif // SJARVISCONNECTION_H
