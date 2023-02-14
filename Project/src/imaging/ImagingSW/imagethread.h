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

class QPixmap;

class ImageThread : public QThread
{
    Q_OBJECT

public:
    explicit ImageThread(int width, int height, QString modeType, QObject *parent = nullptr);

public slots:
    void threadStop();                      // 스레드 중지 (플래그 값 변경)
    void setCount(int count);               // 프레임데이터 수신 카운팅

private:
    void run();

    int count = 0;                          // 수신된 프레임데이터 개수
    int currentCount = 0;                   // display 완료한 프레임데이터 개수
    int maxCount = 0;                       // 프레임데이터 총 개수 (PANO: 1750, CEPH: 1250)

    bool isStop = false;                    // 스레드 중단 플래그
    bool *checkCount;                       // 프레임데이터 수신 완료 플래그

    int width;                              // 표시할 QLabel 가로길이
    int height;                             // 표시할 QLabel 세로길이
    QString modeType;                       // 현재 촬영모드 (PANO, CEPH)

signals:
    void processCount(int);                 // 진행상황 display (QProgressBar)
    void processFinished(const QPixmap&);   // 프레임데이터 한 장마다 display
};

#endif // IMAGETHREAD_H
