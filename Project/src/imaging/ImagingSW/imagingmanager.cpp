#include "imagingmanager.h"
#include "ui_imagingmanager.h"
#include "imagethread.h"

#include <QProgressDialog>
#include <QDir>

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <math.h>




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

    connect(ui->reconButton, SIGNAL(clicked()), this, SLOT(reconImage()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveButtonSlot()));
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

    thread = new ImageThread(ui->viewLabel->width(), ui->viewLabel->height(), currentType, this);
    connect(thread, SIGNAL(imageProgressed(int)), ui->progressBar, SLOT(setValue(int)));
    connect(thread, SIGNAL(processFinished(const QPixmap&)), ui->viewLabel, SLOT(setPixmap(const QPixmap&)));
    thread->start();
}


void ImagingManager::stopButtonSlot()
{
    thread->threadStop();
//    thread->exit();
//    delete thread;
}

void ImagingManager::isProgressMaximum(int value)
{
    if (value == ui->progressBar->maximum()) {
        qDebug("촬영 종료");
        ui->reconButton->setEnabled(true);
        emit shootingEndSignal(currentType);

        //        thread->exit();
        //        delete thread;
    }
}

void ImagingManager::saveButtonSlot()
{
    emit saveSignal(currentPID + "|" + currentType);
    ui->reconButton->setEnabled(false);
    ui->viewLabel->clear();
}

#include <QFile>
void ImagingManager::reconImage()
{
    QFile file("./CEPH.bmp");

    file.open(QIODevice::ReadOnly);
    QByteArray buf = file.readAll();
    file.close();

    QPixmap pix(3000, 2400);
    pix.loadFromData(buf);
    ui->viewLabel->setPixmap(pix.scaledToHeight(ui->viewLabel->height()));

    QString fileName = currentPID + "_" + currentType + "." + file.fileName().section(".", -1);

    QDir dir(QString("recon/%1/").arg(QDate::currentDate().toString("yyyyMMdd")));
    if (!dir.exists())
        dir.mkpath(".");

    file.setFileName(dir.path() + "/" + fileName);
    file.open(QIODevice::WriteOnly);
    file.write(buf);
    file.close();

    ui->saveButton->setEnabled(true);
}


void ImagingManager::on_tempReconButton_clicked()
{
    // CEPH
    /*
    int count = 0;

    FILE *file;

    unsigned short *buf = new unsigned short[48*2400];
    unsigned short *out = new unsigned short[3000*2400];

    for (int k = 1000; k > 0; k--) {
        memset(buf, 0, 48*2400);
        QString fileName;
        if (k >= 1000)
            fileName = QString("./CEPH/%1.raw").arg(k);
        else if (k < 1000 && k >= 100)
            fileName = QString("./CEPH/0%1.raw").arg(k);
        else if (k < 100 && k >= 10)
            fileName = QString("./CEPH/00%1.raw").arg(k);
        else
            fileName = QString("./CEPH/000%1.raw").arg(k);


        file = fopen(fileName.toStdString().c_str(), "rb");
        qDebug() << fileName;
        if (file == nullptr) {
            qDebug() << "open is failed";
            return;
        }
        fread(buf, sizeof(unsigned short), 48 * 2400, file);
        fclose(file);

        //프레임데이터 히스토그램 스트레칭
        unsigned short min, max,range;
        min = 0;
        max = 350;
        range = max - min;

        for (int i = 0; i < 48*2400; i++) {
            if (buf[i] < max)
                buf[i] = cvRound(((double)(buf[i] - min) / range) * 65535.);
        }


        cv::Mat src(2400, 48, CV_16UC1, buf);
        cv::Mat dst;
        cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
        clahe->setClipLimit(32);
        clahe->setTilesGridSize(cv::Size2i(2,75));
        clahe->apply(src, dst);

        memcpy(buf, dst.data, 48*2400*2);


        // 이미지 스티칭
        for (int y = 0; y < 2400; y++) {
            out[(count*3)+y*3000] = buf[(25)+(2400-(y+1))*48];
            out[(count*3)+y*3000+1] = buf[(24)+(2400-(y+1))*48];
            out[(count*3)+y*3000+2] = buf[(23)+(2400-(y+1))*48];
        }

        count++;
    }

    cv::Mat src(2400, 3000, CV_16UC1, out);
    cv::Mat dst;
    cv::medianBlur(src, dst, 5);
    memcpy(out, dst.data, 3000*2400*2);

    file = fopen("./test/first.raw", "wb");
    fwrite(out, sizeof(unsigned short), 3000*2400, file);
    fclose(file);

    delete[] buf;
    delete[] out;
    */

    //PANO
    int count = 0;

    FILE *file;

    unsigned short *buf = new unsigned short[1152*64];
    unsigned short *out = new unsigned short[2320*1152];

    for (int k = 350; k < 1510; k++) {
        memset(buf, 0, 1152*64);
        QString fileName;
        if (k >= 1000)
            fileName = QString("./PANO/%1.raw").arg(k);
        else if (k < 1000 && k >= 100)
            fileName = QString("./PANO/0%1.raw").arg(k);
        else if (k < 100 && k >= 10)
            fileName = QString("./PANO/00%1.raw").arg(k);
        else
            fileName = QString("./PANO/000%1.raw").arg(k);


        file = fopen(fileName.toStdString().c_str(), "rb");
        qDebug() << fileName;
        if (file == nullptr) {
            qDebug() << "open is failed";
            return;
        }
        fread(buf, sizeof(unsigned short), 1152 * 64, file);
        fclose(file);

        // 프레임데이터 히스토그램 스트레칭
        unsigned short min, max,range;
        min = 0;
        max = 10000;
        range = max - min;

        for (int i = 0; i < 1152*64; i++) {
            if (buf[i] < max)
                buf[i] = cvRound(((double)(buf[i] - min) / range) * 65535.);
        }


//        cv::Mat src(64, 1152, CV_16UC1, buf);
//        cv::Mat dst;
//        cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
//        clahe->setClipLimit(2000);
//        clahe->setTilesGridSize(cv::Size2i(8, 8));
//        clahe->apply(src, dst);

//        memcpy(buf, dst.data, 64*1152*2);


        // 이미지 스티칭
        for (int y = 0; y < 1152; y++) {
            out[(count*2)+y*2320] = buf[(1152*33)+y];
            out[(count*2)+y*2320+1] = buf[(1152*32)+y];
        }

        count++;
    }


//    cv::Mat src(1152, 2320, CV_16UC1, out);
//    cv::Mat dst;
//    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
//    clahe->setClipLimit(2000);
//    clahe->setTilesGridSize(cv::Size2i(10, 20));
//    clahe->apply(src, dst);

//    memcpy(out, dst.data, 2320*1152*2);

    cv::Mat src(1152, 2320, CV_16UC1, out);
    cv::Mat dst;
    cv::medianBlur(src, dst, 5);
    memcpy(out, dst.data, 2320*1152*2);

    file = fopen("./test/first.raw", "wb");
    fwrite(out, sizeof(unsigned short), 2320*1152, file);
    fclose(file);

    delete[] buf;
    delete[] out;

    qDebug("recon end");
}

void ImagingManager::on_tempFilterButton_2_clicked()
{
    // CEPH
    /*
    FILE *file;
    unsigned short *img = new unsigned short[3000*2400];

    file = fopen("./test/first.raw", "rb");
    fread(img, sizeof(unsigned short), 3000*2400, file);
    fclose(file);


//****************************************************************************************************
// 감마 보정
    for (int i = 0; i < 3000*2400; i++) {
        img[i] = 65535 * ((double)pow((double)((double)(img[i]) / (double)65535.0), (double)(5.0 / 10.0)));
    }


//****************************************************************************************************
// CLAHE
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->setClipLimit(1);
    clahe->setTilesGridSize(cv::Size2i(4,4));

    cv::Mat src1(2400, 3000, CV_16UC1, img);
    cv::Mat dst1;
    clahe->apply(src1, dst1);
    memcpy(img, dst1.data, 3000*2400*2);

//****************************************************************************************************
// 미디언 필터
//    cv::Mat src4(2400, 3000, CV_16UC1, img);
//    cv::Mat dst4;
//    cv::medianBlur(src4, dst4, 3);
//    memcpy(img, dst4.data, 3000*2400*2);


//****************************************************************************************************
// 언샤프드 필터




//****************************************************************************************************
// 값 역전


//****************************************************************************************************

    file = fopen("./test/RECON.raw", "wb");
    fwrite(img, sizeof(unsigned short), 3000*2400, file);
    fclose(file);

    qDebug() << "filter1 end";
    delete[] img;
    */


    // PANO
    /*
    FILE *file;
    unsigned short *img = new unsigned short[2320*1152];

    file = fopen("./test/first.raw", "rb");
    fread(img, sizeof(unsigned short), 2320*1152, file);
    fclose(file);


//****************************************************************************************************
// 감마 보정
    for (int i = 0; i < 2320*1152; i++) {
        img[i] = 65535 * ((double)pow((double)((double)(img[i]) / (double)65535.0), (double)(5.0 / 10.0)));
    }


//****************************************************************************************************
// CLAHE
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->setClipLimit(1);
    clahe->setTilesGridSize(cv::Size2i(4,4));

    cv::Mat src1(1152, 2320, CV_16UC1, img);
    cv::Mat dst1;
    clahe->apply(src1, dst1);
    memcpy(img, dst1.data, 2320*1152*2);

//****************************************************************************************************
// 미디언 필터
//    cv::Mat src4(1152, 2320, CV_16UC1, img);
//    cv::Mat dst4;
//    cv::medianBlur(src4, dst4, 3);
//    memcpy(img, dst4.data, 2320*1152*2);


//****************************************************************************************************
// 언샤프드 필터




//****************************************************************************************************
// 값 역전


//****************************************************************************************************

    file = fopen("./test/RECON.raw", "wb");
    fwrite(img, sizeof(unsigned short), 2320*1152, file);
    fclose(file);

    qDebug() << "filter1 end";
    delete[] img;
    */


#define PIXELS 1152*2320
#define Bpp 2
#define X_RES 2320
#define Y_RES 1152
    FILE *file;
    unsigned char *inImg;

    file = fopen("./test/first.raw", "rb");

    if (file == nullptr) {
        qDebug("ERROR : Failed file openning! %d", __LINE__);
        return;
    }

    inImg = (unsigned char*)malloc(sizeof(unsigned char) * PIXELS * Bpp);
    memset(inImg, 0, PIXELS*2);
    fread(inImg, sizeof(unsigned char) * PIXELS * Bpp, 1, file);
    fclose(file);


    cv::Mat src(1152, 2320, CV_16UC1, inImg);
    cv::Mat dst;
    src.convertTo(dst, CV_8UC1, 1 / 256.0);


    cv::imwrite("./ToBMP.bmp", dst);

    qDebug("END");

    delete inImg;
}

