/*
 * 프로그램명 : ImagingSW
 * 파일명 : imagethread.cpp
 * 설명 : 프레임데이터 수신 후 화면에 출력하는 스레드 구현
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.02.13
 */

#include "imagethread.h"

#include <QPainter>
#include <QPixmap>

#include <opencv2/opencv.hpp>

ImageThread::ImageThread(int width, int height, QString modeType, QObject *parent)
    : QThread{parent}
{
    this->width = width;
    this->height = height;
    this->modeType = modeType;
    checkCount = new bool[1750];
    memset(checkCount, false, 1750);
}

void ImageThread::setCount(int count)
{
    this->count = count;
    if (count < maxCount) {
        checkCount[count] = true;
    }
}

void ImageThread::threadStop()
{
    isStop = true;
}

void ImageThread::run()
{
    QPixmap pixmap(width, height);
    pixmap.fill(Qt::white);

    QPainter painter(&pixmap);

    FILE *file;
    QString fileName;

    int valueMax = 65535;
    int rows, cols, pixels, currentCount = 0;

    // PANO
    if (modeType == "PANO") {
        rows = 64;
        cols = 1152;
        pixels = rows * cols;
        maxCount = 1750;
        unsigned short *buf = new unsigned short[pixels];

        do {
            if (isStop) {
                delete[] buf;
                delete[] checkCount;
                return;
            }
            if (checkCount[currentCount] == true) {

                if (currentCount >= 1000)
                    fileName = QString("./image/frame/%1/%2.raw").arg(modeType).arg(currentCount);
                else if (currentCount < 1000 && currentCount >= 100)
                    fileName = QString("./image/frame/%1/0%2.raw").arg(modeType).arg(currentCount);
                else if (currentCount < 100 && currentCount >= 10)
                    fileName = QString("./image/frame/%1/00%2.raw").arg(modeType).arg(currentCount);
                else
                    fileName = QString("./image/frame/%1/000%2.raw").arg(modeType).arg(currentCount);

                fopen_s(&file, fileName.toStdString().c_str(), "rb");

                if (file == nullptr) {
                    delete[] buf;
                    return;
                }

                memset(buf, 0, pixels);
                fread(buf, sizeof(unsigned short), pixels, file);
                fclose(file);

                for (int i = 0; i < pixels; i++) {
                    if (buf[i] * 5 > valueMax)
                        buf[i] = valueMax;
                    else
                        buf[i] *= 5;
                }

                cv::Mat mat(rows, cols, CV_16UC1, buf);
                cv::Mat rotateMat;
                rotate(mat, rotateMat, cv::ROTATE_90_CLOCKWISE);

                double transHeight = width / 2.;
                double offset = (height - transHeight) / 2.;    // PANO 영상 비율 2:1

                QImage frameImage(rotateMat.data, rotateMat.cols, rotateMat.rows , QImage::Format_Grayscale16);
                painter.drawImage(width - currentCount*((double)width/1750.), offset, frameImage.scaledToHeight(transHeight));
                emit processFinished(pixmap);
                emit processCount(currentCount);
                currentCount++;
                if (currentCount == maxCount) {
                    isStop = true;
                    currentCount = 0;
                }
            }
        } while(!isStop);

        delete[] buf;
        delete[] checkCount;
    } else if (modeType == "CEPH") {
        rows = 2400;
        cols = 48;
        pixels = rows * cols;
        maxCount = 1250;
        unsigned short *buf = new unsigned short[pixels];

        do {
            if (isStop) {
                delete[] buf;
                delete[] checkCount;
                return;
            }
            if (checkCount[currentCount] == true) {
                if (currentCount >= 1000)
                    fileName = QString("./image/frame/%1/%2.raw").arg(modeType).arg(currentCount);
                else if (currentCount < 1000 && currentCount >= 100)
                    fileName = QString("./image/frame/%1/0%2.raw").arg(modeType).arg(currentCount);
                else if (currentCount < 100 && currentCount >= 10)
                    fileName = QString("./image/frame/%1/00%2.raw").arg(modeType).arg(currentCount);
                else
                    fileName = QString("./image/frame/%1/000%2.raw").arg(modeType).arg(currentCount);

                fopen_s(&file, fileName.toStdString().c_str(), "rb");

                if (file == nullptr) {
                    delete[] buf;
                    return;
                }

                memset(buf, 0, pixels);
                fread(buf, sizeof(unsigned short), pixels, file);
                fclose(file);

                for (int i = 0; i < pixels; i++) {
                    if (buf[i] * 100 > valueMax)
                        buf[i] = valueMax;
                    else
                        buf[i] *= 100;
                }

                cv::Mat mat(rows, cols, CV_16UC1, buf);
                cv::Mat dst;
                cv::rotate(mat, dst, cv::ROTATE_180);

                double transWidth = (double)(height * 5) / 4.;
                double offset = width - ((width - transWidth) / 2.);    // CEPH 영상 비율 5:4

                QImage frameImage(dst.data, dst.cols, dst.rows, QImage::Format_Grayscale16);
                painter.drawImage(offset - ((currentCount+1) * ((transWidth)/1250.)), 0, frameImage.scaledToHeight(height));
                emit processFinished(pixmap);
                emit processCount(currentCount);
                currentCount++;
                if (currentCount == maxCount) {
                    isStop = true;
                    currentCount = 0;
                }
            }
        } while (!isStop);

        delete[] buf;
        delete[] checkCount;
    }
}
