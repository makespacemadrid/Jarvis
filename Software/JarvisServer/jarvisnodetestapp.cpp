#include "jarvisnodetestapp.h"
#include "ui_jarvisnodetestapp.h"

jarvisNodeTestApp::jarvisNodeTestApp(sJarvisNode* node, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::jarvisNodeTestApp)
{
    ui->setupUi(this);
    ui->statusbar->addWidget(&m_rxWidget);
    m_statusLabel.setText("Idle");
    ui->statusbar->addWidget(&m_statusLabel);
    if(node != 0)
    {
        m_sharedNode = true;
        m_node = node;
    } else {
        m_node = new sJarvisNode(this);
        m_sharedNode = false;
    }
    m_graphInit =false;
    connectNodeSignals(m_node);
    m_sensorsTimer.setInterval(500);
}

jarvisNodeTestApp::~jarvisNodeTestApp()
{
    delete ui;
}


void jarvisNodeTestApp::connectNodeSignals(sJarvisNode* node)
{
    connect(node,SIGNAL(tx()),&m_rxWidget,SLOT(tx()));
    connect(node,SIGNAL(rx()),&m_rxWidget,SLOT(rx()));
    connect(node,SIGNAL(rawInput(QByteArray)),this,SLOT(console_log(QByteArray)));
    connect(node,SIGNAL(writeData(QByteArray)),this,SLOT(console_log(QByteArray)));
    connect(node,SIGNAL(incomingEvent(QString,jarvisEvents,QStringList)),this,SLOT(eventLog(QString,jarvisEvents,QStringList)));
    //connect(node,SIGNAL(newComponent(sJarvisNodeComponent*)),this,SLOT(addComponent(sJarvisNodeComponent*)));
    //connect(&m_sensorsTimer,SIGNAL(timeout()),this,SLOT(timedCmd()));
    connect(node,SIGNAL(sensorReads(QVector<QString>,QVector<double>)),this,SLOT(sensorRead(QVector<QString>,QVector<double>)));
    connect(node,SIGNAL(ready()),this,SLOT(nodeConnected()));
    connect(node,SIGNAL(disconnected()),this,SLOT(nodeDisconnected()));
    connect(ui->sliderUpdateInterval,SIGNAL(sliderMoved(int)),m_node,SLOT(setUpdateInterval(int)));
}

sJarvisNode* jarvisNodeTestApp::newNode()
{
    if(!m_sharedNode)
        m_node->deleteLater();
    m_node = new sJarvisNode(this);
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
        ui->scrollComponentsContents->layout()->addWidget(w);
        m_componentWidgets.append(w);
    }
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
}

void jarvisNodeTestApp::on_connectBtn_clicked()
{
    if(ui->connectBtn->isChecked())
    {
        int port = ui->editPort->text().toInt();
        qDebug() << "Connecting-" << ui->editIP->text() << ":" << QString::number(port);
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
            d.append(0);
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
