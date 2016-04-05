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
    ui->editId->setText(m_conn.id());
    ui->spinDelay->setValue(m_conn.getDelay());

    ui->comboSourceNode->clear();
    ui->comboDestNode->clear();
    ui->comboNextAction->clear();

    for(int n = 0 ; n < m_nodeServer->nodes().count() ; n++)
    {
        ui->comboSourceNode->addItem(m_nodeServer->nodes()[n]->getId());
        ui->comboDestNode->addItem(m_nodeServer->nodes()[n]->getId());
    }

    ui->listEvents->clear();
    for(int e  = 0 ; e < m_conn.senderIds().count() ; e++)
    {
        QString str;
        str.append(m_conn.senderIds()[e]);
        str.append("-");
        str.append(m_conn.senderComp()[e]);
        str.append("-");
        str.append(m_conn.senderEvents()[e]);
        ui->listEvents->addItem(str);
    }

    ui->listActions->clear();
    for(int a  = 0 ; a < m_conn.destIds().count() ; a++)
    {
        QString str;
        str.append(m_conn.destIds()[a]);
        str.append("-");
        str.append(m_conn.destComp()[a]);
        str.append("-");
        str.append(m_conn.destActions()[a]);
        ui->listActions->addItem(str);
    }
}

void gRuleEditorWidget::save()
{
    m_conn.setId(ui->editId->text());
    m_conn.setDelay(ui->spinDelay->value());
}

void gRuleEditorWidget::on_comboSourceNode_currentTextChanged(QString str)
{
    qDebug() << "gRuleEditorWidget::on_comboSourceNode_currentTextChanged-> selected:" << str;
    ui->listNodeEvents->clear();
    for(int i = 0 ; i < m_nodeServer->nodes().count() ; i++)
    {
        sJarvisNode* n = m_nodeServer->nodes()[i];
        if(str == n->getId())
        {
            for(int c = 0 ; c < n->components().count() ; c++)
            {
                sJarvisNodeComponent* comp = n->components()[c];
                for(int e = 0 ; e < comp->getCapableEvents().count() ; e++)
                {
                    QListWidgetItem* item = new QListWidgetItem(ui->listNodeEvents);
                    item->setText(comp->getId() +
                                  "-" +
                                  comp->signalName(comp->getCapableEvents()[e]));

                }
            }
            break;
        }
    }
}

void gRuleEditorWidget::on_comboDestNode_currentTextChanged(QString str)
{
    qDebug() << "gRuleEditorWidget::on_comboDestNode_currentTextChanged-> selected:" << str;
    ui->listNodeActions->clear();
    for(int i = 0 ; i < m_nodeServer->nodes().count() ; i++)
    {
        sJarvisNode* n = m_nodeServer->nodes()[i];
        if(str == n->getId())
        {
            for(int c = 0 ; c < n->components().count() ; c++)
            {
                sJarvisNodeComponent* comp = n->components()[c];
                for(int e = 0 ; e < comp->getActions().count() ; e++)
                {
                    QListWidgetItem* item = new QListWidgetItem(ui->listNodeActions);
                    item->setText(comp->getId() +
                                  "-" +
                                  comp->slotName(comp->getActions()[e]));

                }
            }
            break;
        }
    }
}

void gRuleEditorWidget::on_comboNextAction_currentTextChanged(QString str)
{

}

void gRuleEditorWidget::on_btnAddAction_clicked()
{
    if(ui->listNodeActions->selectedItems().count() == 0)return;
    QString destID = ui->comboDestNode->itemText(ui->comboDestNode->currentIndex());
    QString destComp = ui->listNodeActions->currentItem()->text().split("-")[0];
    QString actionName =ui->listNodeActions->currentItem()->text().split("-")[1];
    m_conn.addDestAction(destID,destComp,actionName);
    reload();
}

void gRuleEditorWidget::on_btnAddEvent_clicked()
{
    if(ui->listNodeEvents->selectedItems().count() == 0)return;
    QString senderID = ui->comboSourceNode->itemText(ui->comboSourceNode->currentIndex());
    QString sourceComp = ui->listNodeEvents->currentItem()->text().split("-")[0];
    QString eventName =ui->listNodeEvents->currentItem()->text().split("-")[1];
    m_conn.addSenderEvent(senderID,sourceComp,eventName);
    reload();
}

void gRuleEditorWidget::on_btnRemoveAction_clicked()
{
    if(ui->listActions->selectedItems().count() == 0)return;
    m_conn.removeDestAction(ui->listActions->currentRow());
    reload();

}

void gRuleEditorWidget::on_btnRemoveEvent_clicked()
{
    if(ui->listEvents->selectedItems().count() == 0)return;
    m_conn.removeSenderEvent(ui->listEvents->currentRow());
    reload();
}
