#ifndef SJARVISNODECOMPONENT_H
#define SJARVISNODECOMPONENT_H

#include <QObject>
#include <QStringList>
#include "jarvisProtocol.h"
#include "sjarvisnode.h"

class sJarvisNode;

class sJarvisNodeComponent : public QObject
{
    Q_OBJECT
public:
    sJarvisNodeComponent(sJarvisNode *parentNode,QStringList args = QStringList()) :
     QObject(), m_parentNode(parentNode)
    {
        initArgs(args);
    }
    QString                getId()            {return m_id;}
    QList<jarvisActions>   getActions()       {return m_actions;}
    QList<jarvisEvents>    getCapableEvents() {return m_CapableEvents;}
    void initArgs(QStringList args);
protected:
    QString         m_id = "unknown";
    sJarvisNode*    m_parentNode;
    QList<jarvisActions>   m_actions;
    QList<jarvisEvents>    m_CapableEvents;
signals:
    void enabled();
    void disabled();
    void activated();
    void deactivated();
    void globalPowerOn();
    void globalShutDown();
    void coffeeMaking();
    void coffeeMade();
protected slots:
    void enable();
    void disable();
    void activate();
    void deactivate();
    void dimm(uint power = 128);
    void blink(int freq, int r=0, int g=0, int b=0);
    void setColor(int r, int g, int b);
    void cylon();
    void beep(int freq = 300, int dur = 300);
    void makeCoffe();

public slots:
    void parseEvent(QString component, jarvisEvents event, QStringList args = QStringList());

};
#endif // SJARVISNODECOMPONENT_H
