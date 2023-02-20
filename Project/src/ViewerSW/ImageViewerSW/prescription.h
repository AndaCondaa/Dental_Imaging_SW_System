/*프로그램명 : ImageViewerSW
파일명 : prescription.h
설명 : 서버에서 받아온 환자 정보들을 treeWidget과 tableWidget을 통해 대기 환자 리스트와 환자 정보로 구성하였고
진료 시작 버튼, 촬영 의뢰 버튼을 통해 타 SW들과 연동되어 있는 클래스
작성자 : 이정연
최종 수정 날짜 : 2023.02.11*/

#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H

#include <QWidget>
#include <QMessageBox>

namespace Ui {
class Prescription;
}

class Prescription : public QWidget
{
    Q_OBJECT

public:
    explicit Prescription(QWidget *parent = nullptr);
    ~Prescription();

private:
    Ui::Prescription *ui;
    QString DoctorID;
    QString DoctorName;
    QString PatientID;
    QString PatientName;
    QString PatientSex;
    QString CurrentDate;
    QMessageBox* messagebox;

private slots:
    void on_Finish_clicked();
    void receivePrescriptionYesOrNot(QString);
    void receivePrescription(QString, QString, QString, QString, QString);

signals:
    void sendPrescriptionFinish(QString);
};

#endif // PRESCRIPTION_H
