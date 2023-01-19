#include "imagethread.h"

#include <QPainter>
#include <qdebug.h>
#include <stdio.h>

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
    QString fileName;
    int min = 65535;
    int max = 0;
    int temp = 0;
    int range = 0;
    int cnt = 0;

    // CEPH
    /*
    for(int i = 100; i < 1000; i++)
    {
        QString fileName = QString("./CEPH/0%1.raw").arg(i);
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        QByteArray array = file.readAll();
        file.close();

        uchar* data = (uchar*)array.data();

        Mat mat(2400, 48, CV_16UC1, data);
        Mat beforeMat(2400, 48, CV_8UC1);
        mat.convertTo(beforeMat, CV_8UC1);
        Mat his(2400, 48, CV_8UC1);
        equalizeHist(beforeMat, his);

        QImage frameImage(his.data, his.cols, his.rows, his.step ,QImage::Format_Grayscale16);
        painter.drawImage(cnt*(0.2), 0, frameImage.scaledToHeight(height));

        emit imageProgressed(cnt);
        emit processFinished(pixmap);
        cnt++;
//        sleep(1);
    }
    */


    // PANO
    int width = 64;
    int height = 1152;
    int pixels = width * height;

    unsigned short *buf;
    unsigned short *trans;
    unsigned char *data;
    buf = (unsigned short*)malloc(sizeof(unsigned short) * pixels);
    trans = (unsigned short*)malloc(sizeof(unsigned short) * pixels);
    data = (unsigned char*)malloc(sizeof(unsigned char) * pixels * 2);

    for(int i = 110; i < 1700; i++)
    {
        memset(buf, 0, pixels);
        memset(data, 0, pixels * 2);

        if (i < 1000) {
            fileName = QString("./image/origin/PANO/0%1.raw").arg(i);
        } else {
            fileName = QString("./image/origin/PANO/%1.raw").arg(i);
        }

        FILE *file;
        file = fopen(fileName.toStdString().c_str(), "rb");

        if (file == nullptr) {
            qDebug() << "fail";
            return;
        }
        fread(buf, sizeof(unsigned short), pixels, file);
        fclose(file);

//        for (int i = 0; i < pixels; i++) {
//            temp = buf[i];
//            if (min > temp)
//                min = temp;
//            else if (max < temp)
//                max = temp;
//        }

//        range = max - min;

        for (int i = 0; i < pixels; i++) {
//            buf[i] = ~(unsigned short)(((double)(buf[i]) / (double)65535) * 65535);
            buf[i] *= 10;
        }

//        for (int i = 0; i < height; i++) {
//            for (int j = 0; j < width; j++) {
//                trans[j+(i*width)] = buf[i+(j*height)];
//            }
//        }

        if (i < 1000) {
            fileName = QString("./image/result/PANO/0%1.raw").arg(i);
        } else {
            fileName = QString("./image/result/PANO/%1.raw").arg(i);
        }

        file = fopen(fileName.toStdString().c_str(), "wb");
//        fwrite(trans, sizeof(unsigned short), pixels, file);
        fwrite(buf, sizeof(unsigned short), pixels, file);
        fclose(file);

        file = fopen(fileName.toStdString().c_str(), "rb");
        fread(data, sizeof(unsigned char), pixels*2, file);
        fclose(file);

        Mat mat(64, 1152, CV_16UC1, data);
        Mat rotateMat;
        rotate(mat, rotateMat, cv::ROTATE_90_CLOCKWISE);

        QImage frameImage(rotateMat.data, rotateMat.cols, rotateMat.rows , QImage::Format_Grayscale16);
        painter.drawImage(cnt*(0.3), 0, frameImage.scaledToHeight(height));
        emit imageProgressed(cnt);
        emit processFinished(pixmap);
        cnt++;
//        sleep(1);
    }

}



