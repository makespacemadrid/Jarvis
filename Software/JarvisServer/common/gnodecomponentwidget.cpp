#include "gnodecomponentwidget.h"
#include "ui_gnodecomponentwidget.h"
#include <QToolButton>
#include <QHBoxLayout>
#include "qimageselectionwidget.h"
#include <QColorDialog>
#include <QLabel>
#include <QApplication>


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
        QGridLayout* l = (QGridLayout*)ui->actionsBox->layout();
        l->addWidget(b,l->count()/2,l->count()%2);
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
            connect(b,SIGNAL(clicked()),m_component,SLOT(toggle()));
            b->setText("TOGGLE");
        }else if(actions[i] == A_READ_RAW){
            connect(b,SIGNAL(clicked()),m_component,SLOT(readRaw()));
            b->setText("READR");
        }else if(actions[i] == A_READ_DATA){
            connect(b,SIGNAL(clicked()),m_component,SLOT(readData()));
            b->setText("READ");
        }else if(actions[i] == A_DIMM){
            connect(b,SIGNAL(clicked()),m_component,SLOT(dimm()));
            b->setText("DIMM");
        }else if(actions[i] == A_BLINK){
            //connect(b,SIGNAL(clicked()),m_component,SLOT(blink(50)));
            b->setText("BLINK");
        }else if(actions[i] == A_GLOW){
            connect(b,SIGNAL(clicked()),m_component,SLOT(glow()));
            b->setText("GLOW");
        }else if(actions[i] == A_FADE){
            connect(b,SIGNAL(clicked()),m_component,SLOT(fade()));
            b->setText("FADE");
        }else if(actions[i] == A_SET_COLOR){
            connect(b,SIGNAL(clicked()),this,SLOT(selectComponentColor()));
            b->setText("SCOLOR");
        }else if(actions[i] == A_CYLON){
            connect(b,SIGNAL(clicked()),m_component,SLOT(cylon()));
            b->setText("CYLON");
        }else if(actions[i] == A_SET_LEDS){
            connect(b,SIGNAL(clicked()),this,SLOT(sendImage()));
            b->setText("SET_LEDS");
        }else if(actions[i] == A_DISPLAY){
            connect(b,SIGNAL(clicked()),this,SLOT(sendImage()));
            b->setText("DISPLAY");
        }else if(actions[i] == A_MAKE_COFFEE){
            connect(b,SIGNAL(clicked()),m_component,SLOT(makeCoffe()));
            b->setText("MAKE_COFFEE");
        }
    }

    QList<jarvisEvents> events = m_component->getCapableEvents();
    for(int i = 0 ; i < events.count() ; i++)
    {
        gBlinkWidget* w = new gBlinkWidget(ui->eventsBox);
        QLabel* label = new QLabel(ui->eventsBox);
        QGridLayout* l = (QGridLayout*)ui->eventsBox->layout();
        l->addWidget(label,i,0);
        l->addWidget(w,i,1);
        w->setMaximumHeight(50);
        if(events[i] == E_ACTIVATED)
        {
            connect(m_component,SIGNAL(activated()),w,SLOT(blink()));
            label->setText("Activated");

        }else if(events[i] == E_DEACTIVATED)
        {
            connect(m_component,SIGNAL(deactivated()),w,SLOT(blink()));
            label->setText("Deactivated");

        }else if(events[i] == E_ENABLED)
        {
            connect(m_component,SIGNAL(enabled()),w,SLOT(blink()));
            label->setText("Enabled");

        }else if(events[i] == E_DISABLED)
        {
            connect(m_component,SIGNAL(disabled()),w,SLOT(blink()));
            label->setText("Disabled");

        }else if(events[i] == E_RAW_READ)
        {
            connect(m_component,SIGNAL(rawRead()),w,SLOT(blink()));
            connect(m_component,SIGNAL(rawRead(QStringList)),w,SLOT(displayRead(QStringList)));
            label->setText("Raw");

        }else if(events[i] == E_DATA_READ)
        {
            connect(m_component,SIGNAL(dataRead()),w,SLOT(blink()));
            connect(m_component,SIGNAL(dataRead(QStringList)),w,SLOT(displayRead(QStringList)));
            label->setText("Data");

        }else if(events[i] == E_GLOBAL_POWERON)
        {
            connect(m_component,SIGNAL(globalPowerOn()),w,SLOT(blink()));
            label->setText("PowerOn");

        }else if(events[i] == E_GLOBAL_SHUTDOWN)
        {
            connect(m_component,SIGNAL(globalShutDown()),w,SLOT(blink()));
            label->setText("PowerOff");

        }else if(events[i] == E_COFFEE_MAKING)
        {
            connect(m_component,SIGNAL(coffeeMaking()),w,SLOT(blink()));
            label->setText("Coffee Making");

        }else if(events[i] == E_COFFEE_MADE)
        {
            connect(m_component,SIGNAL(coffeeMade()),w,SLOT(blink()));
            label->setText("Cofee Made");
        }
        else
        {
            label->setText("Unknown");
        }
        //w->setMinimumSize(32,32);
        //w->setMaximumSize(32,32);
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
        QStringList args;
        for(int x = 0 ; x < img.width() ; x++)
        {
            for(int y = 0 ; y < img.height() ; y++)
            {
                QRgb colorRgb = img.pixel(QPoint(x,y));
                QColor color(colorRgb);

                //qDebug() << "x:" << x << " - y:" << y << " - r:" << color.red() << " - g:" << color.green() << " - b:" << color.blue();

                args.append(QString::number(y,'f',0));
                args.append(QString::number(x,'f',0));
                args.append(QString::number(color.red() ,'f',0));
                args.append(QString::number(color.green() ,'f',0));
                args.append(QString::number(color.blue() ,'f',0));
                qDebug() << " - args:" << args.size() ;
                if( args.size() >= (32*5) )
                {
                    m_component->display(args);
                    args.clear();
                }
            }
        }
        if(args.size()) m_component->display(args);
    }
}

void gNodeComponentWidget::selectComponentColor()
{
   QColorDialog w;
   if(!w.exec())return;
   QColor q = w.selectedColor();
   m_component->setColor(q.red(),q.green(),q.blue());
}
