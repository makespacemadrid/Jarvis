#ifndef GBLINKWIDGET_H
#define GBLINKWIDGET_H

#include <QFrame>
#include <QTimer>
#include <QPainter>
#include <QDebug>

namespace Ui {
class gBlinkWidget;
}

class gBlinkWidget : public QFrame
{
    Q_OBJECT

public:
    explicit gBlinkWidget(QWidget *parent = 0);
    ~gBlinkWidget();
private:
    Ui::gBlinkWidget *ui;
    QTimer m_timer;
    quint8 m_decay_step;
    QColor m_color;

    void paintEvent(QPaintEvent *pe);

signals:
private slots:
    void decay_animation();

public slots:
    void blink(QColor color = QColor(0,255,0));

};

#endif // GBLINKWIDGET_H
