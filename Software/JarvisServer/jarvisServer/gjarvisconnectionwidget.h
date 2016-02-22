#ifndef GJARVISCONNECTIONWIDGET_H
#define GJARVISCONNECTIONWIDGET_H

#include <QFrame>

namespace Ui {
class gJarvisConnectionWidget;
}

class gJarvisConnectionWidget : public QFrame
{
    Q_OBJECT

public:
    explicit gJarvisConnectionWidget(QWidget *parent = 0);
    ~gJarvisConnectionWidget();

private:
    Ui::gJarvisConnectionWidget *ui;
};

#endif // GJARVISCONNECTIONWIDGET_H
