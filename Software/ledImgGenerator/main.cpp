
#include <QApplication>

#include "qimageselectionwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qImageSelectionWidget w;
    w.show();

    return a.exec();
}
