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
    void save();
    sJarvisConnection getConfiguredConn() {return m_conn;}
private:
    Ui::gRuleEditorWidget *ui;
    sJarvisNodeServer*      m_nodeServer;
    sJarvisConnection       m_conn;

protected slots:
    void on_comboSourceNode_currentTextChanged(QString);
    void on_comboDestNode_currentTextChanged(QString);
    void on_comboNextAction_currentTextChanged(QString);
    void on_btnAddAction_clicked();
    void on_btnAddEvent_clicked();
    void on_btnRemoveAction_clicked();
    void on_btnRemoveEvent_clicked();
};

#endif // GRULEEDITORWIDGET_H
