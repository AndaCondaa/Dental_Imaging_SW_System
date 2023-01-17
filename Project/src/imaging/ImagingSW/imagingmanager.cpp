#include "imagingmanager.h"
#include "ui_imagingmanager.h"

#include <QFileDialog>
#include <QLabel>
#include <QProgressDialog>

#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdio>
#include <time.h>

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
//    raw16ToBmp8();
    simpleStiching();
}

#define X_RES 3000
#define Y_RES 2400
#define PIXELS 3000 * 2400
#define Bpp 2

void ImagingManager::raw16ToBmp8()
{
    FILE *file;
    BITMAPFILEHEADER bmpHeader;
    BITMAPINFOHEADER bmpInfoHeader;
    RGBQUAD *pal;
    unsigned char *inImg, *outImg;

    file = fopen("CEPH.raw", "rb");

    if (file == nullptr) {
        qDebug("ERROR : Failed file openning! %d", __LINE__);
        return;
    }

    inImg = (unsigned char*)malloc(sizeof(unsigned char) * PIXELS * Bpp);
    outImg = (unsigned char*)malloc(sizeof(unsigned char) * PIXELS);
    pal = (RGBQUAD*)malloc(sizeof(RGBQUAD) * 256);
    memset(inImg, 0, PIXELS*2);
    memset(outImg, 0, PIXELS);

    fread(inImg, sizeof(unsigned char) * PIXELS * Bpp, 1, file);
    fclose(file);

    for (int i = 0; i < 256; i++) {
        pal[i].rgbBlue = i;
        pal[i].rgbGreen = i;
        pal[i].rgbRed = i;
        pal[i].rgbReserved = i;
    }

    for (int i = 0; i < PIXELS; i++) {
        outImg[i] = (unsigned char)(((double)(((inImg[(i*Bpp)] << 8) | (inImg[(i*Bpp)+1])) / 65536)) * 255);
    }

    bmpHeader.bfType = 0x4D42;
    bmpHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256
                                + sizeof(unsigned char) * PIXELS;
    bmpHeader.bfReserved1 = 0;
    bmpHeader.bfReserved2 = 0;
    bmpHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256;

    bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfoHeader.biWidth = X_RES;
    bmpInfoHeader.biHeight = Y_RES;
    bmpInfoHeader.biPlanes = 1;
    bmpInfoHeader.biBitCount = 8;
    bmpInfoHeader.biCompression = 0;
    bmpInfoHeader.SizeImage = sizeof(unsigned char) * PIXELS;
    bmpInfoHeader.biXPelsPerMeter = X_RES;
    bmpInfoHeader.biYPelsPerMeter = Y_RES;
    bmpInfoHeader.biClrUsed = 0;
    bmpInfoHeader.biClrImportant = 0;

    file = fopen("rawToBMP.bmp", "wb");
    if (file == nullptr) {
        qDebug("ERROR : Failed file openning! %d", __LINE__);
        return;
    }

    fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, file);
    fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);
    fwrite(pal, sizeof(RGBQUAD), 256, file);
    fwrite(outImg, sizeof(unsigned char) * PIXELS, 1, file);
    fclose(file);

    qDebug("END");

    delete inImg;
    delete outImg;
    delete pal;
}

void ImagingManager::simpleStiching()
{
    int cnt = 0;
    clock_t p1, p2;
    QByteArray array;
    QByteArray addData;
    QString fileName;
    QFile file;
    QByteArray newData;
    uchar* data;


    for (int i = 100; i < 1000; i++) {
        p1 = clock();

        fileName = QString("./image/0%1.raw").arg(QString::number(i));
        file.setFileName(fileName);
        file.open(QIODevice::ReadOnly);
        newData = file.readAll();
        file.close();

        for (int j = 0; j < 2400; j++) {
            if (cnt == 0) {
                array.append(addData);
                break;
            }
            array.insert((cnt*2)+((cnt+1)*2*j)+1, newData[(j*48*2)]);
            array.insert((cnt*2)+((cnt+1)*2*j)+2, newData[(j*48*2)+1]);
        }

        data = (uchar*)(array.data());
        QImage *img = new QImage(data, cnt, 2400, QImage::Format_Grayscale16);
        QPixmap buf = QPixmap::fromImage(*img);
        buf = buf.scaled(cnt, ui->reconView->height(), Qt::KeepAspectRatio);

        QGraphicsScene *scene = new QGraphicsScene;
        scene->addPixmap(buf);
        ui->reconView->setScene(scene);

        p2 = clock();
        qDebug("%d : %lf", cnt, (double)(p2-p1));
        cnt++;
//        qDebug("%d", cnt);
    }

//    QImage *img = new QImage(data, cnt, 2400, QImage::Format_Grayscale16);
//    QPixmap buf = QPixmap::fromImage(*img);
//    buf = buf.scaled(cnt, ui->reconView->height(), Qt::KeepAspectRatio);

//    QGraphicsScene *scene = new QGraphicsScene;
//    scene->addPixmap(buf);
//    ui->reconView->setScene(scene);

//    int width = 3000;
//    int height = 2400;

//    uchar *data = (uchar*)(byteArray.data());
//    QImage *img = new QImage(data, width, height, QImage::Format_Grayscale16);
//    QPixmap buf = QPixmap::fromImage(*img);
//    buf = buf.scaled(ui->reconView->width(), ui->reconView->height());

//    QGraphicsScene* scene = new QGraphicsScene;
//    scene->addPixmap(buf);
//    ui->reconView->setScene(scene);
}
