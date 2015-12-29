#ifndef GWIDGETNODE_H
#define GWIDGETNODE_H

#include <QFrame>
#include "sjarvisnode.h"

namespace Ui {
class gWidgetNode;
}

class gWidgetNode : public QFrame
{
    Q_OBJECT

public:
    explicit gWidgetNode(sJarvisNode* node = 0,QWidget* parent = 0);
    ~gWidgetNode();
    sJarvisNode* newNode();
    void setNode(sJarvisNode* nNode);

    sJarvisNode* node() {return m_node;}

private:
    Ui::gWidgetNode *ui;
    sJarvisNode*    m_node;
    bool            m_sharedNode;

    void connectNodeSignals(sJarvisNode* node);
};

#endif // GWIDGETNODE_H
