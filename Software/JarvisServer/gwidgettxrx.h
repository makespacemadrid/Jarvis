#ifndef GWIDGETTXRX_H
#define GWIDGETTXRX_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QDebug>

class gWidgetTxRx : public QWidget
{
    Q_OBJECT
public:
    explicit gWidgetTxRx(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *pe);
    QTimer m_timer;
    quint8 m_decay_step;
    QColor m_background;
    QColor m_rx_color;
    QColor m_tx_color;
signals:

private slots:
    void decay_animation();

public slots:
    void tx();
    void rx();

};

#endif // GWIDGETTXRX_H
