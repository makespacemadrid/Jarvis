#include "sjarvisconnection.h"

sJarvisConnection::sJarvisConnection(QObject *parent) : QObject(parent)
{
    m_senderObj = 0;
    m_destObj   = 0;
    m_enabled   = true;
}

void sJarvisConnection::setSenderEvent(sJarvisNodeComponent* sender, jarvisEvents event)
{
    QString evString = sender->signalName(event);
    setSenderEvent(sender,evString);
}

void sJarvisConnection::setSenderEvent(sJarvisNodeComponent* sender, QString event)
{
    m_senderObj   = sender;
    m_senderEvent = event;
}

void sJarvisConnection::setDestAction (sJarvisNodeComponent* dest  , jarvisActions action)
{
    QString actString = dest->slotName(action);
    setDestAction(dest,actString);
}

void sJarvisConnection::setDestAction (sJarvisNodeComponent* dest  , QString action)
{
    m_destObj    = dest;
    m_destAction = action;
}

void sJarvisConnection::setDelay(quint16 delayms)
{

}

void sJarvisConnection::setup(bool doOnSetup)
{
    if(!m_senderObj || !m_destObj)
        return;

    connect(m_senderObj,m_senderEvent.toStdString().c_str(),this,SLOT(doAction()));

    if(doOnSetup)
        doAction();
}

bool sJarvisConnection::isValid()
{
    return true;
}

void sJarvisConnection::setEnabled(bool en)
{
    if(en)
        enable();
    else
        disable();
}

//private:

//private slots:
void sJarvisConnection::doAction()
{
    if(!m_destObj || !m_enabled)
        return;

    QMetaObject::invokeMethod(m_destObj,m_destAction.toStdString().c_str());

    emit activated();
}

//public slots:
void sJarvisConnection::enable()
{
    m_enabled = true;
}

void sJarvisConnection::disable()
{
    m_enabled = false;
}
