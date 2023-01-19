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
    int min = 65535;
    int max = 0;
    int tmp, range, rows, cols, pixels;

    // CEPH
    if (modeType == "CEPH") {
        rows = 2400;
        cols = 48;
        pixels = rows * cols;
        for(int i = 100; i < 1000; i++) {
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

            QImage frameImage(his.data, his.cols, his.rows, his.step ,QImage::Format_Grayscale16);
            painter.drawImage(cnt*(0.2), 0, frameImage.scaledToHeight(height));

            emit imageProgressed(cnt);
            emit processFinished(pixmap);
            cnt++;
        }
    } else if (modeType == "PANO") {
        rows = 64;
        cols = 1152;
        pixels = rows * cols;
        for(int i = 110; i < 1700; i++) {
            if (i < 1000) {
                fileName = QString("./image/origin/PANO/0%1.raw").arg(i);
            } else {
                fileName = QString("./image/origin/PANO/%1.raw").arg(i);
            }

            file = fopen(fileName.toStdString().c_str(), "rb");

            buf = (unsigned short*)malloc(sizeof(unsigned short) * pixels);

            fread(buf, sizeof(unsigned short), pixels, file);
            fclose(file);

            for (int i = 0; i < 1152 * 64; i++) {
                if (buf[i] * 12 > 65535)
                    buf[i] = 65535;
                else
                    buf[i] *= 12;
            }

            for (int i = 0; i < 1152 * 64; i++) {
                tmp = buf[i];
                if (min > tmp)
                    min = tmp;
                else if (max < tmp)
                    max = tmp;
            }

            range = max - min;
            for (int i = 0; i < pixels; i++) {
                buf[i] = ~(unsigned short)(((double)(buf[i] - min) / (double)(range)) * 65535);
            }

            Mat mat(rows, cols, CV_16UC1, buf);
            Mat rotateMat;
            rotate(mat, rotateMat, cv::ROTATE_90_CLOCKWISE);

            QImage frameImage(rotateMat.data, rotateMat.cols, rotateMat.rows , QImage::Format_Grayscale16);
            painter.drawImage(cnt*(0.3), 0, frameImage.scaledToHeight(height));
            emit imageProgressed(cnt);
            emit processFinished(pixmap);
            cnt++;
        }
    }
}



