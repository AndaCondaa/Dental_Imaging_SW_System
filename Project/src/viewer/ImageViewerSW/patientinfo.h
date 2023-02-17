/*프로그램명 : ImageViewerSW
파일명 : patientinfo.h
설명 : 서버에서 받아온 환자 정보들을 treeWidget과 tableWidget을 통해 대기 환자 리스트와 환자 정보로 구성하였고
진료 시작 버튼, 촬영 의뢰 버튼을 통해 타 SW들과 연동되어 있는 헤더
작성자 : 이정연
최종 수정 날짜 : 2023.02.11*/

#ifndef PATIENTINFO_H
#define PATIENTINFO_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QMessageBox>

namespace Ui {
class PatientInfo;
}

class PatientInfo : public QWidget
{
    Q_OBJECT

public:
    explicit PatientInfo(QWidget *parent = nullptr);
    ~PatientInfo();

private:
    Ui::PatientInfo *ui;
    QString selectPatientID;
    QString selectPatientName;
    QString patientSex;
    QString patientID;
    QString patientName;
    QMessageBox* messagebox;
    bool Ceph;
    bool Pano;
    bool AllSendImageCheck;

private slots:
    void on_Camerastart_clicked();
    void on_Treatmentstart_clicked();
    void on_WaitingList_itemClicked(QTreeWidgetItem *item, int column);
    void receiveEndSignal();
    void clearWaitingList(int);
    void receiveAllImageFileP(bool);
    void receiveWaitTreatment(int, QString);
    void receivePhotoEnd(QString);
    void receiveEndTreatment(QString);
    void receiveWaitingList(QString, QString);
    void receiveSelectPatient(QString, QString);
    void receivePMSCameraPatient(QString, QString);
    void receiveDoctorName(QString, QString);

signals:
    void sendCameraStart();
    void sendWaitingPatient(QString);
    void sendCameraPatient(QString);
    void sendCameraDelay(QString);
    void sendPatientInfo(QString, QString, QString);
};

#endif // PATIENTINFO_H
