#ifndef GRULEEDITORWIDGET_H
#define GRULEEDITORWIDGET_H

#include <QDialog>

#include "sjarvisnodeserver.h"
#include "sjarvisconnection.h"

namespace Ui {
class gRuleEditorWidget;
}

class gRuleEditorWidget : public QDialog
{
    Q_OBJECT

public:
    explicit gRuleEditorWidget(sJarvisNodeServer* nodeServer, sJarvisConnection* conn);
    ~gRuleEditorWidget();

    void reload();
private:
    Ui::gRuleEditorWidget *ui;
    sJarvisNodeServer*      m_nodeServer;
    sJarvisConnection*      m_conn;
};

#endif // GRULEEDITORWIDGET_H
