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

void gJarvisServerWidget::removeItem(QObject *obj)
{
    //qDebug() << "Removing from list: " << obj;
    m_nodeWidgets.removeOne((gNodeSimpleWidget*) obj);
    reloadNodeWidgets();
}

void gJarvisServerWidget::addNode(sJarvisNode *n)
{
    gNodeSimpleWidget* w = new gNodeSimpleWidget(n);
    w->setObjectName(n->getId()+"Widget");
    qDebug() << "gJarvisServerWidget::addNode -> Adding widget:" << w << " for client:" <<n;
    QListWidgetItem *item = new QListWidgetItem();
    connect(n,SIGNAL(tx()),ui->txWidget,SLOT(tx()));
    connect(n,SIGNAL(rx()),ui->txWidget,SLOT(rx()));
    item->setSizeHint(w->sizeHint());
    ui->nodeList->addItem(item);
    ui->nodeList->setItemWidget(item,w);
    m_nodeWidgets.append(w);
    connect(m_nodeWidgets.last(),SIGNAL(destroyed(QObject*)),this,SLOT(removeItem(QObject*)));
}

void gJarvisServerWidget::reloadNodeWidgets()
{
    ui->nodeList->clear();
    for(int i = 0 ; i < m_nodeServer->nodes().count() ; i++)
    {
        gNodeSimpleWidget*  w = new gNodeSimpleWidget(m_nodeServer->nodes()[i]);
        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(w->sizeHint());
        ui->nodeList->addItem(item);
        ui->nodeList->setItemWidget(item,w);
    }
}
