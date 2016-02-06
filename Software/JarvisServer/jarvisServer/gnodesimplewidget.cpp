#include "gnodesimplewidget.h"
#include "ui_gnodesimplewidget.h"
#include "jarvisnodetestapp.h"

gNodeSimpleWidget::gNodeSimpleWidget(sJarvisNode *node, QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::gNodeSimpleWidget)
{
    m_node = node;
    ui->setupUi(this);
    connectNode();
}

gNodeSimpleWidget::~gNodeSimpleWidget()
{
    delete ui;
}

void gNodeSimpleWidget::connectNode()
{
    for(int c = 0 ; c < m_node->components().count() ; c++)
    {
        connect(m_node,SIGNAL(tx()),ui->txWidget,SLOT(tx()));
        connect(m_node,SIGNAL(rx()),ui->txWidget,SLOT(rx()));
        sJarvisNodeComponent* comp = m_node->components()[c];
        this->setTitle(m_node->getId());

        for(int e = 0 ; e < comp->getCapableEvents().count() ; e++)
        {
            QString signalName = comp->signalName(comp->getCapableEvents()[e]);
            connect(comp,signalName.toStdString().c_str(),ui->signalWidget,SLOT(blink()));
        }
    }
}

void gNodeSimpleWidget::on_btnView_clicked()
{
    jarvisNodeTestApp* w = new jarvisNodeTestApp(m_node,this);
    w->setAttribute( Qt::WA_DeleteOnClose, true );
    w->show();
}

void gNodeSimpleWidget::on_btnKill_clicked()
{
    m_node->deleteLater();
    this->deleteLater();
}
