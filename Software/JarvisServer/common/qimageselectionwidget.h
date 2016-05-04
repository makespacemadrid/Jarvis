#ifndef QIMAGESELECTIONWIDGET_H
#define QIMAGESELECTIONWIDGET_H

#include <QDialog>

namespace Ui {
class qImageSelectionWidget;
}

class qImageSelectionWidget : public QDialog
{
    Q_OBJECT

public:
    explicit qImageSelectionWidget(QWidget *parent = 0);
    ~qImageSelectionWidget();
    QImage image;
    QImage scaledImage;

private:
    Ui::qImageSelectionWidget *ui;

signals:

private slots:
    void on_selectFileBtn_clicked();
    void on_btnSave_clicked();
    void on_selectDataFileBtn_clicked();
    void resizeImg();
};

#endif // QIMAGESELECTIONWIDGET_H
