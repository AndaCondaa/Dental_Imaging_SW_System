/*프로그램명 : ImageViewerSW
파일명 : prescription.cpp
설명 : 서버에서 받아온 환자 정보들을 treeWidget과 tableWidget을 통해 대기 환자 리스트와 환자 정보로 구성하였고
진료 시작 버튼, 촬영 의뢰 버튼을 통해 타 SW들과 연동되어 있는 클래스
작성자 : 이정연
최종 수정 날짜 : 2023.02.11*/


#include "prescription.h"
#include "qpainter.h"
#include "ui_prescription.h"

#include <QDate>
#include <QMessageBox>
#include <QFileDialog>
#include <QPrintDialog>
#include <QPrinter>

Prescription::Prescription(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Prescription)
{
    ui->setupUi(this);
    this->setFixedSize(592, 714);

    messagebox = new QMessageBox();
    messagebox->setStyleSheet("QMessageBox { "
                              "background-color: rgb(255, 255, 255);"
                              "color:#000000;"
                              "}"
                              "QPushButton { "
                              "background-color: rgb(250, 250, 250);"
                              "border-radius:2px;"
                              "font: bold;"
                              "color:rgb(0, 0, 0);"
                              "border-color: blue"
                              "outline: 0;"
                              "}");
}

Prescription::~Prescription()
{
    delete ui;
}

// 처방전 버튼 클릭 시 의사 정보, 환자 정보가 이미 작성된 채로 띄워지도록 설정
void Prescription::receivePrescription(QString DID, QString DName, QString PID, QString PName, QString PSex)
{
    // 오늘 날짜를 자동적으로 입력되게 하기 위한 변수 (형식 : 2023-02-28)
    CurrentDate = QDate::currentDate().toString("yyyy-MM-dd");

    DoctorID = DID;
    DoctorName = DName;
    PatientID = PID;
    PatientName = PName;
    PatientSex = PSex;

    // 처방전 내용을 제외하고 다른 값은 모두 읽기 전용으로 설정
    ui->PatientName->setReadOnly(true);
    ui->PatientID->setReadOnly(true);
    ui->DoctorName->setReadOnly(true);
    ui->Date->setReadOnly(true);
    ui->PatientSex->setReadOnly(true);

    // 처방전 작성 버튼 클릭 시 이미 데이터가 입력되어 있도록 설정
    ui->PatientName->setText(PatientName);
    ui->PatientID->setText(PatientID);
    ui->DoctorName->setText(DoctorName);
    ui->Date->setText(CurrentDate);
    ui->PatientSex->setText(PatientSex);
}

// 처방전 작성 완료 버튼 클릭 시 처방전에 작성된 내용을 서버로 전송
void Prescription::on_Finish_clicked()
{
    QString DID = DoctorID;
    QString DName = DoctorName;
    QString PID = PatientID;
    QString PName = PatientName;
    QString Date = CurrentDate;
    QString Context = ui->textEdit->toPlainText();

    // VNT<CR>PID<CR>환자이름|의사번호|의사이름|진료날짜|진료내용(처방내용)
    QString Data = "SEN^VNT<CR>" + PID + "<CR>" + PName + "|" + DID + "|" + DName + "|" + Date + "|" + Context;

    // 처방전 작성 완료 시 서버로 해당 데이터를 전송
    emit sendPrescriptionFinish(Data);

}

// 처방전 작성이 완료되면 DB에 저장 여부를 서버로부터 수신
void Prescription::receivePrescriptionYesOrNot(QString signal)
{
    // 처방전 저장이 완료된 경우
    if(signal == "ACK")
    {
        QMessageBox::information(messagebox, "저장 완료", "처방전 작성이 완료되었습니다.");
        ui->PatientName->clear();
        ui->PatientID->clear();
        ui->textEdit->clear();
    }

    // signal이 ERR를 받으면 DB에 처방전이 저장되지 않았으므로 프린트 실행
    else {

        int ret = QMessageBox::critical(messagebox, tr("경고"), tr("처방전이 저장되지 않았습니다.\n\n"
        "이미지로 출력하시겠습니까?"), QMessageBox::Yes | QMessageBox::No,
                                           QMessageBox::No);

        // Yes 선택 시 처방전을 프린터로 출력, No 선택 시 출력하지 않음
        switch (ret) {
          case QMessageBox::Yes: {
            QPrinter printer(QPrinter::HighResolution);
            printer.setFullPage(true);
            printer.setPageSize(QPageSize::A4);

            QPrintDialog* printDialog = new QPrintDialog(&printer, this);
            if (printDialog->exec() == QDialog::Accepted) {
                QPainter painter(&printer);
                // 이미지에 해당하는 부분을 캡쳐
                QPixmap buffer = this->grab();
                QRect rect = printer.pageRect(QPrinter::DevicePixel).toRect();
                buffer.setDevicePixelRatio(1);

                // 프린트 시 중앙에 맞춰서 출력하기 위해 좌표 설정 및 resize
                buffer = buffer.scaled(rect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                painter.drawPixmap(rect.center().x()-buffer.width()/2, rect.center().y()-buffer.height()/2, buffer);
                painter.end();
            }
        }
              break;

          case QMessageBox::No:
              break;

          default:
              break;
        }
    }
}

