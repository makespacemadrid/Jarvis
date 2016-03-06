#include "gjarvisconnectionwidget.h"
#include "ui_gjarvisconnectionwidget.h"

gJarvisConnectionWidget::gJarvisConnectionWidget(sJarvisConnection * conn, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::gJarvisConnectionWidget)
{
    m_conn = 0;
    ui->setupUi(this);
    if(conn)
        setJarvisConnection(conn,true);
    else
        setJarvisConnection(new sJarvisConnection(),false);
}

void gJarvisConnectionWidget::setJarvisConnection(sJarvisConnection *conn, bool sharedConn)
{
    if(!m_sharedConn && m_conn)
        m_conn->deleteLater();
    m_sharedConn = sharedConn;
    m_conn = conn;

    ui->idLabel->setText(m_conn->id());
    connect(m_conn,SIGNAL(activated()),ui->lcdNumber,SLOT(blink(QColor)));
}


gJarvisConnectionWidget::~gJarvisConnectionWidget()
{
    delete ui;
    if(!m_sharedConn)
        m_conn->deleteLater();
}
