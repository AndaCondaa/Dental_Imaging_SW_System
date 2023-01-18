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
    void slotPatientReady();                            // 환자준비 버튼을 눌러서, 서버에 환자 정보 요청하는 함수
    void receivePatientInfo(QStringList dataList);      // 서버로부터 환자 정보를 받는 함수
    void waitDoubleClicked(QTreeWidgetItem*, int);       // 대기명단 고객을 더블클릭

private:
    Ui::PatientManager *ui;

    QMap<QString, QString> typeMap;

signals:
    void sendPid(QString);
    void sendType(QString);

};

#endif // PATIENTMANAGER_H
