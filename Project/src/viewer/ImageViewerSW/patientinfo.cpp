#include "patientinfo.h"
#include "ui_patientinfo.h"

#include <QMessageBox>
#include <QFileDialog>

PatientInfo::PatientInfo(QWidget *parent) :
    QWidget(parent), ui(new Ui::PatientInfo)
{
    ui->setupUi(this);
    ui->lineEdit->setReadOnly(true);
}

PatientInfo::~PatientInfo()
{
    delete ui;
}

//진료 완료 버튼 클릭 시 해당 환자의 정보 받음
void PatientInfo::receiveEndTreatment(QString Data)
{
    qDebug() << "진료 완료" << Data;
    QString event = Data.split("<CR>")[0];
    QString ID = Data.split("<CR>")[1];

    int i = 0;  //0번째 열 (환자 ID)
    auto flag = Qt::MatchCaseSensitive|Qt::MatchContains;
    auto items = ui->WaitingList->findItems(ID, flag, i);

    //진료 완료 버튼 클릭시 넘어온 환자를 대기 환자 리스트에서 삭제
    foreach(auto i, items) {
        QTreeWidgetItem* c = static_cast<QTreeWidgetItem*>(i);
        ui->WaitingList->takeTopLevelItem(ui->WaitingList->indexOfTopLevelItem(c));
    }

    //진료 완료 버튼 클릭시 환자 정보 테이블 위젯을 비워줌
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem(""));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem(""));
    ui->tableWidget->setItem(0, 2, new QTableWidgetItem(""));
    ui->tableWidget->setItem(0, 3, new QTableWidgetItem(""));
    ui->tableWidget->setItem(0, 4, new QTableWidgetItem(""));
}

//촬영 완료된 환자ID를 받아와 해당 환자의 진행 상황 변경
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
    patientSex = data.split("|")[1];
    QString patientBirth = data.split("|")[2];
    QString Memo = data.split("|")[3];

    ui->tableWidget->setItem(0, 0, new QTableWidgetItem(patientID));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem(patientName));
    ui->tableWidget->setItem(0, 2, new QTableWidgetItem(patientSex));
    ui->tableWidget->setItem(0, 3, new QTableWidgetItem(patientBirth));
    ui->tableWidget->setItem(0, 4, new QTableWidgetItem(Memo));

    //대기 리스트에서 선택된 환자 정보를 전송 및 처방전에 환자 정보 띄워주기 위한 시그널
    emit sendPatientInfo(patientID, patientName, patientSex);
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

//대기 리스트에서 환자가 선택되면 해당 환자ID, 환자 이름을 전송하기 위해 멤버 변수에 저장
void PatientInfo::on_WaitingList_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    selectPatientID = item->text(0);
    selectPatientName = item->text(1);
}


//진료 시작 버튼 클릭 시 대기 환자 리스트에서 선택된 환자의 진행 상황을 진료중으로 변경
void PatientInfo::on_Treatmentstart_clicked()
{
    qDebug() << "진료 시작";

    //이미 진료중인 환자가 있을 경우 경고 메시지 출력
    if(!ui->WaitingList->findItems("진료중",  Qt::MatchCaseSensitive, 2).empty()){
        QMessageBox:: critical(this, "경고", "이미 진료중인 환자가 있습니다.");
        return;
    }

    int i = 0;  //0번째 열 (환자 ID)
    auto flag = Qt::MatchCaseSensitive|Qt::MatchContains;
    auto items = ui->WaitingList->findItems(selectPatientID, flag, i);

    foreach(auto i, items) {
        QTreeWidgetItem* c = static_cast<QTreeWidgetItem*>(i);

        //선택된 환자가 촬영중일 경우 경고 메시지 출력
        if(c->text(2) == "촬영중"){
            QMessageBox:: critical(this, "경고", "해당 환자는 촬영중 입니다.");
            return;
        }
        c->setText(2, "진료중");
    }

    QString Data = "VTS<CR>" + selectPatientID + "<CR>" + "";
    emit sendWaitingPatient(Data);



    //진료 시작 버튼 클릭 시 해당 환자의 이미지 업로드
    emit sendImageFile(selectPatientID);
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
        QMessageBox:: critical(this, "경고", "CEPH/PANO 중 하나 이상을 선택하세요.");
        return;
    }

    QString Data = "SRQ<CR>" + patientID + "<CR>" + patientName + "|" + Photo;
    emit sendCameraPatient(Data);

    //환자의 ID로 WaitingList에서 검색하여 해당 환자의 진행 상황을 "촬영중"으로 변경
    int i = 0;
    auto flag = Qt::MatchCaseSensitive|Qt::MatchContains;
    auto items = ui->WaitingList->findItems(patientID, flag, i);

    foreach(auto i, items) {
        QTreeWidgetItem* c = static_cast<QTreeWidgetItem*>(i);
        c->setText(2, "촬영중");
    }

    /*촬영의뢰 버튼 클릭 시 해당 환자의 이미지 파일이 삭제되고
        진료 시작을 누를때 다시 생성*/
    QDir dir(QString("./Image/%1").arg(patientID));
    dir.removeRecursively();

    //촬영 의뢰 버튼 클릭시 환자 정보 테이블 위젯을 비워줌
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem(""));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem(""));
    ui->tableWidget->setItem(0, 2, new QTableWidgetItem(""));
    ui->tableWidget->setItem(0, 3, new QTableWidgetItem(""));
    ui->tableWidget->setItem(0, 4, new QTableWidgetItem(""));
}

