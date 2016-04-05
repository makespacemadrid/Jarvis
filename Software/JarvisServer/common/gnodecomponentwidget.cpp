#include "gnodecomponentwidget.h"
#include "ui_gnodecomponentwidget.h"
#include <QToolButton>
#include <QHBoxLayout>
#include "qimageselectionwidget.h"
#include "grtttlplayer.h"
#include <QColorDialog>
#include <QLabel>
#include <QApplication>
#include <QSlider>


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

        if      (action == A_DIMM){
            b->deleteLater();
            QSlider* w = new QSlider(this);
            w->setMaximum(100);
            w->setValue(50);
            l->addWidget(w,l->count()/2,l->count()%2);
            connect(w,SIGNAL(valueChanged(int)),m_component,SLOT(dimm(int)));

        }else if(action == A_SET_COLOR){
            connect(b,SIGNAL(clicked()),this,SLOT(selectComponentColor()));
            b->setText(m_component->actionName((m_component->getActions()[i])));
            l->addWidget(b,l->count()/2,l->count()%2);

        }else if(action == A_SET_LEDS){
            connect(b,SIGNAL(clicked()),this,SLOT(sendImage()));
            b->setText(m_component->actionName((m_component->getActions()[i])));
            l->addWidget(b,l->count()/2,l->count()%2);

        }else if(action == A_DISPLAY){
            connect(b,SIGNAL(clicked()),this,SLOT(sendImage()));
            b->setText(m_component->actionName((m_component->getActions()[i])));
            l->addWidget(b,l->count()/2,l->count()%2);
        }else if(action == A_PLAYRTTTL){
            connect(b,SIGNAL(clicked()),this,SLOT(openRtttlPlayer()));
            b->setText(m_component->actionName((m_component->getActions()[i])));
            l->addWidget(b,l->count()/2,l->count()%2);
        }else{
            QString slotName = m_component->slotName(m_component->getActions()[i],false);
            b->setText(m_component->actionName((m_component->getActions()[i])));
            l->addWidget(b,l->count()/2,l->count()%2);
            connect(b,SIGNAL(clicked()),m_component,slotName.toStdString().c_str());
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
        if(events[i] == E_RAW_READ)
        {
            connect(m_component,SIGNAL(rawRead()),w,SLOT(blink()));
            connect(m_component,SIGNAL(rawRead(QStringList)),w,SLOT(displayRead(QStringList)));
            label->setText(m_component->eventName((m_component->getCapableEvents()[i])));

        }else if(events[i] == E_DATA_READ)
        {
            connect(m_component,SIGNAL(dataRead()),w,SLOT(blink()));
            connect(m_component,SIGNAL(dataRead(QStringList)),w,SLOT(displayRead(QStringList)));
            label->setText(m_component->eventName((m_component->getCapableEvents()[i])));

        }else
        {
            QString signalName = m_component->signalName(m_component->getCapableEvents()[i]);
            label->setText(m_component->eventName((m_component->getCapableEvents()[i])));
            connect(m_component,signalName.toStdString().c_str(),w,SLOT(blink()));
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
        m_component->setColor(0,0,0);
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

                //qDebug() << " - args:" << args.size() ;
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


void gNodeComponentWidget::openRtttlPlayer()
{
    gRtttlPlayer* w = new gRtttlPlayer(m_component);
    w->show(); //leak al cerrar la ventana?
}
