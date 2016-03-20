#ifndef GJARVISCONNECTIONMANAGERWIDGET_H
#define GJARVISCONNECTIONMANAGERWIDGET_H

#include <QGroupBox>

namespace Ui {
class gJarvisConnectionManagerWidget;
}

class gJarvisConnectionManagerWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit gJarvisConnectionManagerWidget(QWidget *parent = 0);
    ~gJarvisConnectionManagerWidget();

private:
    Ui::gJarvisConnectionManagerWidget *ui;

protected slots:
    void on_addBtn_clicked();
};

#endif // GJARVISCONNECTIONMANAGERWIDGET_H
