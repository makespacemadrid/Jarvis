#include "gwidgetnode.h"
#include "ui_gwidgetnode.h"

gWidgetNode::gWidgetNode(sJarvisNode* node,QWidget* parent) :
    QFrame(parent),
    ui(new Ui::gWidgetNode)
{
    ui->setupUi(this);
    if(node != 0)
    {
        m_sharedNode = true;
        m_node = node;
    } else {
        m_node = new sJarvisNode(0,this);
        m_sharedNode = false;
    }
    connectNodeSignals(m_node);
    this->setStyleSheet("QGroupBox { border: 1px solid gray; border-radius: 9px; margin-top: 0.5em; }");
    this->setStyleSheet("QGroupBox::title {subcontrol-origin: margin;left: 10px;padding: 0 3px 0 3px;}");
}

gWidgetNode::~gWidgetNode()
{
    delete ui;
    if(!m_sharedNode)
        m_node->deleteLater();
}

sJarvisNode* gWidgetNode::newNode()
{
    if(!m_sharedNode)
        m_node->deleteLater();
    m_node = new sJarvisNode(0,this);
    m_sharedNode = false;
    connectNodeSignals(m_node);
    return m_node;
}

void gWidgetNode::setNode(sJarvisNode *nNode)
{
    if(!m_sharedNode)
        m_node->deleteLater();
    m_node = nNode;
    m_sharedNode = true;
    connectNodeSignals(m_node);
}

void gWidgetNode::connectNodeSignals(sJarvisNode* node)
{
    connect(node,SIGNAL(tx()),ui->txwidget,SLOT(tx()));
    connect(node,SIGNAL(rx()),ui->txwidget,SLOT(rx()));
}
