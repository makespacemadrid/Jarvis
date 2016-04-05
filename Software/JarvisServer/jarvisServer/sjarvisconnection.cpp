#include "sjarvisconnection.h"

#include <qmetaobject.h>

sJarvisConnection::sJarvisConnection(QObject *parent) : QObject(parent)
{
    m_enabled   = true;
    m_id        = "Configureme";
    m_delay     = 0;
    //connect(&m_delayTimer,SIGNAL(timeout()),this,SLOT(actuallyDoAction()));
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
    emit updated();
}

void sJarvisConnection::addDestAction (QString destID, QString destComponent, jarvisActions action, QString actionArgs, sJarvisNodeComponent *destObj)
{
    QString actString = sJarvisNodeComponent::slotName(action);
    addDestAction(destID,destComponent,actString,actionArgs,destObj);
}

void sJarvisConnection::addDestAction (QString destID, QString destComponent, QString action, QString actionArgs, sJarvisNodeComponent *destObj)
{
    m_destId.push_back(destID);
    m_destComponent.push_back(destComponent);
    m_destObj.push_back(destObj);
    m_destAction.push_back(action);
    m_destActionArgs.push_back(actionArgs);
    emit updated();
}

void sJarvisConnection::removeSenderEvent(int index)
{
    if(index >= m_senderId.count()) return;
    if(m_senderObj[index])
    {
        m_senderObj[index]->disconnect();
    }

    m_senderId.remove(index);
    m_senderComponent.remove(index);
    m_senderEvent.remove(index);
    m_senderObj.remove(index);
}

void sJarvisConnection::removeDestAction(int index)
{
    if(index >= m_destId.count()) return;
    m_destId.remove(index);
    m_destComponent.remove(index);
    m_destAction.remove(index);
    m_destObj.remove(index);
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
void sJarvisConnection::doAction(QStringList args)
{
    emit activated();
    emit actuallyDoAction(args);
    //if(m_delay >0)
    //    m_delayTimer.start(m_delay);
    //else
    //    actuallyDoAction();
}

//void sJarvisConnection::actuallyDoAction(QStringList args)
//{
//    m_delayTimer.stop();
//
//    if(!m_enabled)
//        return;


//    for(int i = 0 ; i < m_destObj.count() ; i++)
//    {
//        if(m_destObj[i])
//        {
            //if (m_destActionArgs[i].length())
            //    QMetaObject::invokeMethod(m_destObj[i],m_destAction[i].toStdString().c_str(),QGenericArgument(m_destActionArgs[i].toStdString().c_str()));

            //else if (args.size() == 1)
//                QMetaObject::invokeMethod(m_destObj[i],m_destAction[i].toStdString().c_str(),QGenericArgument(args));

            //else if (args.size() == 2)
            //    QMetaObject::invokeMethod(m_destObj[i],m_destAction[i].toStdString().c_str(),QGenericArgument(args[0].toStdString().c_str()),QGenericArgument(args[1].toStdString().c_str()));

            //else if (args.size() == 3)
            //    QMetaObject::invokeMethod(m_destObj[i],m_destAction[i].toStdString().c_str(),QGenericArgument(args[0].toStdString().c_str()),QGenericArgument(args[1].toStdString().c_str()),QGenericArgument(args[2].toStdString().c_str()));
            //else
//                QMetaObject::invokeMethod(m_destObj[i],m_destAction[i].toStdString().c_str());
//const QMetaObject* metaObject = m_destObj[i]->metaObject();
//QStringList methods;
//for(int i = metaObject->methodOffset(); i < metaObject->methodCount(); ++i)
//    methods << QString::fromLatin1(metaObject->method(i).methodSignature());
//qDebug() << "methods:"<<methods;

//        }

//    }
//}


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
            for(int c = 0 ; c < node->components().count() ; c++)
            {
                if(node->components()[c]->getId() == m_senderComponent[i])
                {
                    qDebug() << "sJarvisConnection::registerNode -> Registering" << m_id << m_senderEvent[i];
                    m_senderObj[i] = node->components()[c];
                    disconnect(m_senderObj[i],m_senderEvent[i].toStdString().c_str(),this,SLOT(doAction()));// Para evitar conexiones redundantes de desconecta por si ya esta conectada.
                    connect(m_senderObj[i],m_senderEvent[i].toStdString().c_str(),this,SLOT(doAction(QStringList)));
                    connect(m_senderObj[i],SIGNAL(destroyed(QObject*)),this,SLOT(deRegisterComp(QObject*)));
                }
            }
        }
    }

    for(int i = 0 ; i < m_destId.count() ; i++)
    {
        if(m_destId[i] == node->getId())
        {
            for(int c = 0 ; c < node->components().count() ; c++)
            {
                if(node->components()[c]->getId() == m_destComponent[i])
                {
                    qDebug() << "sJarvisConnection::registerNode -> Registering" << m_id << m_destAction[i];
                    m_destObj[i] = node->components()[c];
                    connect(this,SIGNAL(actuallyDoAction(QStringList)),m_destObj[i],m_destAction[i].toStdString().c_str());
                    connect(m_destObj[i],SIGNAL(destroyed(QObject*)),this,SLOT(deRegisterComp(QObject*)));
                }

            }
        }
    }
}

void sJarvisConnection::deRegisterComp(QObject *comp)
{
    qDebug() << "sJarvisConnection::deRegisterComp -> removing:" <<(sJarvisNodeComponent*)comp;
    for(int i = 0 ; i < m_destObj.count() ; i++)
    {
        if(m_destObj[i] == (sJarvisNodeComponent*) comp) m_destObj[i] = 0;
    }

    for(int i = 0 ; i < m_senderObj.count() ; i++)
    {
        if(m_senderObj[i] == (sJarvisNodeComponent*) comp) m_senderObj[i] = 0;
    }
}
