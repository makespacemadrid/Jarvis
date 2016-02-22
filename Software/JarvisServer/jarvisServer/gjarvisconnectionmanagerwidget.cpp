#include "gjarvisconnectionmanagerwidget.h"
#include "ui_gjarvisconnectionmanagerwidget.h"

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
