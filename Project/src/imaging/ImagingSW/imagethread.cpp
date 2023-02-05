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

void ImageThread::threadStop()
{
    qDebug("%s %d", __FUNCTION__, __LINE__);
    isStop = true;
}

void ImageThread::run()
{
    QPixmap pixmap(width, height);
    pixmap.fill(Qt::white);

    QPainter painter(&pixmap);

    FILE *file;
    QString fileName;
    unsigned short *buf;
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
        buf = (unsigned short*)malloc(sizeof(unsigned short) * pixels);

        for(int k = 0; k < 1750; k++) {
            if (isStop) return;

            if (k >= 1000)
                fileName = QString("./PANO/%1.raw").arg(k);
            else if (k < 1000 && k >= 100)
                fileName = QString("./PANO/0%1.raw").arg(k);
            else if (k < 100 && k >= 10)
                fileName = QString("./PANO/00%1.raw").arg(k);
            else
                fileName = QString("./PANO/000%1.raw").arg(k);

            file = fopen(fileName.toStdString().c_str(), "rb");
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
            painter.drawImage(cnt*((double)width/1750.), 0, frameImage.scaled(cvRound((double)width/1750.), cvRound((double)width/1750.)*1750/2.5));
            emit imageProgressed(cnt);
            emit processFinished(pixmap);
            cnt++;
        }
    } if (modeType == "CEPH") {
        rows = 2400;
        cols = 48;
        pixels = rows * cols;
        buf = (unsigned short*)malloc(sizeof(unsigned short) * pixels);

        for(int k = 1249; k >= 0; k--) {
            if (isStop) return;

            if (k >= 1000)
                fileName = QString("./CEPH/%1.raw").arg(k);
            else if (k < 1000 && k >= 100)
                fileName = QString("./CEPH/0%1.raw").arg(k);
            else if (k < 100 && k >= 10)
                fileName = QString("./CEPH/00%1.raw").arg(k);
            else
                fileName = QString("./CEPH/000%1.raw").arg(k);

            file = fopen(fileName.toStdString().c_str(), "rb");
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
        }

    }
    qDebug("width: %d", width);
    qDebug("height : %d", height);
    delete buf;
}



