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
    static QString eventName        (jarvisEvents e);
    static QString signalName       (jarvisEvents e);
    static QString actionName       (jarvisActions a);
    static QString slotName         (jarvisActions a, bool withArgs = true);
    static QString slotMethodName   (jarvisActions a, bool withArgs = true);

protected:
    sJarvisNode*    m_parentNode;
    QString         m_id;
    QList<jarvisActions>   m_actions;
    QList<jarvisEvents>    m_CapableEvents;
signals:
    void enabled      (QStringList args = QStringList());
    void disabled     (QStringList args = QStringList());
    void activated    (QStringList args = QStringList());
    void deactivated  (QStringList args = QStringList());
    void dataRead     (QStringList args = QStringList());
    void rawRead      (QStringList args = QStringList());
    void coffeeMaking (QStringList args = QStringList());
    void coffeeMade   (QStringList args = QStringList());
protected slots:


public slots:
    void enable(QStringList args = QStringList());
    void disable(QStringList args = QStringList());
    void activate(QStringList args = QStringList());
    void deactivate(QStringList args = QStringList());
    void toggle(QStringList args = QStringList());
    void readRaw(QStringList args = QStringList());
    void readData(QStringList args = QStringList());
    void dimm(int power) {setColor(QStringList()<<QString::number(power));}
    void dimm(QStringList args = QStringList());
    void glow(QStringList args = QStringList());
    void fade(QStringList args = QStringList());
    void blink(QStringList args = QStringList());
    void cylon(QStringList args = QStringList());
    void beep(QStringList args = QStringList());
    void playTone(QStringList args = QStringList());
    void playRtttl(QStringList args = QStringList());
    void playRtttl(QString nokiaTune)         {playRtttl(QStringList() << nokiaTune);}
    void makeCoffe(QStringList args = QStringList());
    void setColor(quint8 r,quint8 g,quint8 b) {setColor(QStringList()<<QString::number(r)<<QString::number(g)<<QString::number(b));}
    void setColor(QStringList args = QStringList());
    void setLeds(QStringList args = QStringList());
    void display(QStringList args = QStringList());
    void parseEvent(QString component, jarvisEvents event, QStringList args = QStringList());


};
#endif // SJARVISNODECOMPONENT_H
