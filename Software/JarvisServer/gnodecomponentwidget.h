#ifndef GNODECOMPONENTWIDGET_H
#define GNODECOMPONENTWIDGET_H

#include <QGroupBox>
#include "sjarvisnodecomponent.h"
#include "gblinkwidget.h"

namespace Ui {
class gNodeComponentWidget;
}

class gNodeComponentWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit gNodeComponentWidget(sJarvisNodeComponent* comp, QWidget *parent = 0);
    ~gNodeComponentWidget();

private:
    Ui::gNodeComponentWidget *ui;
    sJarvisNodeComponent*     m_component;
};

#endif // GNODECOMPONENTWIDGET_H
