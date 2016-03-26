#include "gnodesimplewidget.h"
#include "ui_gnodesimplewidget.h"
#include "jarvisnodetestapp.h"

#include "gnodeconfigdialog.h"

gNodeSimpleWidget::gNodeSimpleWidget(sJarvisNode *node, QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::gNodeSimpleWidget)
{
    m_node = node;
    ui->setupUi(this);
    connectNode();
    connect(ui->btnDelete,SIGNAL(clicked(bool)),this,SLOT(deleteWNode()));
}

gNodeSimpleWidget::~gNodeSimpleWidget()
{
    qDebug() << "gNodeSimpleWidget::~gNodeSimpleWidget()->deleting widget:" << this;
    delete ui;
}

void gNodeSimpleWidget::connectNode()
{
    for(int c = 0 ; c < m_node->components().count() ; c++)
    {
        sJarvisNodeComponent* comp = m_node->components()[c];
        for(int e = 0 ; e < comp->getCapableEvents().count() ; e++)
        {
            QString signalName = comp->signalName(comp->getCapableEvents()[e]);
            connect(comp,signalName.toStdString().c_str(),ui->signalWidget,SLOT(blink()));
        }
    }
    this->setTitle(m_node->getId());
    if(m_node->isValid())
        nodeConnected();
    else
        nodeDisconnected();
    connect(m_node,SIGNAL(tx()),ui->txWidget,SLOT(tx()));
    connect(m_node,SIGNAL(rx()),ui->txWidget,SLOT(rx()));
    connect(m_node,SIGNAL(ready()),this,SLOT(nodeConnected()));
    connect(m_node,SIGNAL(disconnected()),this,SLOT(nodeDisconnected()));
    connect(m_node,SIGNAL(destroyed(QObject*)),this,SLOT(deleteLater()));
}

void gNodeSimpleWidget::nodeConnected()
{

    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::green);
    this->setAutoFillBackground(true);
    this->setPalette(Pal);
    this->repaint();
    ui->btnConnectStatus->setChecked(true);
    ui->btnEditConf->setEnabled(true);
    ui->btnConnectStatus->setPalette(Pal);
    if(!m_node->isValid())
    {
        Pal.setColor(QPalette::Background, Qt::yellow);
        ui->btnConnectStatus->setPalette(Pal);
        QIcon icon(":/resources/iconos/warning.bmp");
        ui->btnConnectStatus->setIcon(icon);
    }
    ui->signalWidget_2->blink(QColor(255,0,0));
}

void gNodeSimpleWidget::nodeDisconnected()
{
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::red);
    this->setAutoFillBackground(true);
    this->setPalette(Pal);
    this->repaint();
    ui->btnConnectStatus->setChecked(false);
    ui->btnConnectStatus->setPalette(Pal);
    ui->btnEditConf->setEnabled(false);
}

void gNodeSimpleWidget::deleteWNode()
{
    qDebug() << "gNodeSimpleWidget::deleteWNode - > Destroying interface and node:" << this;
    m_node->deleteLater();// la destruccion del nodo desencadena la destruccion del widget
    //this->deleteLater();
}

void gNodeSimpleWidget::on_btnView_clicked()
{
    jarvisNodeTestApp* w = new jarvisNodeTestApp(m_node,this);
    w->setAttribute( Qt::WA_DeleteOnClose, true );
    w->show();
}

void gNodeSimpleWidget::on_btnEditConf_clicked()
{
    gNodeConfigDialog w(m_node->getNodeSettings());
    if(w.exec())
    {
        m_node->sendConfig(w.getSettings());
        m_node->saveEEPROM();
    }
}
