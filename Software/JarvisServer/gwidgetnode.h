#ifndef GWIDGETNODE_H
#define GWIDGETNODE_H

#include <QFrame>

namespace Ui {
class gWidgetNode;
}

class gWidgetNode : public QFrame
{
    Q_OBJECT

public:
    explicit gWidgetNode(QWidget *parent = 0);
    ~gWidgetNode();

private:
    Ui::gWidgetNode *ui;
};

#endif // GWIDGETNODE_H
