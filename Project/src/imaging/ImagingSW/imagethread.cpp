#include "imagethread.h"

#include <QPainter>
#include <QFile>

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
    for(int i = 200; i < 300; i++) {
        QString fileName = QString("./image/0%1.raw").arg(i);

        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        QByteArray array = file.readAll();
        file.close();

        uchar* data = (uchar*)array.data();
        QImage frameImage(data, 48, 2400, QImage::Format_Grayscale16);
        QPixmap framePixmap = QPixmap::fromImage(frameImage);
        framePixmap.scaled(width, height, Qt::KeepAspectRatio);

        painter.drawPixmap((cnt-1)*5, 0, framePixmap);
        emit imageProgressed(cnt);
        emit processFinished(pixmap);
        cnt++;
        sleep(1);
    }
}
