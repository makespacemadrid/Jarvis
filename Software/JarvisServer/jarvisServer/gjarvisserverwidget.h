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
    Ui::gJarvisServerWidget   *ui;
    sJarvisNodeServer*         m_nodeServer;
    QList<gNodeSimpleWidget*>   m_nodeWidgets;

private slots:
    void on_btnConnectNode_clicked();
    void removeItem(QObject* obj);
public slots:
    void addNode(sJarvisNode* n);
    void reloadNodeWidgets();
};

#endif // GJARVISSERVERWIDGET_H
