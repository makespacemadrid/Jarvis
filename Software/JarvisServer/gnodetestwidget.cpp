#include "gnodetestwidget.h"
#include "ui_gnodetestwidget.h"
#include "gnodecomponentwidget.h"
#include <QVBoxLayout>


gNodeTestWidget::gNodeTestWidget(sJarvisNode* node, QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::gNodeTestWidget)
{
    ui->setupUi(this);
    if(node != 0)
    {
        m_sharedNode = true;
        m_node = node;
    } else {
        m_node = new sJarvisNode(this);
        m_sharedNode = false;
    }
    connectNodeSignals(m_node);
    m_sensorsTimer.setInterval(500);
    ui->nodeComponents->setLayout(new QVBoxLayout());
    ui->nodeComponents->layout()->setSizeConstraint(QLayout::SetMinimumSize);
    ui->sensorBox->setLayout(new QVBoxLayout());
    ui->sensorBox->layout()->setSizeConstraint(QLayout::SetMinimumSize);
}

gNodeTestWidget::~gNodeTestWidget()
{
    delete ui;
}

void gNodeTestWidget::connectNodeSignals(sJarvisNode* node)
{
    connect(node,SIGNAL(tx()),ui->txwidget,SLOT(tx()));
    connect(node,SIGNAL(rx()),ui->txwidget,SLOT(rx()));
    connect(node,SIGNAL(rawInput(QByteArray)),this,SLOT(console_log(QByteArray)));
    connect(node,SIGNAL(writeData(QByteArray)),this,SLOT(console_log(QByteArray)));
    connect(node,SIGNAL(incomingEvent(QString,jarvisEvents,QStringList)),this,SLOT(eventLog(QString,jarvisEvents,QStringList)));
    connect(node,SIGNAL(newComponent(sJarvisNodeComponent*)),this,SLOT(addComponent(sJarvisNodeComponent*)));
    connect(&m_sensorsTimer,SIGNAL(timeout()),node,SLOT(readSensors()));
    connect(node,SIGNAL(sensorReads(QVector<QString>,QVector<double>)),this,SLOT(sensorRead(QVector<QString>,QVector<double>)));
}

sJarvisNode* gNodeTestWidget::newNode()
{
    if(!m_sharedNode)
        m_node->deleteLater();
    m_node = new sJarvisNode(this);
    m_sharedNode = false;
    connectNodeSignals(m_node);
    return m_node;
}

void gNodeTestWidget::setNode(sJarvisNode *nNode)
{
    if(!m_sharedNode)
        m_node->deleteLater();
    m_node = nNode;
    m_sharedNode = true;
    connectNodeSignals(m_node);
}

void gNodeTestWidget::addComponent(sJarvisNodeComponent *ncomp)
{
    gNodeComponentWidget* w = new gNodeComponentWidget(ncomp,ui->nodeComponents);
    ui->nodeComponents->layout()->addWidget(w);
    ui->idLabel->setText(m_node->getId());
}




void gNodeTestWidget::on_connectBtn_clicked()
{
    if(ui->connectBtn->isChecked())
    {
        int port = 31416;//ui->ipEdit->text().toInt();
        qDebug() << "Connecting-" << ui->ipEdit->text() << ":" << QString::number(port);
        m_node->connectTCP(ui->ipEdit->text(),port);
        m_sensorsTimer.start();
    }
    else
    {

    }
}

void gNodeTestWidget::on_sendBtn_clicked()
{
    m_node->send(ui->cmdEdit->text().toUtf8());
}

void gNodeTestWidget::console_log(QByteArray data)
{
    ui->consoleOutputEdit->append(data);
}

void gNodeTestWidget::eventLog(QString component, jarvisEvents event, QStringList args)
{
    QString result;
    result.append("Source:");
    result.append(component);
    result.append("Event:");
    result.append(QString::number(event,'f',0));
    for(int i = 0 ; i < args.count() ; i++ )
    {
        result.append(args[i]);
    }
    ui->eventLogEdit->append(result);
}

void gNodeTestWidget::sensorRead(QVector<QString> fields,QVector<double> data)
{
    if(!m_graphInit)
    {
        for(int i = 0; i < fields.count() ; i++ )
        {
            gSimpleGraph* g = new gSimpleGraph(ui->sensorBox);
            ui->sensorBox->layout()->addWidget(g);
            ui->sensorBox->layout()->setSizeConstraint(QLayout::SetMinimumSize);
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
