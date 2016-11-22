#include "gjarvisconnectionwidget.h"
#include "ui_gjarvisconnectionwidget.h"

#include "gruleeditorwidget.h"

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
    connect(ui->btnRemove,SIGNAL(clicked(bool)),this,SLOT(deleteLater()));
}

void gJarvisConnectionWidget::setJarvisConnection(sJarvisConnection *conn, bool sharedConn)
{
    if(!m_sharedConn && m_conn)
        m_conn->deleteLater();
    m_sharedConn = sharedConn;
    m_conn = conn;

    ui->idLabel->setText(m_conn->id());
    connect(m_conn,SIGNAL(activated()),ui->lcdNumber,SLOT(blink()));
    connect(m_conn,SIGNAL(updated()),this,SLOT(reload()));
}


gJarvisConnectionWidget::~gJarvisConnectionWidget()
{
    delete ui;
    if(!m_sharedConn)
        m_conn->deleteLater();
}


void gJarvisConnectionWidget::reload()
{
    ui->idLabel->setText(m_conn->id());
}


void gJarvisConnectionWidget::on_editBtn_clicked()
{
    emit(configureme(this->m_conn));
}
