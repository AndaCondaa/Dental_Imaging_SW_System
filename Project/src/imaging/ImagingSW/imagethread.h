#ifndef IMAGETHREAD_H
#define IMAGETHREAD_H

#include <QThread>
#include <QPixmap>
#include <QPainter>

class ImageThread : public QThread
{
    Q_OBJECT

public:
    explicit ImageThread(int width, int height, QString modeType, QObject *parent = nullptr);

public slots:
    void threadStop();
    void setCount(int count);

private:
    void run();

    int before = 0;
    int count = 0;

    bool isStop = false;

    int width;
    int height;
    QString modeType;

signals:
    void imageProgressed(int);
    void processFinished(const QPixmap &);
};

#endif // IMAGETHREAD_H
