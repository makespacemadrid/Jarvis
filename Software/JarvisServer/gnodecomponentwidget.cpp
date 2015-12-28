#include "gnodecomponentwidget.h"
#include "ui_gnodecomponentwidget.h"
#include <QToolButton>
#include <QHBoxLayout>
#include "qimageselectionwidget.h"
#include <QColorDialog>

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
        }else if(action == A_TOGGLE){
            //connect(b,SIGNAL(clicked()),m_component,SLOT(toggle()));
            b->setText("TOGGLE");
        }else if(actions[i] == A_READ_RAW){
            connect(b,SIGNAL(clicked()),m_component,SLOT(readRaw()));
            b->setText("READR");
        }else if(actions[i] == A_READ_DATA){
            connect(b,SIGNAL(clicked()),m_component,SLOT(readData()));
            b->setText("READ");
        }else if(actions[i] == A_DIMM){
            //connect(b,SIGNAL(clicked()),m_component,SLOT(dimm(50)));
            b->setText("DIMM");
        }else if(actions[i] == A_BLINK){
            //connect(b,SIGNAL(clicked()),m_component,SLOT(blink(50)));
            b->setText("BLINK");
        }else if(actions[i] == A_GLOW){
            connect(b,SIGNAL(clicked()),m_component,SLOT(glow()));
            b->setText("GLOW");
        }else if(actions[i] == A_SET_COLOR){
            connect(b,SIGNAL(clicked()),this,SLOT(selectComponentColor()));
            b->setText("SCOLOR");
        }else if(actions[i] == A_CYLON){
            connect(b,SIGNAL(clicked()),m_component,SLOT(cylon()));
            b->setText("CYLON");
        }else if(actions[i] == A_SET_LEDS){
            connect(b,SIGNAL(clicked()),this,SLOT(sendImage()));
            b->setText("SET_IMG");
        }else if(actions[i] == A_SET_LED){
            //connect(b,SIGNAL(clicked()),this,SLOT(sendImage()));
            b->setText("SLED");
        }
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
        }else if(events[i] == E_RAW_READ)
        {
            connect(m_component,SIGNAL(rawRead()),w,SLOT(blink()));
        }else if(events[i] == E_DATA_READ)
        {
            connect(m_component,SIGNAL(dataRead()),w,SLOT(blink()));
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

void gNodeComponentWidget::sendImage()
{
    qImageSelectionWidget w;
    if(w.exec())
    {
        QImage img = w.scaledImage.convertToFormat(QImage::Format_RGB888);
        uchar *bits = img.bits();
        QStringList args;
        for (int i = 0; i < (img.width() * img.height() * 3); i = i+3)
        {
            args.append(QString::number(i/3,'f',0));
            args.append(QString::number((int) bits[i],'f',0));
            args.append(QString::number((int) bits[i+1],'f',0));
            args.append(QString::number((int) bits[i+2],'f',0));
            if(i%16==0)
            {
                m_component->setLed(args);
                args.clear();
            }

        }
        if(args.size())
            m_component->setLed(args);
    }
}

void gNodeComponentWidget::selectComponentColor()
{
   QColorDialog w;
   if(!w.exec())return;
   QColor q = w.selectedColor();
   m_component->setColor(q.red(),q.green(),q.blue());
}
