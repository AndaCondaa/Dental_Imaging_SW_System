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

    thread = new ImageThread(ui->viewLabel->width(), ui->viewLabel->height(), currentType, this);
    connect(thread, SIGNAL(imageProgressed(int)), ui->progressBar, SLOT(setValue(int)));
    connect(thread, SIGNAL(processFinished(const QPixmap&)), ui->viewLabel, SLOT(setPixmap(const QPixmap&)));
    thread->start();
}

void ImagingManager::recvFrameImg(int count)
{
//    ui->progressBar->setValue(count);
//    if (count == 0) {
//        thread = new ImageThread(ui->viewLabel->width(), ui->viewLabel->height(), currentType, this);
//        connect(thread, SIGNAL(processFinished(const QPixmap&)), ui->viewLabel, SLOT(setPixmap(const QPixmap&)));
//        thread->start();
//    }
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
        memset(out, 0, reconRows*reconCols);

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

//            // 이미지 스티칭
//            for (int y = 0; y < 1152; y++) {
//                out[(count*2)+y*2320] = buf[(1152*33)+y];
//                out[(count*2)+y*2320+1] = buf[(1152*32)+y];
//            }

            count++;
            qDebug("%d", count);
        }

        gammaCorrection(out, reconRows*reconCols, 65535.0, 0.5);
        invertImage(out, reconRows*reconCols);
        saveAsJpg(out, reconRows, reconCols);
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
        memset(out, 0, reconRows*reconCols);

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

            // 이미지 스티칭
//            for (int y = 0; y < 2400; y++) {
//                out[(count*3)+y*3000] = buf[(25)+(2400-(y+1))*48];
//                out[(count*3)+y*3000+1] = buf[(24)+(2400-(y+1))*48];
//                out[(count*3)+y*3000+2] = buf[(23)+(2400-(y+1))*48];
//            }


            count++;
            qDebug("%d", count);
        }

        ui->progressBar->setValue(ui->progressBar->maximum()/3);
//        gammaCorrection(out, reconRows*reconCols, 65535.0, 0.9);
        CLAHE(out, reconRows, reconCols, 16.0, 16, 16);
        medianFilter(out, reconRows, reconCols, 3);

        ui->progressBar->setValue(ui->progressBar->maximum()*2/3);
        invertImage(out, reconRows*reconCols);

        unsigned short *tmp1 = new unsigned short[reconRows*reconCols];
        unsigned short *tmp2 = new unsigned short[reconRows*reconCols];
        memcpy(tmp1, out, reconRows*reconCols*2);
        memcpy(tmp2, out, reconRows*reconCols*2);

        gammaCorrection(tmp1, reconRows*reconCols, 65535.0, 10);
        gammaCorrection(tmp2, reconRows*reconCols, 65535.0, 0.4);
        CLAHE(tmp1, reconRows, reconCols, 16.0, 8, 8);
        CLAHE(tmp2, reconRows, reconCols, 16.0, 8, 8);
//        medianFilter(tmp1, reconRows, reconCols, 3);
        medianFilter(tmp2, reconRows, reconCols, 3);
//        for (int i = 0; i < reconRows*reconCols; i++) {
//            out[i] = (tmp1[i] + tmp2[i])/2;
//        }
//        medianFilter(out, reconRows, reconCols, 3);

        ui->progressBar->setValue(ui->progressBar->maximum());
        unsharpFilter(tmp1, reconRows, reconCols, 65535);
//        medianFilter(out, reconRows, reconCols, 5);

//        saveAsJpg(out, reconRows, reconCols);
//        saveAsJpg(tmp1, reconRows, reconCols);
//        viewReconImage(reconRows, reconCols);

        FILE *file;
        fopen_s(&file, "./recon.raw", "wb");

        // *****************************************************************************
        if (file == nullptr) {

        }

        fwrite(tmp1, sizeof(unsigned short), reconRows*reconCols, file);
        fclose(file);

        delete[] tmp1;
        delete[] tmp2;
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
        input[i] = valueMax * (double)((double)pow((double)((double)(input[i]) / valueMax), (double)gamma));
    }
}

void ImagingManager::unsharpFilter(unsigned short *input, int rows, int cols, int valueMax)
{
    unsigned short *result = new unsigned short[rows*cols];
    memcpy(result, input, rows*cols*2);

    double kernel[3][3] = {{0., -1., 0.},
                           {-1., 5., -1.},
                           {0., -1., 0.}};

    for(int y = 1; y < rows-1; y++) {
        for(int x = 1; x < cols-1; x++) {
            double sum = 0.0;
            for (int i = -1; i < 2; i++) {
                for(int j = -1; j < 2; j++) {
                    sum += kernel[i+1][j+1]*input[(x+i)+(y+j)*cols];
                }
            }
            result[(x-1)+(y-1)*cols] = (sum >= valueMax) ? valueMax : sum;
        }
    }

    memcpy(input, result, rows*cols*2);

    delete[] result;
}

void ImagingManager::invertImage(unsigned short *input, int inputSize)
{
    for (int i = 0; i < inputSize; i++) {
        input[i] = ~input[i];
    }
}

void ImagingManager::saveAsJpg(unsigned short *input, int rows, int cols)
{
    QString reconName = QString("./image/recon/%1.jpg").arg(currentPID + "_" + currentType);
    cv::Mat src(rows, cols, CV_16UC1, input);
    cv::Mat dst;
    src.convertTo(dst, CV_8UC1, 1 / 256.0);
    cv::imwrite(reconName.toStdString().c_str(), dst);
}

void ImagingManager::viewReconImage(int rows, int cols)
{
    QFile file(QString("./image/recon/%1.jpg").arg(currentPID+"_"+currentType));
    file.open(QIODevice::ReadOnly);
    QByteArray buf = file.readAll();
    file.close();

    QPixmap pix(cols, rows);
    pix.loadFromData(buf);
    ui->viewLabel->setPixmap(pix.scaled(ui->viewLabel->width(), ui->viewLabel->height(), Qt::KeepAspectRatio));
}


