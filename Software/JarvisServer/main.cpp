#include "arduinoCompat.h"

#include "gjarvisgui.h"
#include "gnodetestwidget.h"
#include "qimageselectionwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //gJarvisgui w;
    gNodeTestWidget w;
    //qImageSelectionWidget w;
    w.show();

    return a.exec();
}
