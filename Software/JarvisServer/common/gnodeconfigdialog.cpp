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
    ui->comboNodeType->setCurrentIndex(m_nodeConfig.moduleType);
    ui->spinUpdateInterval->setValue(m_nodeConfig.updateInterval);

    ui->editWifiEsssid->setText (QString(m_nodeConfig.wifiESSID));
    ui->editWifiPass->setText(QString(m_nodeConfig.wifiPasswd));
    ui->checkApMode->setChecked(m_nodeConfig.wifiAPMode);
    ui->editRemoteAddr->setText(QString(m_nodeConfig.remoteHost));
    ui->editRemotePort->setText(QString::number(m_nodeConfig.remotePort));
    ui->editLocalPort->setText(QString::number(m_nodeConfig.localPort));

    ui->spinLedNumber->setValue(m_nodeConfig.ledCount);
    ui->spinLedBright->setValue(m_nodeConfig.ledStripBrightness);
    ui->spinMatrixFirst->setValue(m_nodeConfig.ledMatrixFirstLed);
    ui->spinMatrixCols->setValue(m_nodeConfig.ledMatrixCols);
    ui->spinMatrixRows->setValue(m_nodeConfig.ledMatrixRows);
    ui->checkMatrixMirror->setChecked(m_nodeConfig.ledMatrixMirror);
    ui->checkMatrixZigZag->setChecked(m_nodeConfig.ledMatrixZigZag);

    ui->spinLedStripPin->setValue(m_nodeConfig.ledStripPin);
    ui->spinPiezoPin->setValue(m_nodeConfig.piezoPin);
    ui->spinAlivePin->setValue(m_nodeConfig.alivePin);

    ui->spinButtonPin_0->setValue(m_nodeConfig.buttonPins[0]);
    ui->spinButtonPin_1->setValue(m_nodeConfig.buttonPins[1]);
    ui->spinButtonPin_2->setValue(m_nodeConfig.buttonPins[2]);
    ui->spinButtonPin_3->setValue(m_nodeConfig.buttonPins[3]);
    ui->spinButtonPin_4->setValue(m_nodeConfig.buttonPins[4]);
    ui->spinButtonPin_5->setValue(m_nodeConfig.buttonPins[5]);
    ui->spinButtonPin_6->setValue(m_nodeConfig.buttonPins[6]);
    ui->spinButtonPin_7->setValue(m_nodeConfig.buttonPins[7]);
    ui->spinButtonPin_8->setValue(m_nodeConfig.buttonPins[8]);
    ui->spinButtonPin_9->setValue(m_nodeConfig.buttonPins[9]);

    ui->spinTempPins_0->setValue(m_nodeConfig.tempSensorPins[0]);
    ui->spinTempPins_1->setValue(m_nodeConfig.tempSensorPins[1]);
    ui->spinTempPins_2->setValue(m_nodeConfig.tempSensorPins[2]);
    ui->spinTempPins_3->setValue(m_nodeConfig.tempSensorPins[3]);
    ui->spinTempPins_4->setValue(m_nodeConfig.tempSensorPins[4]);
    ui->spinTempPins_5->setValue(m_nodeConfig.tempSensorPins[5]);
    ui->spinTempPins_6->setValue(m_nodeConfig.tempSensorPins[6]);
    ui->spinTempPins_7->setValue(m_nodeConfig.tempSensorPins[7]);
    ui->spinTempPins_8->setValue(m_nodeConfig.tempSensorPins[8]);
    ui->spinTempPins_9->setValue(m_nodeConfig.tempSensorPins[9]);

    ui->spinRelayPins_0->setValue(m_nodeConfig.relaypins[0]);
    ui->spinRelayPins_1->setValue(m_nodeConfig.relaypins[1]);
    ui->spinRelayPins_2->setValue(m_nodeConfig.relaypins[2]);
    ui->spinRelayPins_3->setValue(m_nodeConfig.relaypins[3]);
    ui->spinRelayPins_4->setValue(m_nodeConfig.relaypins[4]);
    ui->spinRelayPins_5->setValue(m_nodeConfig.relaypins[5]);
    ui->spinRelayPins_6->setValue(m_nodeConfig.relaypins[6]);
    ui->spinRelayPins_7->setValue(m_nodeConfig.relaypins[7]);
    ui->spinRelayPins_8->setValue(m_nodeConfig.relaypins[8]);
    ui->spinRelayPins_9->setValue(m_nodeConfig.relaypins[9]);
}

settingList gNodeConfigDialog::getSettings()
{
    settingList s;
    for(int i = 0 ; i < sizeof(s.id) ; i++)
        s.id[i] = 0;
    for(int i = 0 ; i < sizeof(s.wifiESSID) ; i++)
        s.wifiESSID[i] = 0;
    for(int i = 0 ; i < sizeof(s.wifiPasswd) ; i++)
        s.wifiPasswd[i] = 0;
    for(int i = 0 ; i < sizeof(s.remoteHost) ; i++)
        s.remoteHost[i] = 0;

    for(uint i = 0 ; (i < sizeof(s.id)) && (i < ui->editID->text().trimmed().length()) ; i++)
    {
        s.id[i] = ui->editID->text().trimmed().toLatin1().data()[i];
    }
    s.moduleType     =  (jarvisModules)ui->comboNodeType->currentIndex();
    s.updateInterval = ui->spinUpdateInterval->value();


    for(uint i = 0 ; (i < sizeof(s.wifiESSID)) && (i < ui->editWifiEsssid->text().trimmed().length()) ; i++)
    {
        s.wifiESSID[i] = ui->editWifiEsssid->text().trimmed().toLatin1().data()[i];
    }
    for(uint i = 0 ; (i < sizeof(s.wifiPasswd)) && (i < ui->editWifiPass->text().trimmed().length()) ; i++)
    {
        s.wifiPasswd[i] = ui->editWifiPass->text().trimmed().toLatin1().data()[i];
    }
    for(uint i = 0 ; (i < sizeof(s.remoteHost )) && (i < ui->editRemoteAddr->text().trimmed().length()) ; i++)
    {
        s.remoteHost[i] = ui->editRemoteAddr->text().trimmed().toLatin1().data()[i];
    }

    s.wifiAPMode    = ui->checkApMode->isChecked();
    s.remotePort    = ui->editRemotePort->text().toInt();
    s.localPort     = ui->editLocalPort->text().toInt();


    s.ledCount              = ui->spinLedNumber->value();
    s.ledStripBrightness    = ui->spinLedBright->value();
    s.ledMatrixFirstLed     = ui->spinMatrixFirst->value();
    s.ledMatrixCols         = ui->spinMatrixCols->value();
    s.ledMatrixRows         = ui->spinMatrixRows->value();
    s.ledMatrixMirror       = ui->checkMatrixMirror->isChecked();
    s.ledMatrixZigZag       = ui->checkMatrixZigZag->isChecked();



    s.ledStripPin       = ui->spinLedStripPin->value();
    s.piezoPin          = ui->spinPiezoPin->value();
    s.alivePin          = ui->spinAlivePin->value();

    s.buttonPins[0] = ui->spinButtonPin_0->value();
    s.buttonPins[1] = ui->spinButtonPin_1->value();
    s.buttonPins[2] = ui->spinButtonPin_2->value();
    s.buttonPins[3] = ui->spinButtonPin_3->value();
    s.buttonPins[4] = ui->spinButtonPin_4->value();
    s.buttonPins[5] = ui->spinButtonPin_5->value();
    s.buttonPins[6] = ui->spinButtonPin_6->value();
    s.buttonPins[7] = ui->spinButtonPin_7->value();
    s.buttonPins[8] = ui->spinButtonPin_8->value();
    s.buttonPins[9] = ui->spinButtonPin_9->value();

    s.tempSensorPins[0] = ui->spinTempPins_0->value();
    s.tempSensorPins[1] = ui->spinTempPins_1->value();
    s.tempSensorPins[2] = ui->spinTempPins_2->value();
    s.tempSensorPins[3] = ui->spinTempPins_3->value();
    s.tempSensorPins[4] = ui->spinTempPins_4->value();
    s.tempSensorPins[5] = ui->spinTempPins_5->value();
    s.tempSensorPins[6] = ui->spinTempPins_6->value();
    s.tempSensorPins[7] = ui->spinTempPins_7->value();
    s.tempSensorPins[8] = ui->spinTempPins_8->value();
    s.tempSensorPins[9] = ui->spinTempPins_9->value();

    s.relaypins[0] = ui->spinRelayPins_0->value();
    s.relaypins[1] = ui->spinRelayPins_1->value();
    s.relaypins[2] = ui->spinRelayPins_2->value();
    s.relaypins[3] = ui->spinRelayPins_3->value();
    s.relaypins[4] = ui->spinRelayPins_4->value();
    s.relaypins[5] = ui->spinRelayPins_5->value();
    s.relaypins[6] = ui->spinRelayPins_6->value();
    s.relaypins[7] = ui->spinRelayPins_7->value();
    s.relaypins[8] = ui->spinRelayPins_8->value();
    s.relaypins[9] = ui->spinRelayPins_9->value();

    return s;
}



gNodeConfigDialog::~gNodeConfigDialog()
{
    delete ui;
}
