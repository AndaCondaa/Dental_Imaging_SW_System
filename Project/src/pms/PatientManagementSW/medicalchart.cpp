#include "medicalchart.h"
#include "ui_medicalchart.h"

MedicalChart::MedicalChart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MedicalChart)
{
    ui->setupUi(this);
}

MedicalChart::~MedicalChart()
{
    delete ui;
}

void MedicalChart::patientReportInfoSended(QString patientDetail, QString reportDetail)
{
    qDebug() << "patientDetail: " << patientDetail;
    qDebug() << "reportDetail: " << reportDetail;

    QString patientID, patientName, patientSex, patientBirth, patientTel, patientAddress, patientMemo;
    patientID = patientDetail.split("|")[0];
    patientName = patientDetail.split("|")[1];
    patientSex = patientDetail.split("|")[2];
    patientBirth = patientDetail.split("|")[3];
    patientTel = patientDetail.split("|")[4];
    patientAddress = patientDetail.split("|")[5];
    //patientMemo = patientDetail.split("|")[6];

    ui->patientIDLineEdit->setText(patientID);
    ui->patientNameLineEdit->setText(patientName);
    ui->sexLineEdit->setText(patientSex);
    ui->birthLineEdit->setText(patientBirth);
    ui->telLineEdit->setText(patientTel);
    ui->addressPlainTextEdit->setPlainText(patientAddress);

    QString reportID, dentistID, reportDate, patientNote, dentistName;
    reportID = reportDetail.split("|")[0];
    //QString patientID = reportDetail.split("|")[1];
    dentistID = reportDetail.split("|")[2];
    reportDate = reportDetail.split("|")[3];
    patientNote = reportDetail.split("|")[4];
    dentistName = reportDetail.split("|")[5];

    ui->reportNoLabel->setText(reportID);
    ui->dentistIDLineEdit->setText(dentistID);
    ui->reportDateLabel->setText(reportDate);
    ui->notePlainTextEdit->setPlainText(patientNote);
    ui->dentistNameLineEdit->setText(dentistName);


}
