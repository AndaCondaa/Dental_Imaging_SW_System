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

//private slots:
public slots:
    void receiveWaitList(int, QString);     // 서버와 연결할 때, 기존 대기목록 리시브
    void receiveWaitPatient(QStringList dataList);      // 촬영의뢰 받은 경우
    void readyButtonSlot();                            // 환자준비 버튼을 눌러서, 서버에 환자 정보 요청하는 함수
    void receivePatientInfo(QStringList dataList);      // 서버로부터 환자 정보를 받는 함수
    void waitDoubleClicked(QTreeWidgetItem*, int);       // 대기명단 고객을 더블클릭
    void saveSlot(QString);
    void finishButtonSlot();

private:
    void settingStyleSheet();

    Ui::PatientManager *ui;

    QString shootingStatus = "NULL";                    // 현재 환자가 어느 사진을 찍었는지 확인하는 지표 (NULL, PANO, CEPH, BOTH)
    QMap<QString, QString> typeMap;                     // <pid, type>

signals:
    void sendPid(QString);
    void sendType(QString);
    void sendPidToImagingManager(QString);
    void deleteSignal(QString);                          // 환자준비를 한 후에 삭제한 경우, control클래스에 시그널 전송 (for currentPID 재설정)
    void finishSignal(QString, QString);                     // currentPID , Type
};

#endif // PATIENTMANAGER_H
