#ifndef IMAGETHREAD_H
#define IMAGETHREAD_H

#include <QThread>

class ImageThread : public QThread
{
    Q_OBJECT

public:
    explicit ImageThread(int width, int height, QString modeType, QObject *parent = nullptr);

public slots:
    void threadStop();

private:
    void run();

    bool isStop = false;

    int width;
    int height;
    QString modeType;

signals:
    void imageProgressed(int);
    void processFinished(const QPixmap &);
};

#endif // IMAGETHREAD_H
