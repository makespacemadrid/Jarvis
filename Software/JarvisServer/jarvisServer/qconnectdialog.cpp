#include "qconnectdialog.h"
#include "ui_qconnectdialog.h"

qConnectDialog::qConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qConnectDialog)
{
    ui->setupUi(this);
}

qConnectDialog::~qConnectDialog()
{
    delete ui;
}

QString qConnectDialog::ip()
{
    return ui->ipEdit->text();
}

quint16 qConnectDialog::port()
{
    return ui->portEdit->text().toInt();
}

bool qConnectDialog::isSerialConnection()
{
    return ui->serialTab->isActiveWindow();
}
