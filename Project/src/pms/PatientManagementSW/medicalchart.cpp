/*
 * 프로그램명 : PatientManagementSW
 * 파일명 : medicalChart.cpp
 * 설명 : 상세 진료기록을 확인, pdf로 저장/프린트
 * 작성자 : 김유선
 * 최종 수정일 : 2023.02.16
 */


#include "medicalchart.h"
#include "ui_medicalchart.h"

#include <QPrintDialog>
#include <QPrinter>
#include <QPainter>
#include <QRectF>
#include <QDate>


MedicalChart::MedicalChart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MedicalChart)
{
    ui->setupUi(this);
    this->setFixedSize(601, 840);

    // 프린트 아이콘 설정
    ui->printPushButton->setIcon(QIcon(":/img/print.png"));

    // 오늘의 날짜 설정
    QString year = QDate::currentDate().toString("yyyy");
    QString month = QDate::currentDate().toString("MM");
    QString day = QDate::currentDate().toString("dd");

    ui->yearLabel->setText(year);
    ui->monthLabel->setText(month);
    ui->dayLabel->setText(day);


}

MedicalChart::~MedicalChart()
{
    delete ui;
}

// 진료기록 차트에 환자 정보와 진료기록 정보를 띄워주는 부분
void MedicalChart::patientReportInfoSended(QString patientDetail, QString reportDetail)
{
    qDebug() << "patientDetail: " << patientDetail;
    qDebug() << "reportDetail: " << reportDetail;

    QString patientID, patientName, patientSex, patientBirth, patientTel, patientAddress;
    patientID = patientDetail.split("|")[0];
    patientName = patientDetail.split("|")[1];
    patientSex = patientDetail.split("|")[2];
    patientBirth = patientDetail.split("|")[3];
    patientTel = patientDetail.split("|")[4];
    patientAddress = patientDetail.split("|")[5];

    ui->patientIDLineEdit->setText(patientID);
    ui->patientNameLineEdit->setText(patientName);
    ui->sexLineEdit->setText(patientSex);
    ui->birthLineEdit->setText(patientBirth);
    ui->telLineEdit->setText(patientTel);
    ui->addressPlainTextEdit->setPlainText(patientAddress);

    QString reportID, dentistID, reportDate, patientNote, dentistName;
    reportID = reportDetail.split("|")[0];
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

// 프린트, pdf저장 기능
void MedicalChart::on_printPushButton_clicked()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPageSize::A4);

    QPrintDialog* printDialog = new QPrintDialog(&printer, this);
    if (printDialog->exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        QPixmap buffer = ui->frame_2->grab();
        QRect rect = printer.pageRect(QPrinter::DevicePixel).toRect();

        buffer.setDevicePixelRatio(1);

        buffer = buffer.scaled(rect.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        painter.drawPixmap(0,0, buffer);

        painter.end();
    }
}

