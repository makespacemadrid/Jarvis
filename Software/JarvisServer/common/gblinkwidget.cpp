#include "gblinkwidget.h"
#include "ui_gblinkwidget.h"

gBlinkWidget::gBlinkWidget(QWidget *parent) :
    QLCDNumber(parent),
    ui(new Ui::gBlinkWidget)
{
    ui->setupUi(this);
    m_timer.setInterval(32);
    m_decay_step = 50;
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(decay_animation()));
    m_color.setAlpha(255);
    m_timer.start();
}

gBlinkWidget::~gBlinkWidget()
{
    delete ui;
}

void gBlinkWidget::paintEvent(QPaintEvent *pe)
{
    QLCDNumber::paintEvent(pe);
    QPainter p(this);
    p.setBrush(QColor(m_color));
    QRect rect = this->rect();
    rect.setSize(rect.size()-QSize(1,1));
    p.drawRect(rect);
}

void gBlinkWidget::blink(QColor color)
{
    display(value()+1);
    m_color = color;
    m_color.setAlpha(255);
    m_timer.start();
    update();
}

void gBlinkWidget::decay_animation()
{
    if((m_color.alpha() == 0))
    {
        m_timer.stop();
        return;
    }
    if(m_color.alpha() > 0)
    {
        m_color.setAlpha(m_color.alpha()-m_decay_step);
    }
    update();
}

void gBlinkWidget::displayRead(QStringList args)
{
    display(args[0]);
}
