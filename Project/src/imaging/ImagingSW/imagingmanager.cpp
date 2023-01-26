#include "imagingmanager.h"
#include "ui_imagingmanager.h"
#include "imagethread.h"

#include <QProgressDialog>

#include <opencv2/opencv.hpp>
#include <stdio.h>

/*
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
*/

ImagingManager::ImagingManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImagingManager)
{
    ui->setupUi(this);

    connect(ui->reconButton, SIGNAL(clicked()), this, SLOT(reconImage()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveButtonSlot()));
    connect(ui->finishButton, SIGNAL(clicked()), this, SLOT(finishButtonSlot()));
    connect(ui->progressBar, SIGNAL(valueChanged(int)), this, SLOT(isProgressMaximum(int)));
}

ImagingManager::~ImagingManager()
{
    delete ui;
}

void ImagingManager::setPID(QString pid)
{
    currentPID = pid;
}

void ImagingManager::setType(QString type)
{
    currentType = type;
}

void ImagingManager::raw16ToBmp8()
{
    /*
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
    */
}

void ImagingManager::loadImage()
{
    if (currentType == "CEPH") {
        ui->progressBar->setRange(0, 865);
    } else if (currentType == "PANO") {
        ui->progressBar->setRange(0, 1589);
    } else {
        return;
    }

    ImageThread *thread = new ImageThread(ui->viewLabel->width(), ui->viewLabel->height(), currentType, this);
    connect(thread, SIGNAL(imageProgressed(int)), ui->progressBar, SLOT(setValue(int)));
    connect(thread, SIGNAL(processFinished(const QPixmap&)), ui->viewLabel, SLOT(setPixmap(const QPixmap&)));
    connect(this, SIGNAL(stopThread()), thread, SLOT(terminate()));
    thread->start();
}

void ImagingManager::finishButtonSlot()
{
    emit finishSignal(currentPID);
}

void ImagingManager::stopButtonSlot()
{
    emit stopThread();
}

void ImagingManager::isProgressMaximum(int value)
{
    if (value == ui->progressBar->maximum()) {
        qDebug("촬영 종료");
        ui->reconButton->setEnabled(true);
        emit shootingEndSignal(currentType);
    }
}

void ImagingManager::saveButtonSlot()
{
    emit saveSignal(currentPID + "|" + currentType);
}

void ImagingManager::reconImage()
{
    int count = 0;

    FILE *file;

    unsigned short *buf = new unsigned short[48*2400];
    unsigned short *out = new unsigned short[3000*2400];

    for (int k = 21; k < 1021; k++) {
        memset(buf, 0, 48*2400);
        QString fileName;
        if (k < 100)
            fileName = QString("./CEPH/00%1.raw").arg(k);
        else if (k >= 100 && k < 1000)
            fileName = QString("./CEPH/0%1.raw").arg(k);
        else if (k >= 1000)
            fileName = QString("./CEPH/%1.raw").arg(k);

        file = fopen(fileName.toStdString().c_str(), "rb");
        qDebug() << fileName;
        if (file == nullptr) {
            qDebug() << "open is failed";
            return;
        }

        fread(buf, sizeof(unsigned short), 48 * 2400, file);
        fclose(file);

        for (int y = 0; y < 2400; y++) {
            for (int x = 0; x < 3; x++) {
                out[(x+count*3)+y*3000] = buf[(23+x)+y*48];
            }
        }

        count++;
    }

    file = fopen("./result1.raw", "wb");
    fwrite(out, sizeof(unsigned short), 3000*2400, file);
    fclose(file);

    delete[] buf;
    delete[] out;
}

void ImagingManager::on_filter1Button_clicked()
{
    FILE *file;
    unsigned short *img = new unsigned short[3000*2400];

    file = fopen("./result1.raw", "rb");
    fread(img, sizeof(unsigned short), 3000*2400, file);
    fclose(file);

//    for (int i = 0; i < 3000*2400; i++) {
//        if (img[i]*50 > 65535) {
//            img[i] = 65535;
//        } else {
//            img[i] *= 50;
//        }
//    }

    file = fopen("./result2.raw", "wb");
    fwrite(img, sizeof(unsigned short), 3000*2400, file);
    fclose(file);

    qDebug() << "filter1 end";
    delete[] img;
}


void ImagingManager::on_filter2Button_2_clicked()
{

}

