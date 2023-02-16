/*
 * 프로그램명 : PatientManagementSW
 * 파일명 : medicalRecordManager.cpp
 * 설명 : 검색한 환자에 대한 진료기록 목록을 확인
 * 작성자 : 김유선
 * 최종 수정일 : 2023.02.16
 */


#include "medicalrecordmanager.h"
#include "ui_medicalrecordmanager.h"

#include "medicalchart.h"

#include <QGraphicsEffect>
#include <QPainter>
#include <QPrinter>
#include <QPrintDialog>
#include <QRectF>


MedicalRecordManager::MedicalRecordManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MedicalRecordManager)
{
    ui->setupUi(this);

    // 라벨 스타일 설정
    QString labelStyle = "QLabel { "
                         "background-color: rgb(150, 150, 150);"
                         "border-radius:10px;"
                         "color:#ffffff;"
                         "outline: 0; "
                         "}";
    ui->label_7->setStyleSheet(labelStyle);

    // 라벨에 설정할 그림자 효과
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(5);
    effect->setXOffset(5);
    effect->setYOffset(5);
    effect->setColor(QColor(220,220,220));
    ui->label_7->setGraphicsEffect(effect);

    // 진료기록을 띄울 때는 새 창으로 띄움
    medicalChart = new MedicalChart(0);
    connect(this, SIGNAL(sendPatientReportInfo(QString, QString)), medicalChart, SLOT(patientReportInfoSended(QString, QString)));
}

MedicalRecordManager::~MedicalRecordManager()
{
    delete ui;
}

// 검색한 환자에 대한 진료기록들을 가져옴
void MedicalRecordManager::recordDataSended(QString sendedID, QString sendedData)
{
    qDebug() << "sendedID, sendedData: " << sendedID << ", "<<sendedData;

    ui->recordTreeWidget->clear();

    if(sendedData == "<NEL>")
        return;

    // 동명이인일 경우 진료기록 데이터를 묶어 보내지 않기 때문에 <NEL>을 포함하지 않을 시를 대비해 예외처리
    if(sendedData.contains("<NEL>", Qt::CaseInsensitive) == false)
        return;

    QString patientName, patientSex, patientBirth, patientTel, patientAddress, patientMemo;
    patientName = sendedData.split("|")[0];
    patientSex = sendedData.split("|")[1];
    patientBirth = sendedData.split("|")[2];
    patientTel = sendedData.split("|")[3];
    patientAddress = sendedData.split("|")[4];
    patientMemo = sendedData.split("|")[5];


    patientDetail = sendedID + "|" + sendedData.split("<NEL>")[0];
    qDebug() << "patientDetail" << patientDetail;

    // 진료기록 목록이 띄워지는 recordTreeWidget에 저장
    QString rowData, reportID, doctorID, reportDate, dentistName;
    for(int i=1; i<sendedData.count("<NEL>"); i++)
    {
        rowData = sendedData.split("<NEL>")[i];
        reportID = rowData.split("|")[0];
        doctorID = rowData.split("|")[2];
        reportDate = rowData.split("|")[3];
        QString patientNote = rowData.split("|")[4];
        dentistName = rowData.split("|")[5];

        QTreeWidgetItem* row = new QTreeWidgetItem;
        ui->recordTreeWidget->addTopLevelItem(row);
        row->setText(0, reportID);
        row->setText(1, reportDate);
        row->setText(2, dentistName);

        reportInfo.insert(i-1, rowData);

        totalRowCount += 1;
    }
}

// recordTreeWidget에서 특정 진료기록을 더블클릭하면 상세 진료기록 창을 띄워줌
void MedicalRecordManager::on_recordTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    int currentRow = ui->recordTreeWidget->currentIndex().row();

    reportDetail = reportInfo.find(currentRow).value();

    medicalChart->show();

    emit sendPatientReportInfo(patientDetail, reportDetail);
}


// 처방전 작성하면 환자진료기록 바로 띄우기
void MedicalRecordManager::addNewRecord(QString newRecordInfo)
{
    // 다시 서치한거같은 효과 주기
    QString searchData = "SEN^PSE<CR>0<CR>" + newRecordInfo.split("<CR>")[1]; // pid담아서 pse를 써서 보냄
    emit sendReSearchData(searchData);
}
