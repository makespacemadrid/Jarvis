#ifndef SJARVIS_H
#define SJARVIS_H

#include <QObject>
#include "sjarvisnodeserver.h"

class sJarvis : public QObject
{
    Q_OBJECT
public:
    explicit sJarvis(QObject *parent = 0);
    sJarvisNodeServer* nodeServer() {return &m_nodeServer;}

private:
    sJarvisNodeServer m_nodeServer;
signals:

public slots:

};

#endif // SJARVIS_H
