#include "sjarvisnodecomponent.h"

//El constructor esta en el .h

#define F(v) v

void sJarvisNodeComponent::initArgs(QStringList args)
{
    if((args.count() < 3) ||
       (!args.contains(E_EVENT) || !args.contains(E_ACTION) ))
    {
        m_id = "parseError!";
        return;
    }
    m_id = args[0];
    int actionsIndex = args.indexOf(E_ACTION);
    int eventsIndex = args.indexOf(E_EVENT);
    for(int i  = eventsIndex+1 ; i < actionsIndex ; i++)
    {
        m_CapableEvents.append(jarvisEvents(args[i].toInt()));
    }
    for(int i = actionsIndex+1 ; i < args.count() ; i++)
    {
        m_actions.append(jarvisActions(args[i].toInt()));
    }
}

void sJarvisNodeComponent::enable()
{
    m_parentNode->doAction(m_id,A_ENABLE);
}

void sJarvisNodeComponent::disable()
{
    m_parentNode->doAction(m_id,A_DISABLE);
}

void sJarvisNodeComponent::activate()
{
    m_parentNode->doAction(m_id,A_ACTIVATE);
}

void sJarvisNodeComponent::deactivate()
{
    m_parentNode->doAction(m_id,A_DEACTIVATE);
}

void sJarvisNodeComponent::toggle()
{
    m_parentNode->doAction(m_id,A_TOGGLE);
}

void sJarvisNodeComponent::readRaw()
{
    m_parentNode->doAction(m_id,A_READ_RAW);
}

void sJarvisNodeComponent::readData()
{
    m_parentNode->doAction(m_id,A_READ_DATA);
}

void sJarvisNodeComponent::dimm(uint power)
{
    m_parentNode->doAction(m_id,A_DIMM, QStringList(QString::number(power)));
}

void sJarvisNodeComponent::blink(int freq, int r,int g,int b)
{
    QStringList args;
    args << QString(freq) << QString(r) << QString(g) << QString(b);
    m_parentNode->doAction(m_id,A_BLINK,args);
}

void sJarvisNodeComponent::glow()
{
    m_parentNode->doAction(m_id,A_GLOW);
}

void sJarvisNodeComponent::fade()
{
    m_parentNode->doAction(m_id,A_FADE);
}

void sJarvisNodeComponent::setColor(int r,int g,int b)
{
    QStringList args;
    args << QString::number(r) << QString::number(g) << QString::number(b);
    m_parentNode->doAction(m_id,A_SET_COLOR,args);
}

void sJarvisNodeComponent::cylon()
{
    m_parentNode->doAction(m_id,A_CYLON);
}

void sJarvisNodeComponent::beep(int freq, int dur)
{
    QStringList args;
    args << QString(freq) << QString(dur);
    m_parentNode->doAction(m_id,A_BEEP,args);
}

void sJarvisNodeComponent::makeCoffe()
{
    m_parentNode->doAction(m_id,A_MAKE_COFFEE);
}

void sJarvisNodeComponent::setLeds(QStringList args)
{
    m_parentNode->doAction(m_id,A_SET_LEDS,args);
}

void sJarvisNodeComponent::display(QStringList args)
{
    m_parentNode->doAction(m_id,A_DISPLAY,args);
}

void sJarvisNodeComponent::parseEvent(QString component, jarvisEvents event, QStringList args)
{
    if(component != m_id) return;
    if      (event == E_DISABLED)
    {
        emit disabled();
    }else if(event == E_ENABLED)
    {
        emit enabled();
    }else if(event == E_ACTIVATED)
    {
        emit activated();
    }else if(event == E_DEACTIVATED)
    {
        emit deactivated();
    }else if(event == E_RAW_READ)
    {
        emit rawRead();
        emit rawRead(args);
    }else if(event == E_DATA_READ)
    {
        emit dataRead();
        emit dataRead(args);
    }else if(event == E_GLOBAL_POWERON)
    {
        emit globalPowerOn();
    }else if(event == E_GLOBAL_SHUTDOWN)
    {
        emit globalShutDown();
    }else if(event == E_COFFEE_MAKING)
    {
        emit coffeeMaking();
    }else if(event == E_COFFEE_MADE)
    {
        emit coffeeMade();
    }
}
