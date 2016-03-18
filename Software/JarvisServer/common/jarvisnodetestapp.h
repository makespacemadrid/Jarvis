#ifndef JARVISNODETESTAPP_H
#define JARVISNODETESTAPP_H

#include <QMainWindow>
#include "gwidgettxrx.h"
#include <QLabel>
#include "sjarvisnode.h"
#include "gnodecomponentwidget.h"
#include "gsimplegraph.h"


namespace Ui {
class jarvisNodeTestApp;
}

class jarvisNodeTestApp : public QMainWindow
{
    Q_OBJECT

public:
    explicit jarvisNodeTestApp(sJarvisNode* node = 0, QWidget *parent = 0);
    ~jarvisNodeTestApp();
    sJarvisNode* newNode();
    void setNode(sJarvisNode* nNode);
    sJarvisNode* node() {return m_node;}

private:
    Ui::jarvisNodeTestApp *ui;
    gWidgetTxRx m_rxWidget;
    QLabel      m_statusLabel;
    QLabel      m_txRxLabel;


    sJarvisNode*         m_node;
    bool                 m_sharedNode;
    QTimer               m_sensorsTimer;
    bool                 m_graphInit;
    QVector<gSimpleGraph*>       m_graphs;
    QList<gNodeComponentWidget*> m_componentWidgets;

    void connectNodeSignals(sJarvisNode* node);

private slots:
    void nodeConnected();
    void nodeDisconnected();
    void updateTxRx();

public slots:
    void on_readComponentsBtn_clicked();
    void on_connectBtn_clicked();
    void on_sendCmdBtn_clicked();
    void on_stopReadBtn_clicked();
    void on_btnGetComps_clicked();
    void on_wifiConfigBtn_clicked();
    void on_btnPing_clicked();
    void on_btnClearGraphs_clicked();
    void on_saveConfigBtn_clicked();
    void on_clearEepromBtn_clicked();
    void on_editConfigBtn_clicked();

    void console_log(QByteArray data);
    void eventLog(QString component,jarvisEvents event,QStringList args);
    void sensorRead(QVector<QString> fields,QVector<double> data);

};

#endif // JARVISNODETESTAPP_H
