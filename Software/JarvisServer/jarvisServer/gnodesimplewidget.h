#ifndef GNODESIMPLEWIDGET_H
#define GNODESIMPLEWIDGET_H

#include <QGroupBox>
#include "sjarvisnode.h"

namespace Ui {
class gNodeSimpleWidget;
}

class gNodeSimpleWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit gNodeSimpleWidget(sJarvisNode* node,QWidget *parent = 0);
    ~gNodeSimpleWidget();

private:
    Ui::gNodeSimpleWidget *ui;
    sJarvisNode*           m_node;
    void connectNode();

protected slots:
    void nodeConnected();
    void nodeDisconnected();
    void deleteWNode();
public slots:
    void on_btnView_clicked();
    void on_btnEditConf_clicked();
};

#endif // GNODESIMPLEWIDGET_H
