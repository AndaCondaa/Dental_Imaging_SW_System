/*
 * 프로그램명 : ImagingSW
 * 파일명 : patientmanager.h
 * 설명 : 대기 환자 및 촬영 중인 환자 정보 관리
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.02.13
 */


#ifndef PATIENTMANAGER_H
#define PATIENTMANAGER_H

#include <QWidget>

class QTreeWidgetItem;

namespace Ui {
class PatientManager;
}

class PatientManager : public QWidget
{
    Q_OBJECT

public:
    explicit PatientManager(QWidget *parent = nullptr);
    ~PatientManager();

private slots:
    void receiveWaitList(int, QString);                 // 첫 연결 시, 이전 대기목록 리시브
    void receiveWaitPatient(QStringList dataList);      // 촬영요청 데이터 리시브
    void readyButtonSlot();                             // 환자준비 버튼 슬롯 (환자정보 요청)
    void receivePatientInfo(QStringList dataList);      // 요청한 환자정보 리시브
    void waitDoubleClicked(QTreeWidgetItem*, int);      // 대기명단 더블클릭 슬롯
    void saveSlot(QString);                             // 재구성 영상 저장 시, 현재 촬영상태 변경
    void finishButtonSlot();                            // 촬영완료 버튼 슬롯 (완료 상황 패킷전송)

private:
    void settingStyleSheet();                           // 스타일시트 설정

    Ui::PatientManager *ui;

    QMap<QString, QString> typeMap;                     // 환자PID:촬영요청타입(BOTH,PANO,CEPH)
    QString shootingStatus = "NULL";                    // 현재 환자의 촬영상태 확인 (NULL, PANO, CEPH, BOTH)
                                                        // -> 요청한 촬영을 모두 수행했는지 확인

signals:
    void sendPid(QString);                              // 정보요청하는 환자의 PID전송
    void sendType(QString);                             // 모드버튼 조작용 촬영타입 전송
    void sendPidToImagingManager(QString);              // 촬영환자 PID 전송
    void finishSignal(QString,QString);                 // 촬영완료 버튼 시그널
};

#endif // PATIENTMANAGER_H
