#ifndef GJARVISCONNECTIONMANAGERWIDGET_H
#define GJARVISCONNECTIONMANAGERWIDGET_H

#include <QGroupBox>
#include "sjarvisconnectionmanager.h"

namespace Ui {
class gJarvisConnectionManagerWidget;
}

class gJarvisConnectionManagerWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit gJarvisConnectionManagerWidget(QWidget *parent = 0);
    ~gJarvisConnectionManagerWidget();
    void setConnManager(sJarvisConnectionManager* connMan);

private:
    Ui::gJarvisConnectionManagerWidget *ui;
    sJarvisConnectionManager*           m_connManager;

    void addWidgetConnection(sJarvisConnection* conn);
protected slots:
    void on_addBtn_clicked();
public slots:
    void configureRule(sJarvisConnection* conn);
};

#endif // GJARVISCONNECTIONMANAGERWIDGET_H
