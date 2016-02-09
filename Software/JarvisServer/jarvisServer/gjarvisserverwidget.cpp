#include "gjarvisserverwidget.h"
#include "ui_gjarvisserverwidget.h"

gJarvisServerWidget::gJarvisServerWidget(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::gJarvisServerWidget)
{
    ui->setupUi(this);
}

gJarvisServerWidget::~gJarvisServerWidget()
{
    delete ui;
}


void gJarvisServerWidget::setNodeServer(sJarvisNodeServer *ns)
{
    m_nodeServer = ns;
    for(int i = 0 ; i < m_nodeServer->nodes().count() ; i++)
    {
        addNode(m_nodeServer->nodes()[i]);
    }
    connect(m_nodeServer,SIGNAL(new_node(sJarvisNode*)),this,SLOT(addNode(sJarvisNode*)));
}

void gJarvisServerWidget::on_btnConnectNode_clicked()
{
    qConnectDialog w(this);
    if(w.exec())
    {
        if(w.isSerialConnection())
        {
            qDebug() << "[DEBUG] - Serial connection not implemented" ;
        } else
        {
            qDebug() << "[DEBUG] - Connecting" << w.ip() << ":" << w.port() ;
            m_nodeServer->connectNode(w.ip(),w.port());
        }

    }
}

void gJarvisServerWidget::addNode(sJarvisNode *n)
{
    gNodeSimpleWidget* w = new gNodeSimpleWidget(n,this);
    QListWidgetItem *item = new QListWidgetItem();
    connect(n,SIGNAL(tx()),ui->txWidget,SLOT(tx()));
    connect(n,SIGNAL(rx()),ui->txWidget,SLOT(rx()));
    ui->nodeList->addItem(item);
    ui->nodeList->setItemWidget(item,w);
    ui->nodeList->set
}
