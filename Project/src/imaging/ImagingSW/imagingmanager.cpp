#include "imagingmanager.h"
#include "ui_imagingmanager.h"
#include "imagethread.h"

#include <QProgressDialog>
#include <QDir>

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <math.h>



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
    qDebug("%s %d", __FUNCTION__, __LINE__);
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
}


void ImagingManager::on_tempReconButton_clicked()
{
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

    file = fopen("./test/RECON.raw", "wb");
    fwrite(out, sizeof(unsigned short), 3000*2400, file);
    fclose(file);

    delete[] buf;
    delete[] out;
}

void ImagingManager::on_tempFilterButton_2_clicked()
{
    FILE *file;
    unsigned short *mix = new unsigned short[3000*2400];
    unsigned short *section1 = new unsigned short[3000*2400];
    unsigned short *section2 = new unsigned short[3000*2400];
    unsigned short *section3 = new unsigned short[3000*2400];

    file = fopen("./test/RECON.raw", "rb");
    fread(section1, sizeof(unsigned short), 3000*2400, file);
    fseek(file, 0, SEEK_SET);
    fread(section2, sizeof(unsigned short), 3000*2400, file);
    fseek(file, 0, SEEK_SET);
    fread(section3, sizeof(unsigned short), 3000*2400, file);
    fclose(file);

    for (int i = 0; i < 3000*2400; i++) {
        if (section1[i] < 21000)
            section1[i] = cvRound(((double)(section1[i] - 100) / 20900) * 55000.);
        else if (section1[i] > 21000 && section2[i] < 55000)
            section1[i] = cvRound(((double)(section1[i] - 21000) / 34000) * 10000.) + 45000;
    }






//****************************************************************************************************
// 감마 보정
//    for (int i = 0; i < 3000*2400; i++) {
//        img[i] = 65535 * ((double)pow((double)((double)(img[i]) / (double)65535.0), (double)(6.0 / 10.0)));
//    }


//****************************************************************************************************
// 섹션 분할
//    for (int i = 0; i < 3000*2400; i++) {
//        int tmp = section1[i];
//        if (tmp < 100) {
//            section2[i] = 0;
//            section3[i] = 0;
//        } else if (tmp <= 30000 && tmp > 100) {
//            section1[i] = 0;
//            section3[i] = 0;
//        } else if (tmp > 30000) {
//            section1[i] = 0;
//            section2[i] = 0;
//        }
//    }


//    unsigned int *his = new unsigned int[65535];
//    unsigned int *sum = new unsigned int[65535];
//    unsigned int *col = new unsigned int[65535];
//    memset(his, 0, 65535);
//    memset(sum, 0, 65535);
//    memset(col, 0, 65535);


//    for (int i = 0; i < 3000*2400; i++) {
//        his[section2[i]]++;
//    }

//    int tmp = 0;
//    for (int i = 0; i < 65535; i++) {
//        tmp += his[i];
//        sum[i] = tmp;
//    }

//    for (int i = 0; i < 65535; i++) {
//        col[i] = cvRound((double)(sum[i] * 65535 / 7200000.));
//    }

//    for (int i = 0 ; i < 3000*2400; i++) {
//        section2[i] = col[section2[i]];
//    }





//    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
//    clahe->setClipLimit(1);
//    clahe->setTilesGridSize(cv::Size2i(4,4));

//    cv::Mat src1(2400, 3000, CV_16UC1, section1);
//    cv::Mat dst1;
//    clahe->apply(src1, dst1);
//    memcpy(section1, dst1.data, 3000*2400*2);

//    cv::Mat src2(2400, 3000, CV_16UC1, section2);
//    cv::Mat dst2;
//    clahe->apply(src2, dst2);
//    memcpy(section2, dst2.data, 3000*2400*2);

//    cv::Mat src3(2400, 3000, CV_16UC1, section3);
//    cv::Mat dst3;
//    clahe->apply(src3, dst3);
//    memcpy(section3, dst3.data, 3000*2400*2);

//****************************************************************************************************
// 미디언 필터
//    cv::Mat src4(2400, 3000, CV_16UC1, section1);
//    cv::Mat dst4;
//    cv::medianBlur(src4, dst4, 3);
//    memcpy(section1, dst4.data, 3000*2400*2);

//    cv::Mat src5(2400, 3000, CV_16UC1, section2);
//    cv::Mat dst5;
//    cv::medianBlur(src5, dst5, 3);
//    memcpy(section2, dst5.data, 3000*2400*2);

//    cv::Mat src6(2400, 3000, CV_16UC1, section3);
//    cv::Mat dst6;
//    cv::medianBlur(src6, dst6, 3);
//    memcpy(section3, dst6.data, 3000*2400*2);

//****************************************************************************************************
// 히스토그램 노멀라이징



//****************************************************************************************************
// 히스토그램 평활화



//****************************************************************************************************
// 블렌딩
    for (int i = 0; i < 3000*2400; i++) {
        mix[i] = section1[i] + section2[i] + section3[i];
    }

//****************************************************************************************************
// 값 역전


//****************************************************************************************************



    file = fopen("./test/section1.raw", "wb");
    fwrite(section1, sizeof(unsigned short), 3000*2400, file);
    fclose(file);

    file = fopen("./test/section2.raw", "wb");
    fwrite(section2, sizeof(unsigned short), 3000*2400, file);
    fclose(file);

    file = fopen("./test/section3.raw", "wb");
    fwrite(section3, sizeof(unsigned short), 3000*2400, file);
    fclose(file);

    file = fopen("./test/mix.raw", "wb");
    fwrite(mix, sizeof(unsigned short), 3000*2400, file);
    fclose(file);

    qDebug() << "filter1 end";
    delete[] section1;
    delete[] section2;

}

