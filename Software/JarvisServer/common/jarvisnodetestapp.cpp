#include "jarvisnodetestapp.h"
#include "ui_jarvisnodetestapp.h"

#include "qwificonfigdialog.h"
#include  "gnodeconfigdialog.h"

jarvisNodeTestApp::jarvisNodeTestApp(sJarvisNode* node, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::jarvisNodeTestApp)
{
    ui->setupUi(this);
    ui->statusbar->addWidget(&m_rxWidget);
    ui->statusbar->addWidget(&m_txRxLabel);
    m_statusLabel.setText("Idle");
    ui->statusbar->addWidget(&m_statusLabel);
    if(node != 0)
    {
        m_sharedNode = true;
        m_node = node;
        ui->groupConnect->setVisible(false);
        nodeConnected();
    } else {
        m_node = new sJarvisNode(0,this);
        m_sharedNode = false;
    }
    m_graphInit =false;
    connectNodeSignals(m_node);
    m_sensorsTimer.setInterval(500);
    this->setStyleSheet("QGroupBox { border: 1px solid gray; border-radius: 9px; margin-top: 0.5em; } QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 3px 0 3px;}");
}

jarvisNodeTestApp::~jarvisNodeTestApp()
{
    qDebug() << "Deleting jarvisNodeTestApp instance";
    if(!m_sharedNode)
        m_node->deleteLater();
    delete ui;
}


void jarvisNodeTestApp::connectNodeSignals(sJarvisNode* node)
{
    connect(node,SIGNAL(tx()),&m_rxWidget,SLOT(tx()));
    connect(node,SIGNAL(rx()),&m_rxWidget,SLOT(rx()));
    connect(node,SIGNAL(tx()),this,SLOT(updateTxRx()));
    connect(node,SIGNAL(rx()),this,SLOT(updateTxRx()));
    connect(node,SIGNAL(rawInput(QByteArray)),this,SLOT(console_log(QByteArray)));
    connect(node,SIGNAL(writeData(QByteArray)),this,SLOT(console_log(QByteArray)));
    connect(node,SIGNAL(incomingEvent(QString,jarvisEvents,QStringList)),this,SLOT(eventLog(QString,jarvisEvents,QStringList)));
    //connect(node,SIGNAL(newComponent(sJarvisNodeComponent*)),this,SLOT(addComponent(sJarvisNodeComponent*)));
    //connect(&m_sensorsTimer,SIGNAL(timeout()),this,SLOT(timedCmd()));
    connect(node,SIGNAL(sensorReads(QVector<QString>,QVector<double>)),this,SLOT(sensorRead(QVector<QString>,QVector<double>)));
    connect(node,SIGNAL(ready()),this,SLOT(nodeConnected()));
    connect(node,SIGNAL(disconnected()),this,SLOT(nodeDisconnected()));
    connect(ui->sliderUpdateInterval,SIGNAL(sliderMoved(int)),node,SLOT(setUpdateInterval(int)));
    connect(ui->btnReset,SIGNAL(clicked()),node,SLOT(resetNode()));
    connect(ui->reloadConfigBtn,SIGNAL(clicked()),node,SLOT(reloadNodeSettings()));
}

sJarvisNode* jarvisNodeTestApp::newNode()
{
    if(!m_sharedNode)
        m_node->deleteLater();
    m_node = new sJarvisNode(0,this);
    m_sharedNode = false;
    connectNodeSignals(m_node);
    return m_node;
}

void jarvisNodeTestApp::setNode(sJarvisNode *nNode)
{
    if(!m_sharedNode)
        m_node->deleteLater();
    m_node = nNode;
    m_sharedNode = true;
    connectNodeSignals(m_node);
}

void jarvisNodeTestApp::nodeConnected()
{
    QList<sJarvisNodeComponent*> comps = m_node->components();
    ui->labelID->setText(m_node->getId());
    for(int i = 0 ; i < comps.count() ; i++ )
    {
        sJarvisNodeComponent* ncomp = comps[i];
        gNodeComponentWidget* w = new gNodeComponentWidget(ncomp,ui->scrollComponents);
        QGridLayout* l = (QGridLayout*)ui->scrollComponentsContents->layout();
        l->addWidget(w);
        //l->addWidget(w,l->count()/2,l->count()%2);
        m_componentWidgets.append(w);
    }
    m_statusLabel.setText("Node connected!");        
}

void jarvisNodeTestApp::nodeDisconnected()
{
    for(int i = 0;  i < m_componentWidgets.count() ; i++)
    {
        m_componentWidgets[i]->deleteLater();
    }
    m_componentWidgets.clear();
    m_sensorsTimer.stop();
    ui->connectBtn->setChecked(false);
    m_statusLabel.setText("Node disconnected!");
}

void jarvisNodeTestApp::on_connectBtn_clicked()
{
    if(ui->connectBtn->isChecked())
    {
        int port = ui->editPort->text().toInt();
        QString status;
        status += "Connecting-" + ui->editIP->text() + ":" + QString::number(port);
        m_statusLabel.setText(status);
        m_node->connectTCP(ui->editIP->text(),port);
    }
    else
    {
        m_node->closeTCP();
    }
}

void jarvisNodeTestApp::on_readComponentsBtn_clicked()
{
    m_node->pollSensor();
}

void jarvisNodeTestApp::on_sendCmdBtn_clicked()
{
    m_node->send(ui->editCommand->text().toUtf8());
}

void jarvisNodeTestApp::on_wifiConfigBtn_clicked()
{
    qWifiConfigDialog w;
    if(w.exec())
    {
        m_node->setWifiConfig(w.essid(),w.passwd(),w.apMode());
    }
}


void jarvisNodeTestApp::on_stopReadBtn_clicked()
{
    m_node->stopPollingSensors();
}

void jarvisNodeTestApp::on_btnGetComps_clicked()
{
    for(int i = 0;  i < m_componentWidgets.count() ; i++)
    {
        m_componentWidgets[i]->deleteLater();
    }
    m_componentWidgets.clear();
    m_node->getComponents();
}

void jarvisNodeTestApp::on_btnPing_clicked()
{
    m_node->ping();
}

void jarvisNodeTestApp::on_btnClearGraphs_clicked()
{
    for(int i = 0 ; i < m_graphs.count() ; i++)
    {
        m_graphs[i]->deleteLater();
    }
    m_graphInit = false;
    m_graphs.clear();
}

void jarvisNodeTestApp::on_saveConfigBtn_clicked()
{
    m_node->saveEEPROM();
}

void jarvisNodeTestApp::on_clearEepromBtn_clicked()
{
    m_node->clearEEPROM();
}

void jarvisNodeTestApp::on_editConfigBtn_clicked()
{
    gNodeConfigDialog w(m_node->getNodeSettings());
    if(w.exec())
    {
        m_node->sendConfig(w.getSettings());
    }
}

void jarvisNodeTestApp::console_log(QByteArray data)
{
    ui->editConsole->appendPlainText(data);
}

void jarvisNodeTestApp::eventLog(QString component, jarvisEvents event, QStringList args)
{
    QString result;
    result.append("Source:");
    result.append(component);
    result.append(":Event:");
    result.append(QString::number(event,'f',0));
    for(int i = 0 ; i < args.count() ; i++ )
    {
        result.append(";");
        result.append(args[i]);
    }
    ui->editEventlog->append(result);
}


void jarvisNodeTestApp::sensorRead(QVector<QString> fields,QVector<double> data)
{      
    if(!m_graphInit)
    {
        for(int i = 0; i < fields.count() ; i++ )
        {
            gSimpleGraph* g = new gSimpleGraph(ui->scrollGraphs);
            ui->scrollGraphContents->layout()->addWidget(g);
            QVector<QString> f;
            QVector<double> d;
            f.append(fields[i]);
            g->setFields(f);
            g->appendData(d);
            m_graphs.append(g);
        }
        if(m_graphs.count()) m_graphInit = true;
    }
    for(int i = 0; i < m_graphs.count() ; i++ )
    {
        QVector<double> d;
        d.append(data[i]);
        m_graphs[i]->appendData(d);
    }
}

void jarvisNodeTestApp::updateTxRx()
{
    m_txRxLabel.clear();
    QString text;
    text.append("Tx:");
    text.append(QString::number(m_node->txCount()));
    text.append(" - Rx:");
    text.append(QString::number(m_node->rxCount()));
    text.append(" - Ping:");
    text.append(QString::number(m_node->pingTime()));
    m_txRxLabel.setText(text);
}
