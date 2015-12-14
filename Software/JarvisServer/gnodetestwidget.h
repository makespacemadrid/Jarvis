#ifndef GNODETESTWIDGET_H
#define GNODETESTWIDGET_H

#include <QGroupBox>
#include "sjarvisnode.h"
#include "gsimplegraph.h"

namespace Ui {
class gNodeTestWidget;
}

class gNodeTestWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit gNodeTestWidget(sJarvisNode* node = 0, QWidget *parent = 0);
    ~gNodeTestWidget();
    sJarvisNode* newNode();
    void setNode(sJarvisNode* nNode);
    sJarvisNode* node() {return m_node;}

private:
    Ui::gNodeTestWidget *ui;
    sJarvisNode*         m_node;
    bool                 m_sharedNode;
    QTimer               m_sensorsTimer;
    bool                 m_graphInit =false;
    QVector<gSimpleGraph*> m_graphs;
    void connectNodeSignals(sJarvisNode* node);
private slots:
    void addComponent(sJarvisNodeComponent* ncomp);

public slots:
    void on_connectBtn_clicked();
    void on_sendBtn_clicked();
    void console_log(QByteArray data);
    void eventLog(QString component,jarvisEvents event,QStringList args);
    void sensorRead(QVector<QString> fields,QVector<double> data);
};

#endif // GNODETESTWIDGET_H
