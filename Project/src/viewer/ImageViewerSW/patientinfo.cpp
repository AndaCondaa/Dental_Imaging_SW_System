#include "patientinfo.h"
#include "ui_patientinfo.h"

#include <QMessageBox>

PatientInfo::PatientInfo(QWidget *parent) :
    QWidget(parent), ui(new Ui::PatientInfo)
{
    ui->setupUi(this);
}

PatientInfo::~PatientInfo()
{
    delete ui;
}

void PatientInfo::receivePhotoEnd(QString ID)
{
    //환자의 ID로 WaitingList에서 검색하여 해당 환자의 진행 상황을 "촬영중"으로 변경
    int i = 0;  //0번째 열 (환자 ID)
    auto flag = Qt::MatchCaseSensitive|Qt::MatchContains;
    auto items = ui->WaitingList->findItems(ID, flag, i);

    foreach(auto i, items) {
        QTreeWidgetItem* c = static_cast<QTreeWidgetItem*>(i);
        c->setText(2, "대기중");
    }
}

void PatientInfo::receiveDoctorName(QString DoctorID, QString DoctorName)
{
    ui->lineEdit->setText(DoctorName);
    ui->lineEdit->setReadOnly(true);
}

void PatientInfo::receiveWaitingList(QString ID, QString Name)
{
    QTreeWidgetItem* item = new QTreeWidgetItem;
    ui->WaitingList->addTopLevelItem(item);
    item->setText(0, ID);
    item->setText(1, Name);
    item->setText(2, "대기중");
}

void PatientInfo::receiveSelectPatient(QString ID, QString data)
{
    patientID = ID;
    patientName = data.split("|")[0];
    QString patientSex = data.split("|")[1];
    QString patientBirth = data.split("|")[2];
    QString Memo = data.split("|")[3];

    ui->tableWidget->setItem(0, 0, new QTableWidgetItem(patientID));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem(patientName));
    ui->tableWidget->setItem(0, 2, new QTableWidgetItem(patientSex));
    ui->tableWidget->setItem(0, 3, new QTableWidgetItem(patientBirth));
    ui->tableWidget->setItem(0, 4, new QTableWidgetItem(Memo));
}

void PatientInfo::receivePMSCameraPatient(QString ID, QString data)
{
    //환자의 ID로 WaitingList에서 검색하여 해당 환자의 진행 상황을 "촬영중"으로 변경
    int i = 0;  //0번째 열 (환자 ID)
    auto flag = Qt::MatchCaseSensitive|Qt::MatchContains;
    auto items = ui->WaitingList->findItems(ID, flag, i);

    foreach(auto i, items) {
        QTreeWidgetItem* c = static_cast<QTreeWidgetItem*>(i);
        c->setText(2, "촬영중");
    }
}

void PatientInfo::on_WaitingList_itemClicked(QTreeWidgetItem *item, int column)
{
    selectPatientID = item->text(0);
    selectPatientName = item->text(1);
    qDebug() << item->text(0);
}


void PatientInfo::on_Treatmentstart_clicked()
{
    qDebug() << "진료 시작";
    QString Data = "VTS<CR>" + selectPatientID + "<CR>" + "";
    emit sendWaitingPatient(Data);
    emit sendPatientInfo(selectPatientID, selectPatientName);

    int i = 0;  //0번째 열 (환자 ID)
    auto flag = Qt::MatchCaseSensitive|Qt::MatchContains;
    auto items = ui->WaitingList->findItems(selectPatientID, flag, i);

    foreach(auto i, items) {
        QTreeWidgetItem* c = static_cast<QTreeWidgetItem*>(i);
        c->setText(2, "진료중");
    }

}

void PatientInfo::on_Camerastart_clicked()
{
    qDebug() << "촬영 시작";

    QString Photo;
    Ceph = ui->Ceph->isChecked();
    Pano = ui->Pano->isChecked();

    if(Ceph && Pano)
    {
        Photo = "BOTH";
    }
    else if(Ceph == 1)
    {
        Photo = "CEPH";
    }
    else if(Pano == 1)
    {
        Photo = "PANO";
    }
    else
    {
        QMessageBox:: critical(this, "경고",
                              "CEPH/PANO 중 하나 이상을 선택하세요.");
        return;
    }


    QString Data = "SRQ<CR>" + patientID + "<CR>" + patientName + "|" + Photo;
    qDebug() << Data;
    emit sendCameraPatient(Data);

    //환자의 ID로 WaitingList에서 검색하여 해당 환자의 진행 상황을 "촬영중"으로 변경
    int i = 0;
    auto flag = Qt::MatchCaseSensitive|Qt::MatchContains;
    auto items = ui->WaitingList->findItems(patientID, flag, i);

    foreach(auto i, items) {
        QTreeWidgetItem* c = static_cast<QTreeWidgetItem*>(i);
        c->setText(2, "촬영중");
    }
}

