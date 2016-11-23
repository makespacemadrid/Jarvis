#include "sjarvis.h"
#include <QFile>

sJarvis::sJarvis(QObject *parent) :
    QObject(parent), m_connectionManager(&m_nodeServer)
{
        m_nodeServer.listen(QHostAddress::Any,31416);
}
