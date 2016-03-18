#include "gnodeconfigdialog.h"
#include "ui_gnodeconfigdialog.h"

gNodeConfigDialog::gNodeConfigDialog(settingList config, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::gNodeConfigDialog)
{
    ui->setupUi(this);
    m_nodeConfig = config;
    loadSettings();
}

void gNodeConfigDialog::loadSettings()
{
    ui->editID->setText(QString(m_nodeConfig.id));
    ui->editWifiEsssid->setText (QString(m_nodeConfig.wifiESSID));
    ui->editWifiPass->setText(QString(m_nodeConfig.wifiPasswd));
    ui->editRemoteAddr->setText(QString(m_nodeConfig.remoteHost));
    ui->editRemotePort->setText(QString(m_nodeConfig.remotePort));
    ui->editLocalPort->setText(QString(m_nodeConfig.localPort));

}

settingList gNodeConfigDialog::getSettings()
{
    settingList s;

}



gNodeConfigDialog::~gNodeConfigDialog()
{
    delete ui;
}
