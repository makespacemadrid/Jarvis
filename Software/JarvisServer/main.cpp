#include "stringCompatibility.h"

#include "gjarvisgui.h"
#include "gnodetestwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    //gJarvisgui w;
    gNodeTestWidget w;
    w.show();

    return a.exec();
}
