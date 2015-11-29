#include "sjarvis.h"

sJarvis::sJarvis(QObject *parent) :
    QObject(parent)
{
        m_nodeServer.listen(QHostAddress::Any,31416);
}
