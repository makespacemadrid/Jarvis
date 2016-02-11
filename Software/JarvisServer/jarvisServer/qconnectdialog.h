#ifndef QCONNECTDIALOG_H
#define QCONNECTDIALOG_H

#include <QDialog>

namespace Ui {
class qConnectDialog;
}

class qConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit qConnectDialog(QWidget *parent = 0);
    ~qConnectDialog();
    QString ip();
    quint16 port();
    bool isSerialConnection();
private:
    Ui::qConnectDialog *ui;
};

#endif // QCONNECTDIALOG_H
