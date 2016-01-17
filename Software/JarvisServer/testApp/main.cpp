#include "arduinoCompat.h"

#include "gnodetestwidget.h"
#include "qimageselectionwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    gNodeTestWidget w;
    w.show();

    return a.exec();
}
