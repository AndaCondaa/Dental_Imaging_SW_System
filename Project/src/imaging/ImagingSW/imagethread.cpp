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

    // CEPH
    if (modeType == "CEPH") {
        rows = 2400;
        cols = 48;
        pixels = rows * cols;
        buf = (unsigned short*)malloc(sizeof(unsigned short) * pixels);

        for(int k = 876; k > 10; k--) {
            /*
            QString fileName = QString("./image/origin/CEPH/0%1.raw").arg(i);
            QFile file(fileName);
            file.open(QIODevice::ReadOnly);
            QByteArray array = file.readAll();
            file.close();

            uchar* data = (uchar*)array.data();

            Mat mat(rows, cols, CV_16UC1, data);
            Mat beforeMat(rows, cols, CV_8UC1);
            mat.convertTo(beforeMat, CV_8UC1);
            Mat his(rows, cols, CV_8UC1);
            equalizeHist(beforeMat, his);

            QImage frameImage(his.data, his.cols, his.rows, his.step, QImage::Format_Grayscale8);
            painter.drawImage(cnt*(0.2), 0, frameImage.scaledToHeight(height));
            */
            if (k >= 100)
                fileName = QString("./image/origin/CEPH/0%1.raw").arg(k);
            else
                fileName = QString("./image/origin/CEPH/00%1.raw").arg(k);

            qDebug() << fileName;
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

            for (int i = 0; i < pixels; i++) {
                tmp = buf[i];
                if (min > tmp)
                    min = tmp;
                else if (max < tmp)
                    max = tmp;
            }

            range = max - min;
            for (int i = 0; i < pixels; i++) {
                buf[i] = (unsigned short)(((double)(buf[i] - min) / (double)(range)) * valueMax);
            }

//            int hist[sizeof(unsigned short)];
//            int sum[sizeof(unsigned short)];
//            memset(hist, 0, sizeof(unsigned short));
//            memset(sum, 0, sizeof(unsigned short));
//            for (int i = 0; i < pixels; i++) {
//                hist[buf[i]]++;
//            }
//            for (int i = 0; i < 65535; i++) {
//                for (int j = 0; j <= i; j++) {
//                    sum[i] += hist[j];
//                }
//                sum[i] = (unsigned short)(((double)sum[i] / pixels) * 65535);
//                qDebug("%d : %d", i, sum[i]);
//            }
//            for (int i = 0; i < pixels; i++) {
//                buf[i] = sum[buf[i]];
//            }

            Mat mat(rows, cols, CV_16UC1, buf);
            Mat dst;
            flip(mat, dst, 0);

            QImage frameImage(dst.data, dst.cols, dst.rows, QImage::Format_Grayscale16);
            painter.drawImage((width/5) + cnt*((double)(width*3/5)/900), 0, frameImage.scaledToHeight(height));

            emit imageProgressed(cnt);
            emit processFinished(pixmap);
            cnt++;
        }
        delete buf;
    } else if (modeType == "PANO") {
        rows = 64;
        cols = 1152;
        pixels = rows * cols;
        buf = (unsigned short*)malloc(sizeof(unsigned short) * pixels);
        for(int k = 110; k < 1700; k++) {
            if (k < 1000) {
                fileName = QString("./image/origin/PANO/0%1.raw").arg(k);
            } else {
                fileName = QString("./image/origin/PANO/%1.raw").arg(k);
            }

            qDebug() << fileName;
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
            painter.drawImage(cnt*((double)width/1590), height/8, frameImage.scaledToHeight(height*3/4));
            emit imageProgressed(cnt);
            emit processFinished(pixmap);
            cnt++;
        }
        delete buf;
    }

}


