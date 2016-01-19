#include "arduinoCompat.h"

#include <QApplication>

#include "jarvisnodetestapp.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    jarvisNodeTestApp w;
    w.show();

    return a.exec();
}
