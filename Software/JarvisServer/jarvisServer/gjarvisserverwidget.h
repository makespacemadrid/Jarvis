#ifndef GJARVISSERVERWIDGET_H
#define GJARVISSERVERWIDGET_H

#include <QGroupBox>
#include "sjarvisnodeserver.h"
#include "gnodesimplewidget.h"
#include "qconnectdialog.h"

namespace Ui {
class gJarvisServerWidget;
}

class gJarvisServerWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit gJarvisServerWidget(QWidget *parent = 0);
    ~gJarvisServerWidget();
    void setNodeServer(sJarvisNodeServer* ns);

private:
    Ui::gJarvisServerWidget *ui;
    sJarvisNodeServer*      m_nodeServer;

private slots:
    void on_btnConnectNode_clicked();
public slots:
    void addNode(sJarvisNode* n);
};

#endif // GJARVISSERVERWIDGET_H
