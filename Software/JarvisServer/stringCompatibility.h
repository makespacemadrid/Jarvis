#ifndef STRINGCOMPAT_H
#define STRINGCOMPAT_H

#include <QObject>
#include <QString>

class String : public QString
{
    Q_OBJECT
public:
    explicit String(QObject *parent = 0);
    String substring(int start,int end) {return this->section(start,end =-1);}

signals:

public slots:

};

#endif // STRINGCOMPAT_H
