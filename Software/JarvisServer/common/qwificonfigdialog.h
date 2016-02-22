#ifndef QWIFICONFIGDIALOG_H
#define QWIFICONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class qWifiConfigDialog;
}

class qWifiConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit qWifiConfigDialog(QWidget *parent = 0);
    ~qWifiConfigDialog();

    QString essid();
    QString passwd();
    bool apMode();

private:
    Ui::qWifiConfigDialog *ui;
};

#endif // QWIFICONFIGDIALOG_H
