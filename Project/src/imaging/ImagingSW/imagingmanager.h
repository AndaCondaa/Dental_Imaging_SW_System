/*
 * 프로그램명 : ImagingSW
 * 파일명 : imagingmanager.h
 * 설명 : 이미지 정합 및 저장 구현
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.02.13
 */

#ifndef IMAGINGMANAGER_H
#define IMAGINGMANAGER_H

#include <QWidget>

class ImageThread;

namespace Ui {
class ImagingManager;
}

class ImagingManager : public QWidget
{
    Q_OBJECT

public:
    explicit ImagingManager(QWidget *parent = nullptr);
    ~ImagingManager();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void setPID(QString);                   // 현재 PID 저장
    void setType(QString);                  // 현재 촬영타입 저장

    void startSetting(QString pid, QString type);       // 프레임데이터 display 스레드 실행
    void recvFrameImg(int);                             // 프레임데이터 전송
    void stopButtonSlot();                              // 스레드 종료
    void isProgressMaximum(int);                        // ProgressBar Maimum 체크

    void saveButtonSlot();                  // 저장파일 정보
    void reconImage();                      // 영상 재구성



    // 영상 재구성 관련 함수
    QString makeFileName(QString type, int count);          // 파일이름 생성
    void histoStretch(unsigned short *input, int inputSize, int min, int max, double maxValue);     // 히스토그램 스트레칭
    void CLAHE(unsigned short *input, int rows, int cols, double clipLimit, int gridX, int gridY);  // CLAHE 적용(OpenCV)
    void medianFilter(unsigned short *input, int rows, int cols, int ksize);                        // Median 필터
    void gammaCorrection(unsigned short *input, int inputSize, double maxValue, double gamma);      // 감마보정
    void unsharpFilter(unsigned short *input, int rows, int cols, double maxValue);                 // Unsharp 필터
    void invertImage(unsigned short *input, int inputSize);                                         // 이미지 반전
    void saveAsJpg(unsigned short *input, int rows, int cols);                                      // Jpg로 저장
    void viewReconImage(unsigned short *input, int rows, int cols);                                 // 재구성영상 출력

private:
    void settingStyleSheet();               // 스타일시트 설정

    Ui::ImagingManager *ui;

    QString currentPID = "NULL";            // 현재 촬영중인 환자번호
    QString currentType = "NULL";           // 촬영하려는 타입 (PANO, CEPH)

    ImageThread *thread;                    // 사용자 정의 스레드 클래스

signals:
    void sendCount(int);                    // 전송받은 프레임데이터 카운트 전송 to thread
    void shootingEndSignal(QString);        // 촬영종료 시 발생 to ControlPanel
    void saveSignal(QString);               // 저장버튼 클릭 시 발생 to MainNetworkManager, ControlPanel
};

#endif // IMAGINGMANAGER_H
