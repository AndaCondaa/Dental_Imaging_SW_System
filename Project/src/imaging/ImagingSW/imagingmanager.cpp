#include "imagingmanager.h"
#include "ui_imagingmanager.h"

#include <QFileDialog>
#include <QLabel>

#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdio>


using namespace cv;
using namespace std;

typedef struct {
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    unsigned int biSize;
    unsigned int biWidth;
    unsigned int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int SizeImage;
    unsigned int biXPelsPerMeter;
    unsigned int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
} BITMAPINFOHEADER;

typedef struct {
    unsigned char rgbBlue;
    unsigned char rgbGreen;
    unsigned char rgbRed;
    unsigned char rgbReserved;
} RGBQUAD;


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
//    QString fileName = QFileDialog::getOpenFileName(0,
//                                                    "Open a file", QDir::homePath(),
//                                                    "image file(*.raw *.pbm *.pgm *.ppm *.bmp *.jpg *.png)");

//    QFile file(fileName);
//    file.open(QFile::ReadOnly);
//    QByteArray byteArray = file.readAll();
//    file.close();


//    for (int i = 10; i < 1000; i++) {
//        if (((i / 100) > 0) )
//            QString fileName = QString("00%1").arg(QString::number(i));
//        else
//            QString fileName = QString("0%1").arg(QString::number(i));

//    }



//    int width = 3000;
//    int height = 2400;

//    uchar *data = (uchar*)(byteArray.data());
//    QImage *img = new QImage(data, width, height, QImage::Format_Grayscale16);
//    QPixmap buf = QPixmap::fromImage(*img);
//    buf = buf.scaled(ui->reconView->width(), ui->reconView->height());

//    QGraphicsScene* scene = new QGraphicsScene;
//    scene->addPixmap(buf);
//    ui->reconView->setScene(scene);
    raw16ToBmp8();
}

#define X_RES 3000
#define Y_RES 2400
#define PIXELS 3000 * 2400

void ImagingManager::raw16ToBmp8()
{
    FILE *file;
    BITMAPFILEHEADER bmpHeader;
    BITMAPINFOHEADER bmpInfoHeader;
    RGBQUAD *pal;
    unsigned char *inImg, *outImg;

    file = fopen("CEPH.raw", "rb");
    if (file == nullptr) {
        return;
    }

    inImg = (unsigned char*)malloc(sizeof(unsigned char) * PIXELS * 2);
    outImg = (unsigned char*)malloc(sizeof(unsigned char) * PIXELS * 2);
//    pal = (RGBQUAD*)malloc(sizeof(RGBQUAD) * 256);
    memset(inImg, 0, PIXELS*2);
//    memset(outImg, 0, PIXELS*2);
    fread(inImg, sizeof(unsigned char)*PIXELS*2, 1, file);
    fclose(file);

    bmpHeader.bfType = 0x4D42;
    bmpHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(unsigned char) * 3000 * 2400 * 2;
    bmpHeader.bfReserved1 = 0;
    bmpHeader.bfReserved2 = 0;
    bmpHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfoHeader.biWidth = 3000;
    bmpInfoHeader.biHeight = 2400;
    bmpInfoHeader.biPlanes = 1;
    bmpInfoHeader.biBitCount = 16;
    bmpInfoHeader.biCompression = 0;
    bmpInfoHeader.SizeImage = sizeof(unsigned char) * 3000 * 2400 * 2;
    bmpInfoHeader.biXPelsPerMeter = 0;
    bmpInfoHeader.biYPelsPerMeter = 0;
    bmpInfoHeader.biClrUsed = 0;
    bmpInfoHeader.biClrImportant = 0;


    file = fopen("result.bmp", "wb");
    if (file == nullptr) {
        return;
    }
    qDebug("%d", sizeof(BITMAPFILEHEADER));
    qDebug("%d", sizeof(BITMAPINFOHEADER));
    fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, file);
    fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);
//    fwrite(pal, sizeof(RGBQUAD), 256, file);
    fwrite(inImg, sizeof(unsigned char)*3000*2400*2, 1, file);
    fclose(file);

    qDebug("END");

    delete inImg;
}



