#include "imagingmanager.h"
#include "ui_imagingmanager.h"
#include "imagethread.h"

#include <QProgressDialog>

#include <opencv2/opencv.hpp>
#include <stdio.h>


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
}

ImagingManager::~ImagingManager()
{
    delete ui;
}

void ImagingManager::setPID(QString pid)
{
    m_pid = pid;
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
void ImagingManager::setType(QString type)
{
    m_type = type;
}

void ImagingManager::loadImage()
{
    m_type = "PANO";
    if (m_type == "CEPH") {
        ui->progressBar->setRange(0, 866);
    } else if (m_type == "PANO") {
        ui->progressBar->setRange(0, 1600);
    } else {
//        return;
    }

    ImageThread *thread = new ImageThread(ui->viewLabel->width(), ui->viewLabel->height(), m_type, this);
    connect(thread, SIGNAL(imageProgressed(int)), ui->progressBar, SLOT(setValue(int)));
    connect(thread, SIGNAL(processFinished(const QPixmap&)), ui->viewLabel, SLOT(setPixmap(const QPixmap&)));
    thread->start();
}

void ImagingManager::reconImage()
{
    // 스티칭
    /*
    vector<Mat> imgs;
    int cnt = 0;
    for(int i = 100; i < 1000; i++)
    {
        QString fileName = QString("./CEPH/0%1.raw").arg(i);
//        QString fileName = QString("./image/0350.raw");
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        QByteArray array = file.readAll();
        file.close();

        uchar* data = (uchar*)array.data();

        Mat mat(2400, 48, CV_16UC1, data);
//        Mat rotateMat(1152, 64, CV_16UC1);
//        rotate(mat, rotateMat, cv::ROTATE_90_CLOCKWISE);
        Mat beforeMat(2400, 48, CV_8UC1);
        mat.convertTo(beforeMat, CV_8UC1);
        Mat his(2400, 48, CV_8UC1);
        equalizeHist(beforeMat, his);

        imgs.push_back(his);
        cnt++;
        qDebug("%d", cnt);
    }

    Ptr<Stitcher> stitcher = Stitcher::create();

    Mat dst(2400, 3000, CV_8UC1);
    qDebug("%d", __LINE__);
    Stitcher::Status status = stitcher->stitch(imgs, dst);
    qDebug("%d", __LINE__);
    if (status != Stitcher::Status::OK) {
        qDebug() << "Error on stitching!";
        return;
    }
    imshow("dst", dst);
    waitKey();
//    QImage frameImage(dst.data, dst.cols, dst.rows, dst.step ,QImage::Format_Grayscale8);
//    ui->reconLabel->setPixmap(QPixmap::fromImage(frameImage).scaledToWidth(ui->reconLabel->width()));
    */


    FILE *file;
    file = fopen("./image/origin/PANO/0555.raw", "rb");

    if (file == nullptr) {
        qDebug() << "open is failed";
        return;
    }

    unsigned short *buf;
    unsigned char *data;
    buf = (unsigned short*)malloc(sizeof(unsigned short) * 1152 * 64);
    data = (unsigned char*)malloc(sizeof(unsigned char) * 1152 * 64 * 2);

    fread(buf, sizeof(unsigned short), 1152 * 64, file);
    fclose(file);

    int temp = 0;
    int min = 65535;
    int max = 0;
    int range;

    for (int i = 0; i < 1152 * 64; i++) {
        temp = buf[i];
        if (min > temp)
            min = temp;
        else if (max < temp)
            max = temp;
    }

    range = max - min;
    for (int i = 0; i < 1152 * 64; i++) {
        buf[i] = ~(unsigned short)(((double)(buf[i] - min) / (double)(range)) * 65535);
    }

    file = fopen("./result.raw", "wb");
    fwrite(buf, sizeof(unsigned short), 1152 * 64, file);
    fclose(file);

    file = fopen("./result.raw", "rb");
    fread(data, sizeof(unsigned char), 1152 * 64 * 2, file);
    fclose(file);

    QImage frameImage(data, 1152, 64, QImage::Format_Grayscale16);
    ui->viewLabel->setPixmap(QPixmap::fromImage(frameImage).scaledToWidth(ui->viewLabel->width()));
}

void ImagingManager::saveButtonSlot()
{
    emit saveSignal(m_pid);
}
