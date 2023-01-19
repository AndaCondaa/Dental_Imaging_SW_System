#include "prescription.h"
#include "ui_prescription.h"

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

void Prescription::receivePrescription(QString DID, QString DName, QString PID, QString PName)
{
    DoctorID = DID;
    DoctorName = DName;
    PatientID = PID;
    PatientName = PName;

    ui->PatientName->setReadOnly(true);
    ui->PatientID->setReadOnly(true);
    ui->DoctorName->setReadOnly(true);
    ui->DoctorID->setReadOnly(true);

    ui->PatientName->setText(PatientName);
    ui->PatientID->setText(PatientID);
    ui->DoctorName->setText(DoctorName);
    ui->DoctorID->setText(DoctorID);


    qDebug() << "처방전" << DoctorID << DoctorName << PatientID << PatientName;
}
