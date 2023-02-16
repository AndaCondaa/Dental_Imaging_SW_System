/*
 * 프로그램명 : ImagingSW
 * 파일명 : controlpanel.h
 * 설명 : CT 장비 제어동작 구현
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.02.13
 */

#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>

class QAbstractButton;
class QButtonGroup;

namespace Ui {
class ControlPanel;
}

class ControlPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ControlPanel(QWidget *parent = nullptr);
    ~ControlPanel();

private slots:
    void checkTypeButton(QString);                  // 촬영타입에 따른 버튼 조작
    void controlButtonClicked(QAbstractButton*);    // 제어명령 요청에 따른 패킷 전송
    void receiveButtonControl(int);                 // 제어명령 패킷 수신

    void resetControl();                        // RESET 동작
    void readyControl();                        // READY 동작
    void startControl();                        // START 동작
    void stopControl();                         // STOP 동작
    void shootingEndSlot(QString);              // 촬영완료 동작 (PANO, CEPH)
    void saveSlot(QString);                     // SAVE 동작 후 버튼 리셋
    void finishSlot(QString, QString);          // 특정 환자의 촬영프로세스 종료

private:
    void settingStyleSheet();                   // GUI 스타일시트 설정

    Ui::ControlPanel *ui;

    QString requestType = "NULL";               // 요청된 촬영 타입 (BOTH, PANO, CEPH)
    QString currentPID = "NULL";                // 현재 촬영중인 환자번호
    QString currentType = "NULL";               // 촬영하려는 타입 (PANO, CEPH)

    QButtonGroup *modeButtonGroup;              // 촬영타입 버튼 그룹
    QButtonGroup *controlButtonGroup;           // 제어명령 버튼 그룹

signals:
    void buttonSignal(int, QString);            // 제어명령 패킷 송신 요청
    void readySignal(QString);                  // 촬영준비 시, 촬영타입 전송
    void startSignal(QString, QString);         // 촬영시작 시, 프레임데이터 수신용 쓰레드 준비 요청
    void stopSignal();                          // 촬영중단 시, 프레임데이터 수신용 쓰레드 중지 요청
};

#endif // CONTROLPANEL_H
