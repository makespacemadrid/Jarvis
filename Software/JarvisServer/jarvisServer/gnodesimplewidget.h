#ifndef GNODESIMPLEWIDGET_H
#define GNODESIMPLEWIDGET_H

#include <QGroupBox>

namespace Ui {
class gNodeSimpleWidget;
}

class gNodeSimpleWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit gNodeSimpleWidget(QWidget *parent = 0);
    ~gNodeSimpleWidget();

private:
    Ui::gNodeSimpleWidget *ui;
};

#endif // GNODESIMPLEWIDGET_H
