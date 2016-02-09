#ifndef GJARVISGUI_H
#define GJARVISGUI_H

#include <QMainWindow>

#include "sjarvis.h"


namespace Ui {
class gJarvisgui;
}

class gJarvisgui : public QMainWindow
{
    Q_OBJECT

public:
    explicit gJarvisgui(QWidget *parent = 0);
    ~gJarvisgui();

private:
    Ui::gJarvisgui *ui;
    sJarvis m_jarvis;

};

#endif // GJARVISGUI_H
