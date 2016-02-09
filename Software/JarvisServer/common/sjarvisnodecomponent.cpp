#include "sjarvisnodecomponent.h"

//El constructor esta en el .h

#define F(v) v



QString sJarvisNodeComponent::eventName(jarvisEvents e)
{
    QString result = "Unknown";
    if     (e == E_ENABLED)
        result = "enabled";
    else if(e == E_DISABLED)
        result = "disabled";
    else if(e == E_ACTIVATED)
        result = "activated";
    else if(e == E_DEACTIVATED)
        result = "deactivated";
    else if(e == E_RAW_READ)
        result = "rawRead";
    else if(e == E_DATA_READ)
        result = "dataRead";
    else if(e == E_COFFEE_MAKING)
        result = "coffeeMaking";
    else if(e == E_COFFEE_MADE)
        result = "coffeeMade";

    return result;
}

QString sJarvisNodeComponent::signalName(jarvisEvents e)
{
    QString result = ":";
    result.append(eventName(e));
    result.append("()");
    return result;
}

QString sJarvisNodeComponent::actionName(jarvisActions a)
{
    QString result = "Unknown";
    if     (a == A_ENABLE)
        result = "enable";
    else if(a == A_DISABLE)
        result = "disable";
    else if(a == A_ACTIVATE)
        result = "activate";
    else if(a == A_DEACTIVATE)
        result = "deactivate";
    else if(a == A_TOGGLE)
        result = "toggle";
    else if(a == A_READ_RAW)
        result = "readRaw";
    else if(a == A_READ_DATA)
        result = "readData";
    else if(a == A_DIMM)
        result = "dimm";
    else if(a == A_BLINK)
        result = "blink";
    else if(a == A_GLOW)
        result = "glow";
    else if(a == A_FADE)
        result = "fade";
    else if(a == A_SET_COLOR)
        result = "setColor";
    else if(a == A_CYLON)
        result = "cylon";
    else if(a == A_SET_LEDS)
        result = "setLeds";
    else if(a == A_DISPLAY)
        result = "display";
    else if(a == A_BEEP)
        result = "beep";
    else if(a == A_MAKE_COFFEE)
        result = "makeCoffe";

    return result;
}

QString sJarvisNodeComponent::slotName(jarvisActions a)
{
    QString result = "1";
    result.append(actionName(a));
    result.append("()");
    return result;
}

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

void sJarvisNodeComponent::dimm(int power)
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
    }else if(event == E_COFFEE_MAKING)
    {
        emit coffeeMaking();
    }else if(event == E_COFFEE_MADE)
    {
        emit coffeeMade();
    }
}
