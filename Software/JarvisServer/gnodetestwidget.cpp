#include "gnodetestwidget.h"
#include "ui_gnodetestwidget.h"

gNodeTestWidget::gNodeTestWidget(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::gNodeTestWidget)
{
    ui->setupUi(this);
}

gNodeTestWidget::~gNodeTestWidget()
{
    delete ui;
}
