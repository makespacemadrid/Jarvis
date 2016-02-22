#ifndef SJARVISCONNECTION_H
#define SJARVISCONNECTION_H

#include  <QObject>
#include  "sjarvisnodecomponent.h"

class sJarvisConnection : public QObject
{
    Q_OBJECT
public:
    explicit sJarvisConnection(QObject *parent = 0);

    void setSenderEvent(sJarvisNodeComponent* sender, jarvisEvents event);
    void setSenderEvent(sJarvisNodeComponent* sender, QString event);
    void setDestAction (sJarvisNodeComponent* dest  , jarvisActions action);
    void setDestAction (sJarvisNodeComponent* dest  , QString action);

    void setId(QString id)  {m_id = id;}
    QString id()            {return m_id;}

    void setDelay(quint16 delayms);
    void setup(bool doOnSetup = false);
    bool isValid();
    void setEnabled(bool en);

private:
    bool                    m_enabled;
    QString                 m_id;
    sJarvisNodeComponent*   m_senderObj;
    QString                 m_senderEvent;
    sJarvisNodeComponent*   m_destObj;
    QString                 m_destAction;


signals:
    void activated();
private slots:
    void doAction();
public slots:
    void enable();
    void disable();

};

#endif // SJARVISCONNECTION_H
