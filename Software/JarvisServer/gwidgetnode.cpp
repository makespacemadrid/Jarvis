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
        m_node = new sJarvisNode(this);
        m_sharedNode = false;
    }
    connectNodeSignals(m_node);
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
    m_node = new sJarvisNode(this);
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
