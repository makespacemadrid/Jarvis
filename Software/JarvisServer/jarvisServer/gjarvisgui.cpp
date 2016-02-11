#include "gjarvisgui.h"
#include "ui_gjarvisgui.h"

gJarvisgui::gJarvisgui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::gJarvisgui)
{
    ui->setupUi(this);
    ui->nodeServerWidget->setNodeServer(m_jarvis.nodeServer());
}

gJarvisgui::~gJarvisgui()
{
    delete ui;
}
