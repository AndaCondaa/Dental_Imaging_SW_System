#include "imagethread.h"

#include <QPainter>
#include <qdebug.h>
#include <stdio.h>
#include <QFile>

#include <opencv2/opencv.hpp>

ImageThread::ImageThread(int width, int height, QString modeType, QObject *parent)
    : QThread{parent}
{
    this->width = width;
    this->height = height;
    this->modeType = modeType;
}

void ImageThread::setCount(int count)
{
    this->count = count;
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

    int cnt = 0;
    int valueMax = 65535;
    int rows, cols, pixels;

    // PANO
    if (modeType == "PANO") {
        rows = 64;
        cols = 1152;
        pixels = rows * cols;
        unsigned short *buf = new unsigned short[pixels];

        for(int k = 0; k < 1750; k++) {
            if (isStop) {
                delete[] buf;
                return;
            }

            if (k >= 1000)
                fileName = QString("./image/frame/%1/%2.raw").arg(modeType).arg(k);
            else if (k < 1000 && k >= 100)
                fileName = QString("./image/frame/%1/0%2.raw").arg(modeType).arg(k);
            else if (k < 100 && k >= 10)
                fileName = QString("./image/frame/%1/00%2.raw").arg(modeType).arg(k);
            else
                fileName = QString("./image/frame/%1/000%2.raw").arg(modeType).arg(k);

            fopen_s(&file, fileName.toStdString().c_str(), "rb");

            if (file == nullptr) {
                qDebug() << "파일 오픈 오류!";
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
            painter.drawImage(width - cnt*((double)width/1750.), offset, frameImage.scaledToHeight(transHeight));
            emit imageProgressed(cnt);
            emit processFinished(pixmap);
            cnt++;
            qDebug("%d", cnt);
        }
        delete[] buf;
    } else if (modeType == "CEPH") {
        rows = 2400;
        cols = 48;
        pixels = rows * cols;
        unsigned short *buf = new unsigned short[pixels];

        for(int k = 0; k < 1169; k++) {
            if (isStop) {
                delete[] buf;
                return;
            }

            if (k >= 1000)
                fileName = QString("./image/frame/%1/%2.raw").arg(modeType).arg(k);
            else if (k < 1000 && k >= 100)
                fileName = QString("./image/frame/%1/0%2.raw").arg(modeType).arg(k);
            else if (k < 100 && k >= 10)
                fileName = QString("./image/frame/%1/00%2.raw").arg(modeType).arg(k);
            else
                fileName = QString("./image/frame/%1/000%2.raw").arg(modeType).arg(k);

            fopen_s(&file, fileName.toStdString().c_str(), "rb");

            if (file == nullptr) {
                qDebug() << "파일 오픈 오류!";
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
            painter.drawImage(offset - ((cnt+1) * ((transWidth)/1250.)), 0, frameImage.scaledToHeight(height));

            emit imageProgressed(cnt);
            emit processFinished(pixmap);
            cnt++;
            qDebug("%d", cnt);
        }
        delete[] buf;
    }
}
