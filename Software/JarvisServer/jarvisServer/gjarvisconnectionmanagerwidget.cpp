#include "gjarvisconnectionmanagerwidget.h"
#include "ui_gjarvisconnectionmanagerwidget.h"
#include "gjarvisconnectionwidget.h"

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

void gJarvisConnectionManagerWidget::on_addBtn_clicked()
{
    gJarvisConnectionWidget* w = new gJarvisConnectionWidget(this);
    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(w->sizeHint());
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item,w);
}
