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
     QObject(), m_parentNode(parentNode),m_id("unknown")
    {
        initArgs(args);
    }
    QString                getId()            {return m_id;}
    QList<jarvisActions>   getActions()       {return m_actions;}
    QList<jarvisEvents>    getCapableEvents() {return m_CapableEvents;}
    void initArgs(QStringList args);
    static QString eventName   (jarvisEvents e);
    static QString signalName  (jarvisEvents e);
    static QString actionName  (jarvisActions e);
    static QString slotName    (jarvisActions e);

protected:
    sJarvisNode*    m_parentNode;
    QString         m_id;
    QList<jarvisActions>   m_actions;
    QList<jarvisEvents>    m_CapableEvents;
signals:
    void enabled();
    void disabled();
    void activated();
    void deactivated();
    void dataRead();
    void rawRead();
    void dataRead(QStringList args);
    void rawRead(QStringList args);
    void coffeeMaking();
    void coffeeMade();
protected slots:


public slots:
    void enable();
    void disable();
    void activate();
    void deactivate();
    void toggle();
    void readRaw();
    void readData();
    void dimm(int power = 128);
    void glow();
    void fade();
    void blink(int freq, int r=0, int g=0, int b=0);
    void cylon();
    void beep();
    void playTone(int freq = 100, int dur = 150);
    void playRtttl(QString songrtttl = "imperial:d=6,o=5,b=80:8d.,8d.,8d.,8a#4,16f,8d.,8a#4,16f,d.,16p,8a.,8a.,8a.,8a#,16f,8c#.,8a#4,16f,8d.");
    void makeCoffe();
    void setColor(int r, int g, int b);
    void parseEvent(QString component, jarvisEvents event, QStringList args = QStringList());
    void setLeds(QStringList args);
    void display(QStringList args);

};
#endif // SJARVISNODECOMPONENT_H
