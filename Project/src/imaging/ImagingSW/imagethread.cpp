#include "imagethread.h"

#include <QPainter>
#include <QFile>
#include <qdebug.h>
#include <QLabel>

ImageThread::ImageThread(int width, int height, QObject *parent)
    : QThread{parent}
{
    this->width = width;
    this->height = height;
}

void ImageThread::run()
{
    QPixmap pixmap(width, height);
    pixmap.fill(Qt::white);

    QPainter painter(&pixmap);
    int cnt = 0;
    for(int i = 100; i < 1000; i++)
    {
        QString fileName = QString("./image/0%1.raw").arg(i);
//        QString fileName = QString("./image/0350.raw");
        qDebug() << fileName;
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        QByteArray array = file.readAll();

        file.close();

        uchar* data = (uchar*)array.data();
        QImage frameImage(data, 48, 2400, QImage::Format_RGB555);
//        QLabel * label = new QLabel;
//        label->setPixmap(QPixmap::fromImage(frameImage));
//        label->show();
        painter.drawImage(cnt*(900/width), 0, frameImage.scaledToHeight(height));
//        QPixmap framePixmap = QPixmap::fromImage(frameImage);
//        painter.drawPixmap(48, 0, framePixmap.scaledToHeight(height));
        emit imageProgressed(cnt);
        emit processFinished(pixmap);
        cnt++;
//        sleep(1);
    }
}
