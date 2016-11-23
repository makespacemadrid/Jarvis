#include "gjarvisgui.h"
#include "ui_gjarvisgui.h"

gJarvisgui::gJarvisgui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::gJarvisgui)
{
    ui->setupUi(this);
    ui->nodeServerWidget->setNodeServer(m_jarvis.nodeServer());
    ui->connManagerWidget->setConnManager(m_jarvis.connectionManager());
    this->setStyleSheet("QGroupBox { border: 1px solid gray; border-radius: 9px; margin-top: 0.5em; } QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 3px 0 3px;}");
}

gJarvisgui::~gJarvisgui()
{
    delete ui;
}
