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
    result.append("(QStringList)");
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
    else if(a == A_PLAYTONE)
        result = "playTone";
    else if(a == A_PLAYRTTTL)
        result = "playRtttl";
    else if(a == A_MAKE_COFFEE)
        result = "makeCoffe";

    return result;
}

QString sJarvisNodeComponent::slotName(jarvisActions a,bool withArgs)
{
    QString result = "1";
    result.append(slotMethodName(a,withArgs));
    return result;
}

QString sJarvisNodeComponent::slotMethodName(jarvisActions a,bool withArgs)
{
    QString result;
    result.append(actionName(a));
    if(withArgs)
        result.append("(QStringList)");
    else
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

void sJarvisNodeComponent::enable(QStringList args)
{
    m_parentNode->doAction(m_id,A_ENABLE);
}

void sJarvisNodeComponent::disable(QStringList args)
{
    m_parentNode->doAction(m_id,A_DISABLE);
}

void sJarvisNodeComponent::activate(QStringList args)
{
    m_parentNode->doAction(m_id,A_ACTIVATE);
}

void sJarvisNodeComponent::deactivate(QStringList args)
{
    m_parentNode->doAction(m_id,A_DEACTIVATE);
}

void sJarvisNodeComponent::toggle(QStringList args)
{
    m_parentNode->doAction(m_id,A_TOGGLE);
}

void sJarvisNodeComponent::readRaw(QStringList args)
{
    m_parentNode->doAction(m_id,A_READ_RAW);
}

void sJarvisNodeComponent::readData(QStringList args)
{
    m_parentNode->doAction(m_id,A_READ_DATA);
}

void sJarvisNodeComponent::dimm(QStringList args)
{
    if(args.count() == 0)
        args << QString::number(50);
    m_parentNode->doAction(m_id,A_DIMM, args);
}

void sJarvisNodeComponent::blink(QStringList args)
{
    m_parentNode->doAction(m_id,A_BLINK,args);
}

void sJarvisNodeComponent::glow(QStringList args)
{
    m_parentNode->doAction(m_id,A_GLOW);
}

void sJarvisNodeComponent::fade(QStringList args)
{
    m_parentNode->doAction(m_id,A_FADE);
}

void sJarvisNodeComponent::setColor(QStringList args)
{
    if(args.count() == 0)
        args << QString::number(100) << QString::number(100) << QString::number(100);
    m_parentNode->doAction(m_id,A_SET_COLOR,args);
}

void sJarvisNodeComponent::cylon(QStringList args)
{
    m_parentNode->doAction(m_id,A_CYLON);
}

void sJarvisNodeComponent::beep(QStringList args)
{
    m_parentNode->doAction(m_id,A_BEEP,args);
}

void sJarvisNodeComponent::playTone(QStringList args)
{
    if(args.count() == 0)
        args << QString::number(150) << QString::number(100);
    m_parentNode->doAction(m_id,A_PLAYTONE,args);
}

void sJarvisNodeComponent::playRtttl(QStringList args)
{
    if(args.count() == 0)
        args << "imperial:d=6,o=5,b=80:8d.,8d.,8d.,8a#4,16f,8d.,8a#4,16f,d.,16p,8a.,8a.,8a.,8a#,16f,8c#.,8a#4,16f,8d.";
    m_parentNode->doAction(m_id,A_PLAYRTTTL,args);
}

void sJarvisNodeComponent::makeCoffe(QStringList args)
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
        emit rawRead(args);
    }else if(event == E_DATA_READ)
    {
        emit dataRead(args);
    }else if(event == E_COFFEE_MAKING)
    {
        emit coffeeMaking();
    }else if(event == E_COFFEE_MADE)
    {
        emit coffeeMade();
    }
}
