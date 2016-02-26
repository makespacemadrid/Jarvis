#include "sjarvisconnection.h"

sJarvisConnection::sJarvisConnection(QObject *parent) : QObject(parent)
{
    m_enabled   = true;
    m_id        = "Configureme";
    m_delay     = 0;
    connect(&m_delayTimer,SIGNAL(timeout()),this,SLOT(actuallyDoAction()));
}

void sJarvisConnection::addSenderEvent(QString senderID, QString senderComponent , jarvisEvents event, sJarvisNodeComponent* senderObj)
{
    QString evString = sJarvisNodeComponent::signalName(event);
    addSenderEvent(senderID,senderComponent,evString,senderObj);
}

void sJarvisConnection::addSenderEvent(QString senderID, QString senderComponent , QString event, sJarvisNodeComponent *senderObj)
{
    m_senderId.push_back(senderID);
    m_senderComponent.push_back(senderComponent);
    m_senderObj.push_back(senderObj);
    m_senderEvent.push_back(event);

    if(m_senderObj.last())
        connect(m_senderObj.last(),m_senderEvent.last().toStdString().c_str(),this,SLOT(doAction()));
}

void sJarvisConnection::addDestAction (QString destID, QString destComponent, jarvisActions action, sJarvisNodeComponent *destObj)
{
    QString actString = sJarvisNodeComponent::slotName(action);
    addDestAction(destID,destComponent,actString,destObj);
}

void sJarvisConnection::addDestAction (QString destID, QString destComponent,QString action, sJarvisNodeComponent *destObj)
{
    m_destId.push_back(destID);
    m_destComponent.push_back(destComponent);
    m_destObj.push_back(destObj);
    m_destAction.push_back(action);
}

void sJarvisConnection::setDelay(quint16 delayms)
{
    m_delay = delayms;
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
    if(m_delay >0)
        m_delayTimer.start(m_delay);
    else
        actuallyDoAction();
}

void sJarvisConnection::actuallyDoAction()
{
    if(m_delay > 0)
        m_delayTimer.stop();

    if(!m_enabled)
        return;

    for(int i = 0 ; i < m_destObj.count() ; i++)
    {
        if(m_destObj[i])
            QMetaObject::invokeMethod(m_destObj[i],m_destAction[i].toStdString().c_str());
    }

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

void sJarvisConnection::registerNode(sJarvisNode *node)
{
    for(int i = 0 ; i < m_senderId.count() ; i++)
    {
        if(m_senderId[i] == node->getId())
        {

        }
    }

    for(int i = 0 ; i < m_destId.count() ; i++)
    {
        if(m_destId[i] == node->getId())
        {

        }
    }
}
