#include "gwidgetnode.h"
#include "ui_gwidgetnode.h"

gWidgetNode::gWidgetNode(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::gWidgetNode)
{
    ui->setupUi(this);
}

gWidgetNode::~gWidgetNode()
{
    delete ui;
}
