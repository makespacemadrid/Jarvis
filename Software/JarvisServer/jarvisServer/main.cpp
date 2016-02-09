
#include <QApplication>
#include <QDebug>

#include "arduinoCompat.h"
#include "gjarvisgui.h"
#include "gnodetestwidget.h"
#include "qimageselectionwidget.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << "[INFO]main: Starting Jarvis with gui..." ;
    gJarvisgui w;
    //gNodeTestWidget w;
    //qImageSelectionWidget w;
    w.show();

    return a.exec();
}
