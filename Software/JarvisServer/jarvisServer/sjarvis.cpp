#include "sjarvis.h"

sJarvis::sJarvis(QObject *parent) :
    QObject(parent), m_connectionManager(&m_nodeServer)
{
        m_nodeServer.listen(QHostAddress::Any,31416);

        sJarvisConnection* conn = m_connectionManager.addConnection();
        conn->addSenderEvent("MakeSwitch","switch",E_ACTIVATED);
        conn->addDestAction("ledPanel","ledPanel",A_ACTIVATE);
        conn->setId("Global On");

        sJarvisConnection* conn1 = m_connectionManager.addConnection();
        conn1->addSenderEvent("MakeSwitch","switch",E_DEACTIVATED );
        conn1->addDestAction("ledPanel","ledPanel",A_DEACTIVATE);
        conn1->setId("Global Off");
}
