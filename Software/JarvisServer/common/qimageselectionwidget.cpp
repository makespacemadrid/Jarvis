#include "qimageselectionwidget.h"
#include "ui_qimageselectionwidget.h"

#include <QFileDialog>
#include <QDebug>
#include <QPainter>

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
    quint16 rows = img.height();
    quint16 cols = img.width();

    char* r = (char*) &rows;
    char* c = (char*) &cols;

    data.append(r[1]);
    data.append(r[0]);
    data.append(c[1]);
    data.append(c[0]);

    qDebug() << "header written:" << data.size() << "bytes";
    qDebug() << data;
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

void qImageSelectionWidget::on_selectDataFileBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Data File"),"",tr("any (*)"));
    if(fileName.isEmpty()) return;
    quint16 rows,cols;
    QFile f(fileName);
    if(!f.open(QIODevice::ReadOnly))
    {
        qDebug() << "Cannot open:" << fileName;
        return;
    }
    char a,b,c,d;
    f.read(&a,1);
    f.read(&b,1);
    f.read(&c,1);
    f.read(&d,1);

    rows = (b<< 8) | a;
    cols = (d<< 8) | c;
    quint8 headerSize = sizeof(rows) + sizeof(cols);


    quint16 expectedMatrixSize = (rows*cols*3)+headerSize;

    if(f.size() != expectedMatrixSize)
    {
        qDebug() << "Wrong size:" << f.size() << "/" << expectedMatrixSize;
        return;
    }

    QImage img;
    QPainter p;
    QPixmap pmap;
//    pmap.loadFromData()
    p.begin(&pmap);

    for(int row = 0 ; row < rows ; row++)
    {
        for(int col = 0 ; col < cols ; col++)
        {
            quint16 offset = (( (row*cols) + col) * 3) + headerSize;
            f.seek(offset);
            quint8 r,g,b;
            f.read((char*)&r,1);
            f.read((char*)&g,1);
            f.read((char*)&b,1);
            p.setPen(QPen(QColor(r,g,b)));
            p.drawPoint(QPoint(col,row));
        }
    }
    p.end();
    ui->scaledView->setPixmap(QPixmap::fromImage(img));
}
