#include "qwificonfigdialog.h"
#include "ui_qwificonfigdialog.h"

qWifiConfigDialog::qWifiConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qWifiConfigDialog)
{
    ui->setupUi(this);
}

qWifiConfigDialog::~qWifiConfigDialog()
{
    delete ui;
}


QString qWifiConfigDialog::essid()
{
    return ui->essidEdit->text();
}

QString qWifiConfigDialog::passwd()
{
    return ui->pwdEdit->text();
}

bool qWifiConfigDialog::apMode()
{
    return ui->apRadio->isChecked();
}
