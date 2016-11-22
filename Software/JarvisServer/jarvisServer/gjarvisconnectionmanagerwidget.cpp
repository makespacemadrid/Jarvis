#include "gjarvisconnectionmanagerwidget.h"
#include "ui_gjarvisconnectionmanagerwidget.h"
#include "gjarvisconnectionwidget.h"
#include "gruleeditorwidget.h"

gJarvisConnectionManagerWidget::gJarvisConnectionManagerWidget(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::gJarvisConnectionManagerWidget)
{
    ui->setupUi(this);
}

gJarvisConnectionManagerWidget::~gJarvisConnectionManagerWidget()
{
    delete ui;
}

void gJarvisConnectionManagerWidget::setConnManager(sJarvisConnectionManager *connMan)
{
    m_connManager = connMan;
    for(int c = 0 ; c < m_connManager->connections().count() ; c++)
    {
        addWidgetConnection(m_connManager->connections()[c]);
    }
    connect(ui->btnSave,SIGNAL(clicked(bool)),m_connManager,SLOT(saveConfig()));
}

void gJarvisConnectionManagerWidget::addWidgetConnection(sJarvisConnection *conn)
{
    gJarvisConnectionWidget* w = new gJarvisConnectionWidget(conn);
    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(w->sizeHint());
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item,w);
    connect(w,SIGNAL(configureme(sJarvisConnection*)),this,SLOT(configureRule(sJarvisConnection*)));
}

void gJarvisConnectionManagerWidget::on_addBtn_clicked()
{
    sJarvisConnection * c = m_connManager->addConnection();
    addWidgetConnection(c);
}

void gJarvisConnectionManagerWidget::configureRule(sJarvisConnection *conn)
{
    gRuleEditorWidget w(m_connManager->nodeServer(),conn);
    if(w.exec())
    {
        w.save();
        sJarvisConnection nconn = w.getConfiguredConn();
        *conn = nconn;
        m_connManager->reconnectSignals();
    }
}
