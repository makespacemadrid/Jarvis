#include "gnodesimplewidget.h"
#include "ui_gnodesimplewidget.h"

gNodeSimpleWidget::gNodeSimpleWidget(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::gNodeSimpleWidget)
{
    ui->setupUi(this);
}

gNodeSimpleWidget::~gNodeSimpleWidget()
{
    delete ui;
}
