#ifndef PATIENTMANAGER_H
#define PATIENTMANAGER_H

#include <QWidget>
#include <QTreeWidget>

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
    void receiveWaitPatient(QStringList dataList);      // 촬영의뢰 받은 경우
    void deletePatient();                               // 대기환자 목록에서 삭제
    void readyButtonSlot();                            // 환자준비 버튼을 눌러서, 서버에 환자 정보 요청하는 함수
    void receivePatientInfo(QStringList dataList);      // 서버로부터 환자 정보를 받는 함수
    void waitDoubleClicked(QTreeWidgetItem*, int);       // 대기명단 고객을 더블클릭
    void saveSlot(QString);
    void finishButtonSlot();

private:
    Ui::PatientManager *ui;

    QString shootingStatus = "CEPH";                    // 현재 환자가 어느 사진을 찍었는지 확인하는 지표 (NULL, PANO, CEPH, BOTH)
    QMap<QString, QString> typeMap;                     // <pid, type>


signals:
    void sendPid(QString);
    void sendType(QString);
    void sendPidToImagingManager(QString);
    void finishSignal(QString);                     // currentPID

};

#endif // PATIENTMANAGER_H
