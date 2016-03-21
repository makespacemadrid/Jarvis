#include "gruleeditorwidget.h"
#include "ui_gruleeditorwidget.h"

gRuleEditorWidget::gRuleEditorWidget(sJarvisNodeServer* nodeServer, sJarvisConnection* conn) :
    QDialog(0),
    ui(new Ui::gRuleEditorWidget)
{
    ui->setupUi(this);
    m_conn = conn;
    m_nodeServer = nodeServer;
    reload();
}

gRuleEditorWidget::~gRuleEditorWidget()
{
    delete ui;
}


void gRuleEditorWidget::reload()
{
    ui->editId->setText(m_conn->id());
    ui->spinDelay->setValue(m_conn->getDelay());


    for(int n = 0 ; n < m_nodeServer->nodes().count() ; n++)
    {
        ui->comboSourceNode->addItem(m_nodeServer->nodes()[n]->getId());
        ui->comboDestNode->addItem(m_nodeServer->nodes()[n]->getId());
    }

    ui->listEvents->clear();
    for(int e  = 0 ; e < m_conn->senderIds().count() ; e++)
    {
        QString str;
        str.append(m_conn->senderIds()[e]);
        str.append("-");
        str.append(m_conn->senderComp()[e]);
        str.append("-");
        str.append(m_conn->senderEvents()[e]);
        ui->listEvents->addItem(str);
    }

    ui->listActions->clear();
    for(int a  = 0 ; a < m_conn->destIds().count() ; a++)
    {
        QString str;
        str.append(m_conn->destIds()[a]);
        str.append("-");
        str.append(m_conn->destComp()[a]);
        str.append("-");
        str.append(m_conn->destActions()[a]);
        ui->listActions->addItem(str);
    }
}
