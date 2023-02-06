#include "imagingmanager.h"
#include "ui_imagingmanager.h"
#include "imagethread.h"

#include <QFile>
#include <QProgressDialog>
#include <QDir>
#include <QPainter>
#include <QMessageBox>

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <math.h>

using namespace cv;
//using namespace std;

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

    if (currentType == "CEPH") {
        ui->progressBar->setRange(0, 1249);
    } else if (currentType == "PANO") {
        ui->progressBar->setRange(0, 1749);
    }



    thread = new ImageThread(ui->viewLabel->width(), ui->viewLabel->height(), "CEPH", this);
    connect(thread, SIGNAL(processFinished(const QPixmap&)), ui->viewLabel, SLOT(setPixmap(const QPixmap&)));
    thread->start();
}

void ImagingManager::recvFrameImg(int count)
{
    ui->progressBar->setValue(count);
    if (count == 0) {
        thread = new ImageThread(ui->viewLabel->width(), ui->viewLabel->height(), currentType, this);
        connect(thread, SIGNAL(processFinished(const QPixmap&)), ui->viewLabel, SLOT(setPixmap(const QPixmap&)));
        thread->start();
    }
}

void ImagingManager::stopButtonSlot()
{
    thread->threadStop();
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
    ui->reconButton->setEnabled(false);
    ui->viewLabel->clear();
}

void ImagingManager::reconImage()
{
    FILE *file;
    QString fileName;
    int count = 0;
    int frameRows, frameCols;
    int reconRows, reconCols;

    if (currentType == "PANO") {
        frameRows = 64;
        frameCols = 1152;

        reconRows = 1152;
        reconCols = 2320;

        unsigned short *buf = new unsigned short[frameRows*frameCols];
        unsigned short *out = new unsigned short[reconRows*reconCols];


        for (int k = 350; k < 1510; k++) {
            memset(buf, 0, frameRows*frameCols);
            fileName = makeFileName(currentType, k);

            fopen_s(&file, fileName.toStdString().c_str(), "rb");
            if (file == nullptr) {
                qDebug() << "파일 오픈 오류";
                delete[] buf;
                delete[] out;
                return;
            }

            fread(buf, sizeof(unsigned short), frameRows*frameCols, file);
            fclose(file);

            histoStretch(buf, frameRows*frameCols, 0, 14296, 65535.);

            // 이미지 스티칭
            for (int y = 0; y < 1152; y++) {
                out[(count*2)+y*2320] = buf[(1152*33)+y];
                out[(count*2)+y*2320+1] = buf[(1152*32)+y];
            }
            count++;
            qDebug("%d", count);
        }
        saveAsBmp(out, reconRows, reconCols);
        viewReconImage(reconRows, reconCols);

        delete[] buf;
        delete[] out;
    } else if (currentType == "CEPH") {
        frameRows = 2400;
        frameCols = 48;

        reconRows = 2400;
        reconCols = 3000;

        unsigned short *buf = new unsigned short[frameRows*frameCols];
        unsigned short *out = new unsigned short[reconRows*reconCols];

        for (int k = 1000; k > 0; k--) {
            memset(buf, 0, frameRows*frameCols);
            fileName = makeFileName(currentType, k);

            fopen_s(&file, fileName.toStdString().c_str(), "rb");
            if (file == nullptr) {
                qDebug() << "파일 오픈 오류";
                delete[] buf;
                delete[] out;
                return;
            }
            fread(buf, sizeof(unsigned short), frameRows*frameCols, file);
            fclose(file);

            histoStretch(buf, frameRows*frameCols, 0, 400, 65535.);
//            CLAHE(buf, frameRows, frameCols, 32, 2, 30);

            // 이미지 스티칭
            for (int y = 0; y < 2400; y++) {
                out[(count*3)+y*3000] = buf[(25)+(2400-(y+1))*48];
                out[(count*3)+y*3000+1] = buf[(24)+(2400-(y+1))*48];
                out[(count*3)+y*3000+2] = buf[(23)+(2400-(y+1))*48];
            }
            count++;
            qDebug("%d", count);
        }
        medianFilter(out, reconRows, reconCols, 5);
        gammaCorrection(out, reconRows*reconCols, 65535.0, 6.0);
        CLAHE(out, reconRows, reconCols, 1, 8, 8);
        medianFilter(out, reconRows, reconCols, 5);
        //****************************************************************************************************
        // 언샤프드 필터
        // 값 역전

        saveAsBmp(out, reconRows, reconCols);
        viewReconImage(reconRows, reconCols);

        delete[] buf;
        delete[] out;
    } else {
        QMessageBox checkType(QMessageBox::NoIcon, "NONE TYPE",
                              "선택된 모드를 확인하세요.", QMessageBox::Ok,
                              this, Qt::Dialog);
        checkType.exec();
        return;
    }

    ui->saveButton->setEnabled(true);
}



QString ImagingManager::makeFileName(QString type, int count)
{
    if (count >= 1000)
        return QString("./image/frame/%1/%2.raw").arg(type).arg(count);
    else if (count < 1000 && count >= 100)
        return QString("./image/frame/%1/0%2.raw").arg(type).arg(count);
    else if (count < 100 && count >= 10)
        return QString("./image/frame/%1/00%2.raw").arg(type).arg(count);
    else
        return QString("./image/frame/%1/000%2.raw").arg(type).arg(count);
}


void ImagingManager::histoStretch(unsigned short *input, int inputSize, int min, int max, double valueMax)
{
    double range = max - min;

    for (int i = 0; i < inputSize; i++) {
        if (input[i] < max)
            input[i] = cvRound(((double)(input[i] - min) / range) * valueMax);
    }
}

void ImagingManager::CLAHE(unsigned short *input, int rows, int cols, int clipLimit, int gridX, int gridY)
{
    cv::Mat src(rows, cols, CV_16UC1, input);
    cv::Mat dst;
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->setClipLimit(clipLimit);
    clahe->setTilesGridSize(cv::Size2i(gridX, gridY));
    clahe->apply(src, dst);

    memcpy(input, dst.data, rows*cols*2);
}

void ImagingManager::medianFilter(unsigned short *input, int rows, int cols, int ksize)
{
    if (((ksize & 0b1) == 0) || ksize < 1) {
        qDebug() << "커널의 크기는 1이상의 홀수만 입력하세요.";
        return;
    }

    cv::Mat src(rows, cols, CV_16UC1, input);
    cv::Mat dst;
    cv::medianBlur(src, dst, 5);

    memcpy(input, dst.data, rows*cols*2);
}

void ImagingManager::gammaCorrection(unsigned short *input, int inputSize, double valueMax, double gamma)
{
    for (int i = 0; i < inputSize; i++) {
        input[i] = valueMax * ((double)pow((double)((double)(input[i]) / valueMax), (double)(gamma / 10.0)));
    }
}

void ImagingManager::unsharpFilter()
{

}

void ImagingManager::saveAsBmp(unsigned short *input, int rows, int cols)
{
    QString reconName = QString("./image/recon/%1.bmp").arg(currentPID + "_" + currentType);
    cv::Mat src(rows, cols, CV_16UC1, input);
    cv::Mat dst;
    src.convertTo(dst, CV_8UC1, 1 / 256.0);
    cv::imwrite(reconName.toStdString().c_str(), dst);
}

void ImagingManager::viewReconImage(int rows, int cols)
{
    QFile file(QString("./image/recon/%1.bmp").arg(currentPID+"_"+currentType));
    file.open(QIODevice::ReadOnly);
    QByteArray buf = file.readAll();
    file.close();

    QPixmap pix(cols, rows);
    pix.loadFromData(buf);
    ui->viewLabel->setPixmap(pix.scaled(ui->viewLabel->width(), ui->viewLabel->height(), Qt::KeepAspectRatio));
}

