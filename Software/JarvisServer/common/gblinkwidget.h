#ifndef GBLINKWIDGET_H
#define GBLINKWIDGET_H

#include <QFrame>
#include <QTimer>
#include <QPainter>
#include <QDebug>
#include <QLCDNumber>

namespace Ui {
class gBlinkWidget;
}

class gBlinkWidget : public QLCDNumber
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
    void displayRead(QStringList args);

};

#endif // GBLINKWIDGET_H
