#include "gjarvisconnectionmanagerwidget.h"
#include "ui_gjarvisconnectionmanagerwidget.h"
#include "gjarvisconnectionwidget.h"

gJarvisConnectionManagerWidget::gJarvisConnectionManagerWidget(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::gJarvisConnectionManagerWidget)
{
    ui->setupUi(this);
    sJarvisConnection* conn = new sJarvisConnection();
    conn->addSenderEvent("MakeSwitch","switch",E_ACTIVATED);
    conn->setId("Global Switch");
    gJarvisConnectionWidget* w = new gJarvisConnectionWidget(conn);
    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(w->sizeHint());
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item,w);
}

gJarvisConnectionManagerWidget::~gJarvisConnectionManagerWidget()
{
    delete ui;
}

void gJarvisConnectionManagerWidget::on_addBtn_clicked()
{
    gJarvisConnectionWidget* w = new gJarvisConnectionWidget();
    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(w->sizeHint());
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item,w);
}
