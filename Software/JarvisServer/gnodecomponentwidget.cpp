#include "gnodecomponentwidget.h"
#include "ui_gnodecomponentwidget.h"
#include <QToolButton>
#include <QHBoxLayout>

gNodeComponentWidget::gNodeComponentWidget(sJarvisNodeComponent* comp,QWidget *parent) :
    QGroupBox(parent), m_component(comp),
    ui(new Ui::gNodeComponentWidget)
{
    ui->setupUi(this);

    this->setTitle(m_component->getId());

    QList<jarvisActions> actions = m_component->getActions();
    for(int i = 0 ; i < actions.count() ; i++)
    {
        jarvisActions action = actions[i];
        //qDebug() << QString::number(int(action));
        QToolButton* b = new QToolButton(ui->actionsBox);
        ui->actionsBox->layout()->addWidget(b);
        b->setText(QString::number(action,'f',0));

        if      (action == A_ENABLE){
            connect(b,SIGNAL(clicked()),m_component,SLOT(enable()));
            b->setText("EN");
        }
        else if(action == A_DISABLE){
            connect(b,SIGNAL(clicked()),m_component,SLOT(disable()));
            b->setText("DIS");
        }else if(action == A_ACTIVATE){
            connect(b,SIGNAL(clicked()),m_component,SLOT(activate()));
            b->setText("ACT");
        }else if(action == A_DEACTIVATE){
            connect(b,SIGNAL(clicked()),m_component,SLOT(deactivate()));
            b->setText("DEA");
        }
//        else if(actions[i] == A_DIMM)
//            connect(b,SIGNAL(clicked()),m_component,SLOT(enable()));
//        else if(actions[i] == A_SET_COLOR)
//            connect(b,SIGNAL(clicked()),m_component,SLOT(enable()));
//        else if(actions[i] == A_CYLON)
//            connect(b,SIGNAL(clicked()),m_component,SLOT(enable()));
//        else if(actions[i] == A_BEEP)
//            connect(b,SIGNAL(clicked()),m_component,SLOT(enable()));
//        else if(actions[i] == A_MAKE_COFFEE)
//            connect(b,SIGNAL(clicked()),m_component,SLOT(enable()));
    }

    QList<jarvisEvents> events = m_component->getCapableEvents();
    for(int i = 0 ; i < events.count() ; i++)
    {
        gBlinkWidget* w = new gBlinkWidget(ui->eventsBox);
        ui->eventsBox->layout()->addWidget(w);
        if(events[i] == E_ACTIVATED)
        {
            connect(m_component,SIGNAL(activated()),w,SLOT(blink()));
        }else if(events[i] == E_DEACTIVATED)
        {
            connect(m_component,SIGNAL(deactivated()),w,SLOT(blink()));
        }else if(events[i] == E_ENABLED)
        {
            connect(m_component,SIGNAL(enabled()),w,SLOT(blink()));
        }else if(events[i] == E_DISABLED)
        {
            connect(m_component,SIGNAL(disabled()),w,SLOT(blink()));
        }else if(events[i] == E_GLOBAL_POWERON)
        {
            connect(m_component,SIGNAL(globalPowerOn()),w,SLOT(blink()));
        }else if(events[i] == E_GLOBAL_SHUTDOWN)
        {
            connect(m_component,SIGNAL(globalShutDown()),w,SLOT(blink()));
        }
        w->setMinimumSize(32,32);
        w->setMaximumSize(32,32);
    }
}

gNodeComponentWidget::~gNodeComponentWidget()
{
    delete ui;
}
