#include "medicalchart.h"
#include "ui_medicalchart.h"

#include <QPrintDialog>
#include <QPrinter>
#include <QPainter>
#include <QRectF>



MedicalChart::MedicalChart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MedicalChart)
{
    ui->setupUi(this);
    this->setFixedSize(610, 645);



//    QString pushButtonStyle = "QPushButton { "
//                              "qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(90, 90, 90, 100));"
//                              "border-radius:10px;"
//                              "border-color: gray;"
//                          "}"
//                          "QPushButton:hover { "
//                              "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(237, 136, 23, 50));"
//                              "border-radius:10px;"
//                          "}";


//    ui->pushButton_2->setStyleSheet(pushButtonStyle);


    ui->printPushButton->setIcon(QIcon("./print.png"));
    ui->emailPushButton->setIcon(QIcon("./email.png"));
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

void MedicalChart::on_printPushButton_clicked()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setFullPage(true);
    printer.setPageSize(QPageSize::A4);
    //printer.setOutputFormat(QPrinter::PdfFormat);
    //printer.setOutputFileName("test.pdf");

    QPrintDialog* printDialog = new QPrintDialog(&printer, this);
    if (printDialog->exec() == QDialog::Accepted) {
        // print …
        QPainter painter(&printer);
        QPixmap buffer = grab();
        //        QRect rect = painter.viewport();
        QRect rect = printer.pageRect(QPrinter::DevicePixel).toRect();
        painter.drawPixmap(0, 0, buffer.scaled(rect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        painter.end();
        //        this->render(&painter);
    }
}

