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

void qImageSelectionWidget::on_btnSave_clicked()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    QString strFile = dialog.getSaveFileName(NULL, "Create New File","","");
    if(strFile.isEmpty()) return;

    QImage img = scaledImage.convertToFormat(QImage::Format_RGB888);
    QFile f(strFile);

    if(!f.open(QIODevice::WriteOnly))
    {
        qDebug() << "Cannot open for write" << strFile ;
        return;
    }

    QByteArray data;
    data.append(img.height());
    data.append(img.width());

    for(int y = 0 ; y < img.height() ; y++)
    {
        for(int x = 0 ; x < img.width() ; x++)
        {
            QRgb colorRgb = img.pixel(QPoint(x,y));
            QColor color(colorRgb);
            //qDebug() << "x:" << x << " - y:" << y << " - r:" << color.red() << " - g:" << color.green() << " - b:" << color.blue();
            data.append(color.red());
            data.append(color.green());
            data.append(color.blue());
        }
    }
    f.write(data);
    qDebug() << "Written" << f.size() << " bytes" ;
    f.close();
}
