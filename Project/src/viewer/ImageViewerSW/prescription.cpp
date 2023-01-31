#include "prescription.h"
#include "ui_prescription.h"

#include <QDate>

Prescription::Prescription(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Prescription)
{
    ui->setupUi(this);
}

Prescription::~Prescription()
{
    delete ui;
}

//처방전 버튼 클릭 시 의사 정보, 환자 정보가 이미 작성된 채로 띄워지도록 설정
void Prescription::receivePrescription(QString DID, QString DName, QString PID, QString PName, QString PSex)
{
    //오늘 날짜를 자동적으로 입력되게 하기 위한 변수 (형식 : 2023-02-28)
    CurrentDate = QDate::currentDate().toString("yyyy-MM-dd");

    DoctorID = DID;
    DoctorName = DName;
    PatientID = PID;
    PatientName = PName;
    PatientSex = PSex;

    ui->PatientName->setReadOnly(true);
    ui->PatientID->setReadOnly(true);
    ui->DoctorName->setReadOnly(true);
    ui->Date->setReadOnly(true);

    ui->PatientName->setText(PatientName);
    ui->PatientID->setText(PatientID);
    ui->DoctorName->setText(DoctorName);
    ui->Date->setText(CurrentDate);
    ui->PatientSex->setText(PatientSex);
}

//작성 완료 버튼 클릭 시 처방전에 작성된 내용을 서버로 전송
void Prescription::on_Finish_clicked()
{
    QString DID = DoctorID;
    QString DName = DoctorName;
    QString PID = PatientID;
    QString PName = PatientName;
    QString Date = CurrentDate;
    QString Context = ui->textEdit->toPlainText();

    //VNT<CR>PID<CR>환자이름|의사번호|의사이름|진료날짜|진료내용(처방내용)
    QString Data = "VNT<CR>" + PID + "<CR>" + PName + "|" + DID + "|" + DName + "|" + Date + "|" + Context;

    //처방전 작성 완료 시 서버로 해당 데이터를 전송
    emit sendPrescriptionFinish(Data);

    ui->PatientName->clear();
    ui->PatientID->clear();
    ui->textEdit->clear();
}


