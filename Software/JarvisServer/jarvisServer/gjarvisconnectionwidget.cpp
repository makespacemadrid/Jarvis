#include "gjarvisconnectionwidget.h"
#include "ui_gjarvisconnectionwidget.h"

gJarvisConnectionWidget::gJarvisConnectionWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::gJarvisConnectionWidget)
{
    ui->setupUi(this);
}

gJarvisConnectionWidget::~gJarvisConnectionWidget()
{
    delete ui;
}
