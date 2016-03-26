#include "gjarvisserverwidget.h"
#include "ui_gjarvisserverwidget.h"


gJarvisServerWidget::gJarvisServerWidget(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::gJarvisServerWidget)
{
    ui->setupUi(this);
    //this->setStyleSheet("QGroupBox{border:1px solid gray;border-radius:3px;margin-top: 1ex;} QGroupBox::title{subcontrol-origin: margin;subcontrol-position:top center;padding:-10 25px;}");
    this->setStyleSheet("QGroupBox { border: 1px solid gray; border-radius: 9px; margin-top: 0.5em; } QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 3px 0 3px;}");
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
    m_nodeWidgets.removeAll((gNodeSimpleWidget*)obj);
    for(int i = 0 ; i < m_nodeWidgets.count() ; i++)
    {//WTF
        disconnect(m_nodeWidgets[i],SIGNAL(destroyed(QObject*)),this,SLOT(removeItem(QObject*)));
        m_nodeWidgets[i]->deleteLater();
    }
    m_nodeWidgets.clear();
    reloadNodeWidgets();
}

void gJarvisServerWidget::addNode(sJarvisNode *n)
{
    gNodeSimpleWidget* w = new gNodeSimpleWidget(n);
    w->setObjectName(n->getId()+"Widget");
    qDebug() << "gJarvisServerWidget::addNode -> Adding widget:" << w << " for client:" <<n;
    QListWidgetItem *item = new QListWidgetItem(ui->nodeList);
    item->setSizeHint(w->sizeHint());
    ui->nodeList->setItemWidget(item,w);
    connect(n,SIGNAL(tx()),ui->txWidget,SLOT(tx()));
    connect(n,SIGNAL(rx()),ui->txWidget,SLOT(rx()));
    m_nodeWidgets.append(w);
    connect(m_nodeWidgets.last(),SIGNAL(destroyed(QObject*)),this,SLOT(removeItem(QObject*)));
}

void gJarvisServerWidget::reloadNodeWidgets()
{
    qDebug() << "Reloading nodeWidgets";
    ui->nodeList->clear();
    ui->nodeList->updateGeometry();

    for(int i = 0 ; i < m_nodeServer->nodes().count() ; i++)
    {
        qDebug() << "Adding widget: "<< m_nodeServer->nodes()[i];
        gNodeSimpleWidget*  w = new gNodeSimpleWidget(m_nodeServer->nodes()[i]);
        QListWidgetItem *item = new QListWidgetItem(ui->nodeList);
        item->setSizeHint(w->sizeHint());
        ui->nodeList->setItemWidget(item,w);
    }
}
