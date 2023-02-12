/*
 * 프로그램명 : ImagingSW
 * 파일명 : imagethread.h
 * 설명 : 프레임데이터 수신 후 화면에 출력하는 스레드 구현
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.02.13
 */

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

    int count = 0;
    int currentCount = 0;
    int maxCount = 0;

    bool isStop = false;
    bool *checkCount;

    int width;
    int height;
    QString modeType;

signals:
    void processCount(int);
    void processFinished(const QPixmap&);
};

#endif // IMAGETHREAD_H
