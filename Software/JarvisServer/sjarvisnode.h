#ifndef SJARVISNODE_H
#define SJARVISNODE_H

#include "sjarvistcpclient.h"
#include "jarvisParser.h"

class sJarvisNode : public QObject, jarvisParser
{
    Q_OBJECT
public:
    sJarvisNode();
};

#endif // SJARVISNODE_H
