/*
 * 프로그램명 : ImagingSW
 * 파일명 : imagingmanager.cpp
 * 설명 : 이미지 정합 및 저장 구현
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.02.13
 */

#include "imagingmanager.h"
#include "ui_imagingmanager.h"
#include "imagethread.h"

#include <QProgressDialog>
#include <QMessageBox>

#include <opencv2/opencv.hpp>
#include <math.h>

ImagingManager::ImagingManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImagingManager)
{
    ui->setupUi(this);
    settingStyleSheet();

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

void ImagingManager::startSetting(QString pid, QString type)
{
    if (type == "PANO") {
        ui->progressBar->setMaximum(1748);
    } else if (type == "CEPH") {
        ui->progressBar->setMaximum(1248);
    }

    thread = new ImageThread(ui->viewLabel->width(), ui->viewLabel->height(), currentType, this);
    connect(this, SIGNAL(sendCount(int)), thread, SLOT(setCount(int)), Qt::QueuedConnection);
    connect(thread, SIGNAL(processCount(int)), ui->progressBar, SLOT(setValue(int)));
    connect(thread, SIGNAL(processFinished(const QPixmap&)), ui->viewLabel, SLOT(setPixmap(const QPixmap&)));
    thread->start();
}

void ImagingManager::recvFrameImg(int count)
{
    emit sendCount(count);
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

    ui->progressBar->setValue(0);
    ui->reconButton->setEnabled(false);
    ui->saveButton->setEnabled(false);
    ui->viewLabel->clear();
}

void ImagingManager::reconImage()
{
    FILE *file;
    QString fileName;
    double maxValue = 65535.;
    int count = 0;
    int frameRows, frameCols;
    int reconRows, reconCols;

    if (currentType == "PANO") {
        frameRows = 64;
        frameCols = 1152;

        reconRows = 1152;
        reconCols = 1623;

        unsigned short *buf = new unsigned short[frameRows*frameCols];
        unsigned short *tmpOut = new unsigned short[reconRows*reconCols];
        memset(tmpOut, 0, reconRows*reconCols);

        for (int k = 1679; k >= 120; k--) { //1560
            memset(buf, 0, frameRows*frameCols);
            fileName = makeFileName(currentType, k);

            fopen_s(&file, fileName.toStdString().c_str(), "rb");
            if (file == nullptr) {
                qDebug() << "파일 오픈 오류";
                delete[] buf;
                delete[] tmpOut;
                return;
            }

            fread(buf, sizeof(unsigned short), frameRows*frameCols, file);
            fclose(file);

            // 이미지 스티칭
            for (int y = 0; y < frameCols; y++) {
                for (int x = 0; x < frameRows; x++) {
                    tmpOut[(count+x)+(y*reconCols)] = buf[(y)+(x*frameCols)];
                }
            }

            count++;
            qDebug("%d", count);
        }

        cv::Mat src(reconRows, reconCols, CV_16UC1, tmpOut);
        reconRows = 1152;
        reconCols = 2320;
        cv::Mat dst(reconRows, reconCols, CV_16UC1);
        cv::resize(src, dst, dst.size(), 0, 0, cv::INTER_NEAREST);

        unsigned short *out = new unsigned short[reconRows*reconCols];
        memcpy(out, dst.data, reconRows*reconCols*2);

        histoStretch(out, reconRows*reconCols, 0, 10000, maxValue);
        invertImage(out, reconRows*reconCols);
        gammaCorrection(out, reconRows*reconCols, maxValue, 2);
        CLAHE(out, reconRows, reconCols, 40.0, 8, 8);
        medianFilter(out, reconRows, reconCols, 3);
        unsharpFilter(out, reconRows, reconCols, maxValue);
        medianFilter(out, reconRows, reconCols, 3);
        medianFilter(out, reconRows, reconCols, 3);


        FILE *file;
        fopen_s(&file, "./RECON.raw", "wb");
        fwrite(out, sizeof(unsigned short), reconRows*reconCols, file);
        fclose(file);

        viewReconImage(out, reconRows, reconCols);
        saveAsJpg(out, reconRows, reconCols);

        delete[] buf;
//        delete[] tmpOut;
        delete[] out;
    } else if (currentType == "CEPH") {
        frameRows = 2400;
        frameCols = 48;

        reconRows = 2400;
        reconCols = 2500;

        unsigned short *buf = new unsigned short[frameRows*frameCols];
        unsigned short *tmpOut = new unsigned short[reconRows*reconCols];
        memset(tmpOut, 0, reconRows*reconCols*2);

        for (int k = 1229; k > 2; k--) {
            memset(buf, 0, frameRows*frameCols);
            fileName = makeFileName(currentType, k);

            fopen_s(&file, fileName.toStdString().c_str(), "rb");
            if (file == nullptr) {
                qDebug() << "파일 오픈 오류";
                delete[] buf;
                delete[] tmpOut;
                return;
            }
            fread(buf, sizeof(unsigned short), frameRows*frameCols, file);
            fclose(file);

            // 이미지 스티칭
            for (int y = 0; y < reconRows; y++) {
                for (int x = 0; x < frameCols; x++) {
//                    double weight;
//                    if (x < 24)
//                        weight = ((x % 24)+1) * (0.04);
//                    else
//                        weight = 0.96 - ((x % 24)) * (0.04);
//                    tmpOut[(count*2+x)+(y*reconCols)] =
//                            ((double)tmpOut[(count*2+x)+(y*reconCols)] * (1.0 - weight))+ ((double)buf[x+((frameRows-y-1)*frameCols)] * weight);
                    tmpOut[(count*2+x)+(y*reconCols)] = buf[x+((frameRows-y-1)*frameCols)];
                }
            }
            count++;
            qDebug("%d", count);
            ui->progressBar->setValue((double)(ui->progressBar->maximum() / 5.) * ((double)k/11197.0));
        }

        cv::Mat src(reconRows, reconCols, CV_16UC1, tmpOut);
        reconRows = 2400;
        reconCols = 3000;
        cv::Mat dst(reconRows, reconCols, CV_16UC1);
        cv::resize(src, dst, dst.size(), 0, 0, cv::INTER_NEAREST);

        unsigned short *out = new unsigned short[reconRows*reconCols];
        memcpy(out, dst.data, reconRows*reconCols*2);

        histoStretch(out, reconRows*reconCols, 0, 381, maxValue);
        invertImage(out, reconRows*reconCols);

        ui->progressBar->setValue(ui->progressBar->maximum()*2/5.);

        unsigned short *tmp1 = new unsigned short[reconRows*reconCols];
        unsigned short *tmp2 = new unsigned short[reconRows*reconCols];
        memcpy(tmp1, out, reconRows*reconCols*2);
        memcpy(tmp2, out, reconRows*reconCols*2);

        gammaCorrection(tmp1, reconRows*reconCols, maxValue, 15);
        gammaCorrection(tmp2, reconRows*reconCols, maxValue, 0.3);

        CLAHE(tmp1, reconRows, reconCols, 4000.0, 8, 8);
        medianFilter(tmp1, reconRows, reconCols, 3);

        ui->progressBar->setValue(ui->progressBar->maximum()*3/5.);

        for (int i = 0; i < reconRows*reconCols; i++) {
            out[i] = (double)(tmp1[i] * 0.8) + (double)(tmp2[i] * 0.2);
        }

        medianFilter(out, reconRows, reconCols, 5);

        ui->progressBar->setValue(ui->progressBar->maximum()*4/5.);

        saveAsJpg(out, reconRows, reconCols);
        viewReconImage(out, reconRows, reconCols);

        ui->progressBar->setValue(ui->progressBar->maximum());

        delete[] tmp1;
        delete[] tmp2;
        delete[] buf;
        delete[] tmpOut;
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

void ImagingManager::histoStretch(unsigned short *input, int inputSize, int min, int max, double maxValue)
{
    double range = max - min;

    for (int i = 0; i < inputSize; i++) {
        if (input[i] <= max)
            input[i] = cvRound(((double)(input[i] - min) / range) * maxValue);
        else {
            input[i] = maxValue;
        }
    }
}

void ImagingManager::CLAHE(unsigned short *input, int rows, int cols, double clipLimit, int gridX, int gridY)
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
    cv::medianBlur(src, dst, ksize);

    memcpy(input, dst.data, rows*cols*2);
}

void ImagingManager::gammaCorrection(unsigned short *input, int inputSize, double maxValue, double gamma)
{
    for (int i = 0; i < inputSize; i++) {
        input[i] = maxValue * ((double)pow((double)(input[i]) / maxValue, (double)gamma));
    }
}

void ImagingManager::unsharpFilter(unsigned short *input, int rows, int cols, double maxValue)
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
            result[(x-1)+(y-1)*cols] = (sum >= maxValue) ? maxValue : sum;

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

void ImagingManager::viewReconImage(unsigned short *input, int rows, int cols)
{
    cv::Mat mat(rows, cols, CV_16UC1, input);
    QImage image(mat.data, mat.cols, mat.rows , QImage::Format_Grayscale16);
    ui->viewLabel->setPixmap(QPixmap::fromImage(image.scaled(ui->viewLabel->width(), ui->viewLabel->height(), Qt::KeepAspectRatio)));
}

void ImagingManager::settingStyleSheet()
{
    ui->reconButton->setStyleSheet("QPushButton:disabled {"
                                  "background-color: rgb(100, 100, 100);"
                                  "border-radius: 10px;"
                                  "border-style: solid;"
                                  "}"
                                  "QPushButton {"
                                  "background-color: rgb(200, 200, 200);"
                                  "border-radius: 10px;"
                                  "border-style: solid;"
                                  "}");

    ui->saveButton->setStyleSheet("QPushButton:disabled {"
                                  "background-color: rgb(100, 100, 100);"
                                  "border-radius: 10px;"
                                  "border-style: solid;"
                                  "}"
                                  "QPushButton {"
                                  "background-color: rgb(200, 200, 200);"
                                  "border-radius: 10px;"
                                  "border-style: solid;"
                                  "}");
}
