#ifndef GNODETESTWIDGET_H
#define GNODETESTWIDGET_H

#include <QGroupBox>

namespace Ui {
class gNodeTestWidget;
}

class gNodeTestWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit gNodeTestWidget(QWidget *parent = 0);
    ~gNodeTestWidget();

private:
    Ui::gNodeTestWidget *ui;
};

#endif // GNODETESTWIDGET_H
