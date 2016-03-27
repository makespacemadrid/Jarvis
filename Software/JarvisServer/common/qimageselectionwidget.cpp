#include "qimageselectionwidget.h"
#include "ui_qimageselectionwidget.h"

#include <QFileDialog>
#include <QDebug>

qImageSelectionWidget::qImageSelectionWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qImageSelectionWidget)
{
    ui->setupUi(this);
    connect(ui->widthSpin,SIGNAL(valueChanged(int)),this,SLOT(resizeImg()));
    connect(ui->heightSpin,SIGNAL(valueChanged(int)),this,SLOT(resizeImg()));
    connect(ui->aspectRatioCheck,SIGNAL(clicked(bool)),this,SLOT(resizeImg()));
    connect(ui->scaleCheckbox,SIGNAL(clicked(bool)),this,SLOT(resizeImg()));
}


qImageSelectionWidget::~qImageSelectionWidget()
{
    delete ui;
}

void qImageSelectionWidget::on_selectFileBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("image (*.*)"));
    ui->fileLabel->setText(fileName);
    image.load(fileName);
    if((image.width() > 256) || (image.height() > 256))
    {
        image = image.scaled(256,256,Qt::KeepAspectRatio);
        ui->origView->setPixmap(QPixmap::fromImage(image, Qt::AutoColor));
    }
    else
    {
        ui->origView->setPixmap(QPixmap::fromImage(image.scaled(256,256,Qt::KeepAspectRatio), Qt::AutoColor));
    }


    resizeImg();
}

void qImageSelectionWidget::resizeImg()
{
    if(image.isNull()) return;
    if(ui->scaleCheckbox->isChecked())
    {
        if(ui->aspectRatioCheck)
            scaledImage = image.scaled(ui->widthSpin->value(),ui->heightSpin->value(),Qt::KeepAspectRatio);
        else
            scaledImage = image.scaled(ui->widthSpin->value(),ui->heightSpin->value());
    }
    else
        scaledImage = image;

    ui->scaledView->setPixmap(QPixmap::fromImage(scaledImage.scaled(256,256,Qt::KeepAspectRatio), Qt::AutoColor));

    QImage img = scaledImage.convertToFormat(QImage::Format_RGB888);
    uchar *bits = img.bits();
    QString args;
    args.append("static constexpr uint8_t icon[] = {");
    int count = (img.width() * img.height() * 3);
    for (int i = 0 ; i < count-1 ; i++)
    {
        args.append(QString::number((int) bits[i],'f',0));
        args.append(",");
    }
    args.append(QString::number((int) bits[count-1],'f',0));
    args.append("}");
    ui->dataArrayEdit->clear();
    ui->dataArrayEdit->appendPlainText(args);
}
