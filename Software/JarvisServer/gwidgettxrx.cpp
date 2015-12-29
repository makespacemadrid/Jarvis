#include "gwidgettxrx.h"

gWidgetTxRx::gWidgetTxRx(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumSize(50,25);
    m_background = QColor(0,0,0,0);
    m_rx_color   = QColor(0,255,0,0);
    m_tx_color   = QColor(255,0,0,0);
    m_timer.setInterval(25);
    m_decay_step = 50;
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(decay_animation()));
}

void gWidgetTxRx::paintEvent(QPaintEvent* pe)
{
    QWidget::paintEvent(pe);
    QPainter p(this);
    QColor background(m_background);
    background.setAlpha(255);
    p.setPen(QColor(background));
    QRect rect = this->rect();
    rect.setSize(rect.size()-QSize(1,1));
    p.drawRect(rect);

    p.setPen(QColor(0,0,0,0));
    p.setBrush(m_background);
    p.drawRect(rect);

    QPoint center = rect.center();
    QPoint l_center(center);
    QPoint r_center(center);
    l_center.setX(l_center.x()/2);
    r_center.setX(center.x()+ l_center.x());
    quint16 radius_x = l_center.x()*0.9;
    //quint16 radius_y = l_center.y()*0.9;
    QColor rx_color(m_rx_color);
    rx_color.setAlpha(255);
    p.setPen(rx_color);
    p.setBrush(m_rx_color);
    p.drawEllipse(l_center,radius_x,radius_x);
    //qDebug() << l_center << radius_x << radius_y;

    QColor tx_color(m_tx_color);
    tx_color.setAlpha(255);
    p.setPen(tx_color);
    p.setBrush(m_tx_color);
    p.drawEllipse(r_center,radius_x,radius_x);
}

void gWidgetTxRx::tx()
{
    m_tx_color.setAlpha(250);
    m_timer.start();
    update();
}

void gWidgetTxRx::rx()
{
    m_rx_color.setAlpha(250);
    m_timer.start();
    update();
}

void gWidgetTxRx::decay_animation()
{
    if((m_rx_color.alpha() == 0) && (m_tx_color.alpha() == 0))
    {
        m_timer.stop();
        return;
    }
    if(m_rx_color.alpha() > 0)
    {
        m_rx_color.setAlpha(m_rx_color.alpha()-m_decay_step);
    }

    if(m_tx_color.alpha() > 0)
    {
        m_tx_color.setAlpha(m_tx_color.alpha()-m_decay_step);
    }
    update();
}
