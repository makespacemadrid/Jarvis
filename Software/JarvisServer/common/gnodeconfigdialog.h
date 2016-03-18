#ifndef GNODECONFIGDIALOG_H
#define GNODECONFIGDIALOG_H

#include <QDialog>
#include <sjarvisnode.h>

namespace Ui {
class gNodeConfigDialog;
}

class gNodeConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit gNodeConfigDialog(settingList config = settingList(), QWidget *parent = 0);
    ~gNodeConfigDialog();
    void loadSettings();
    settingList getSettings();


private:
    Ui::gNodeConfigDialog *ui;
    settingList m_nodeConfig;
};

#endif // GNODECONFIGDIALOG_H
