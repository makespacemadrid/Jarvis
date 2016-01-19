#include "sjarvistcpclient.h"

sJarvisTcpClient::sJarvisTcpClient(QObject *parent) :
    QTcpSocket(parent)
{
  m_tx_count = 0;
  m_rx_count = 0;

  connect(this,SIGNAL(readyRead()),
          this, SLOT(readSocket()));

//  connect(this,SIGNAL(disconnected()),
//          this,SLOT(deleteLater()));

  //m_keepAliveT = new QTimer();
  //connect(m_keepAliveT,SIGNAL(timeout()),this,SLOT(keep_alive()));
  m_uptime = QDateTime::currentDateTime();
  //m_keepAliveT->start(m_keepAliveRate);
}

sJarvisTcpClient::~sJarvisTcpClient()
{
  close();
  //delete m_keepAliveT;
}

QString sJarvisTcpClient::info()
{
    QString srt("CLIENT-INFO:\n\n");
    srt.append("\tconnected:\t").append(QString::number(m_uptime.toTime_t()));
    srt.append("\n\tuptime :\t").append(QString::number(m_uptime.secsTo(QDateTime::currentDateTime())));
    srt.append("\n\tlast ping:\t").append(QString::number(m_lastPing)).append("ms");
    srt.append("\n\ttx packets:\t").append(QString::number(m_tx_count));
    srt.append("\n\trx packets:\t").append(QString::number(m_rx_count));
    return srt;
}

// private slots
void sJarvisTcpClient::readSocket()
{
  emit rx();
  m_rx_count++;
  QByteArray data = this->readAll();
  emit socket_rx(data);
}

void sJarvisTcpClient::socket_tx(const QByteArray &data)
{
  emit tx();
  m_tx_count++;
  this->write(data);
}

void sJarvisTcpClient::closeSocket()
{
  this->close();
}

/*
void sJarvisTcpClient::pong()
{
    quint16 elapsed = m_pingMeter.elapsed();
    qDebug() << "[INFO]sJarvisTcpClient::pong()-> Ping time:" << elapsed << "ms";
    emit  ping_time(elapsed);
}

void sJarvisTcpClient::keep_alive()
{
    m_pingMeter.start();
}
*/
