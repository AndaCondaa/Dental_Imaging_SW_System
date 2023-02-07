#include "imagethread.h"

#include <QPainter>
#include <qdebug.h>
#include <stdio.h>
#include <QFile>

#include <opencv2/opencv.hpp>

using namespace cv;

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

//void ImageThread::run()
//{
//    qDebug("%d", __LINE__);
//    QPixmap pixmap(width, height);
//    pixmap.fill(Qt::white);

//    QPainter painter(&pixmap);

//    FILE *file;
//    QString fileName;
//    unsigned short *buf;
//    int valueMax = 65535;
//    int min = 65535;
//    int max = 0;
//    int tmp, range, rows, cols, pixels;

//    // PANO
//    if (modeType == "PANO") {
//        rows = 64;
//        cols = 1152;
//        pixels = rows * cols;
//        buf = (unsigned short*)malloc(sizeof(unsigned short) * pixels);

//        if (isStop) return;
//        while(count == 1700) {
//            if (before == count) {
//                qDebug("BEFORE == COUNT");
//            } else {
//                if (count >= 1000)
//                    fileName = QString("./PANO/%1.raw").arg(count);
//                else if (count < 1000 && count >= 100)
//                    fileName = QString("./PANO/0%1.raw").arg(count);
//                else if (count < 100 && count >= 10)
//                    fileName = QString("./PANO/00%1.raw").arg(count);
//                else
//                    fileName = QString("./PANO/000%1.raw").arg(count);

//                file = fopen(fileName.toStdString().c_str(), "rb");
//                memset(buf, 0, pixels);
//                fread(buf, sizeof(unsigned short), pixels, file);
//                fclose(file);

//                for (int i = 0; i < pixels; i++) {
//                    if (buf[i] * 12 > valueMax)
//                        buf[i] = valueMax;
//                    else
//                        buf[i] *= 12;
//                }

//                for (int i = 0; i < pixels; i++) {
//                    tmp = buf[i];
//                    if (min > tmp)
//                        min = tmp;
//                    else if (max < tmp)
//                        max = tmp;
//                }

//                range = max - min;
//                for (int i = 0; i < pixels; i++) {
//                    buf[i] = ~(unsigned short)(((double)(buf[i] - min) / (double)(range)) * valueMax);
//                }
//                Mat mat(rows, cols, CV_16UC1, buf);
//                Mat rotateMat;
//                rotate(mat, rotateMat, cv::ROTATE_90_CLOCKWISE);
//                QImage frameImage(rotateMat.data, rotateMat.cols, rotateMat.rows , QImage::Format_Grayscale16);
//                painter.drawImage(count*((double)width/1750.), 0, frameImage.scaled(cvRound((double)width/1750.), cvRound((double)width/1750.)*1750/2.5));
//                emit imageProgressed(count);
//                emit processFinished(pixmap);
//                before = count;
//            }
//        }
//    } if (modeType == "CEPH") {
//        rows = 2400;
//        cols = 48;
//        pixels = rows * cols;
//        buf = (unsigned short*)malloc(sizeof(unsigned short) * pixels);


//            if (isStop) return;

//            if (count >= 1000)
//                fileName = QString("./CEPH/%1.raw").arg(count);
//            else if (count < 1000 && count >= 100)
//                fileName = QString("./CEPH/0%1.raw").arg(count);
//            else if (count < 100 && count >= 10)
//                fileName = QString("./CEPH/00%1.raw").arg(count);
//            else
//                fileName = QString("./CEPH/000%1.raw").arg(count);

//            file = fopen(fileName.toStdString().c_str(), "rb");
//            memset(buf, 0, pixels);
//            fread(buf, sizeof(unsigned short), pixels, file);
//            fclose(file);

//            for (int i = 0; i < pixels; i++) {
//                if (buf[i] * 100 > valueMax)
//                    buf[i] = valueMax;
//                else
//                    buf[i] *= 100;
//            }

//            Mat mat(rows, cols, CV_16UC1, buf);
//            Mat dst;
//            flip(mat, dst, 0);

//            QImage frameImage(dst.data, dst.cols, dst.rows, QImage::Format_Grayscale16);
//            painter.drawImage(count*((double)(width)/1250.), 0, frameImage.scaledToHeight(height));

//            emit imageProgressed(count);
//            emit processFinished(pixmap);
//    }
//    qDebug("width: %d", width);
//    qDebug("height : %d", height);
//    delete buf;
//}


void ImageThread::run()
{
    QPixmap pixmap(width, height);
    pixmap.fill(Qt::white);

    QPainter painter(&pixmap);

    FILE *file;
    QString fileName;

    int cnt = 0;
    int valueMax = 65535;
    int min = 65535;
    int max = 0;
    int tmp, range, rows, cols, pixels;

    // PANO
    if (modeType == "PANO") {
        rows = 64;
        cols = 1152;
        pixels = rows * cols;
        unsigned short *buf = new unsigned short[pixels];


        for(int k = 0; k < 1750; k++) {
            if (isStop) return;

            if (k >= 1000)
                fileName = QString("./image/frame/%1/%2.raw").arg(modeType).arg(k);
            else if (k < 1000 && k >= 100)
                fileName = QString("./image/frame/%1/0%2.raw").arg(modeType).arg(k);
            else if (k < 100 && k >= 10)
                fileName = QString("./image/frame/%1/00%2.raw").arg(modeType).arg(k);
            else
                fileName = QString("./image/frame/%1/000%2.ra").arg(modeType).arg(k);

            fopen_s(&file, fileName.toStdString().c_str(), "rb");

            if (file == nullptr) {
                qDebug() << "파일 오픈 오류!";
                return;
            }

            memset(buf, 0, pixels);
            fread(buf, sizeof(unsigned short), pixels, file);
            fclose(file);

            for (int i = 0; i < pixels; i++) {
                if (buf[i] * 12 > valueMax)
                    buf[i] = valueMax;
                else
                    buf[i] *= 12;
            }

            for (int i = 0; i < pixels; i++) {
                tmp = buf[i];
                if (min > tmp)
                    min = tmp;
                else if (max < tmp)
                    max = tmp;
            }

            range = max - min;
            for (int i = 0; i < pixels; i++) {
                buf[i] = ~(unsigned short)(((double)(buf[i] - min) / (double)(range)) * valueMax);
            }
            Mat mat(rows, cols, CV_16UC1, buf);
            Mat rotateMat;
            rotate(mat, rotateMat, cv::ROTATE_90_CLOCKWISE);
            QImage frameImage(rotateMat.data, rotateMat.cols, rotateMat.rows , QImage::Format_Grayscale16);
            painter.drawImage(cnt*((double)width/1750.), 0, frameImage.scaledToHeight(height));
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


        for(int k = 1249; k >= 0; k--) {
            if (isStop) return;

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

            Mat mat(rows, cols, CV_16UC1, buf);
            Mat dst;
            flip(mat, dst, 0);

            QImage frameImage(dst.data, dst.cols, dst.rows, QImage::Format_Grayscale16);
            painter.drawImage(cnt*((double)(width)/1250.), 0, frameImage.scaledToHeight(height));

            emit imageProgressed(cnt);
            emit processFinished(pixmap);
            cnt++;
            qDebug("%d", cnt);
        }
        delete[] buf;
    }
    qDebug("width: %d", width);
    qDebug("height : %d", height);
}

