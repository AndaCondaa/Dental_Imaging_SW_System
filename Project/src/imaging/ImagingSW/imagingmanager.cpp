#include "imagingmanager.h"
#include "ui_imagingmanager.h"

#include <QFileDialog>

ImagingManager::ImagingManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImagingManager)
{
    ui->setupUi(this);
}

ImagingManager::~ImagingManager()
{
    delete ui;
}

void ImagingManager::on_reconCancelButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(0,
                                                    "Open a file", QDir::homePath(),
                                                    "image file(*.raw *.pbm *.pgm *.ppm *.bmp *.jpg *.png)");

    QFile file(fileName);
    file.open(QFile::ReadOnly);
    QByteArray byteArray = file.readAll();
    file.close();

    int width = widthLineEdit->text().toInt();
    int height = heightLineEdit->text().toInt();
    uchar *data = (uchar*)(byteArray.data());

    QImage image = QImage(data, width, height, QImage::Format_Grayscale8);
    QFileInfo fileInfo(fileName);
    if(fileInfo.suffix() == "raw")
        imageLabel->setPixmap(QPixmap::fromImage(image, Qt::AutoColor));
    else
        imageLabel->setPixmap(QPixmap(fileName).scaled(width, height));
    w.setWindowTitle(fileName);
}



