#include "gjarvisgui.h"
#include "ui_gjarvisgui.h"

gJarvisgui::gJarvisgui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::gJarvisgui)
{
    ui->setupUi(this);
}

gJarvisgui::~gJarvisgui()
{
    delete ui;
}
