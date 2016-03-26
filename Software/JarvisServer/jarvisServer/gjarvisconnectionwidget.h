#ifndef GJARVISCONNECTIONWIDGET_H
#define GJARVISCONNECTIONWIDGET_H

#include <QFrame>
#include "sjarvisconnection.h"

namespace Ui {
class gJarvisConnectionWidget;
}

class gJarvisConnectionWidget : public QFrame
{
    Q_OBJECT

public:
    explicit gJarvisConnectionWidget(sJarvisConnection* conn = 0, QWidget *parent = 0);
    ~gJarvisConnectionWidget();
    void setJarvisConnection(sJarvisConnection* conn,bool sharedConn = false);
    sJarvisConnection*   connection()      {return m_conn;}

private:
    Ui::gJarvisConnectionWidget *ui;
    sJarvisConnection*           m_conn;
    bool                         m_sharedConn;
signals:
    void configureme(sJarvisConnection*);
protected slots:
    void on_editBtn_clicked();
    void reload();

};

#endif // GJARVISCONNECTIONWIDGET_H
