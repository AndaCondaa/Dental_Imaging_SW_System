/*프로그램명 : ImageViewerSW
파일명 : patientinfo.cpp
설명 : 서버에서 받아온 환자 정보들을 treeWidget과 tableWidget을 통해 대기 환자 리스트와 환자 정보로 구성하였고
진료 시작 버튼, 촬영 의뢰 버튼을 통해 타 SW들과 연동되어 있는 클래스
작성자 : 이정연
최종 수정 날짜 : 2023.02.11*/


#include "patientinfo.h"
#include "ui_patientinfo.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QGraphicsDropShadowEffect>

PatientInfo::PatientInfo(QWidget *parent) :
    QWidget(parent), ui(new Ui::PatientInfo)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnWidth(0, 210);
    ui->WaitingList->setColumnWidth(0, 75);
    ui->WaitingList->setColumnWidth(1, 90);
    ui->WaitingList->setColumnWidth(2, 100);

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

    // 담당 의사 명이 출력되는 lineEdit을 read-only로 설정
    ui->lineEdit->setReadOnly(true);

    // 서버로부터 이미지 파일이 모두 수신되면 true로 변경
    AllSendImageCheck = false;

    // 화면이 처음 열리면 진료 시작 버튼 활성화, 촬영 의뢰 버튼 비활성화
    ui->Camerastart->setDisabled(true);
    ui->Treatmentstart->setDisabled(false);

    ui->Treatmentstart->setStyleSheet("QPushButton { "
                                      "background-color: #ED8817;"
                                      "border-radius:10px;"
                                      "color:#ffffff;"
                                      "outline: 0; "
                                      "font-family:Malgun Gothic;"
                                      "font-size:15px;"
                                      "font-weight:bold;"
                                      "text-decoration:none;"
                                      "min-height: 50px;"
                                      "min-width: 80px;"
                                      "}"
                                      "QPushButton:hover { "
                                      "background-color: #F2A754;"
                                      "border-radius:10px;"
                                      "color:#ffffff;"
                                      "outline: 0; "
                                      "font-family:Malgun Gothic;"
                                      "font-size:15px;"
                                      "font-weight:bold;"
                                      "text-decoration:none;"
                                      "min-height: 50px;"
                                      "min-width: 80px;"
                                      "}"
                                      "QPushButton:disabled { "
                                      "background-color: rgb(132,132,132); "      // 회색
                                      "border-radius:10px;"
                                      "border:1px solid rgb(132,132,132);"
                                      "color:#ffffff;"
                                      "font-family:Malgun Gothic;"
                                      "font-size:15px;"
                                      "font-weight:bold;"
                                      "text-decoration:none;"
                                      "min-width: 80px;"
                                      "outline: 0; "
                                      "}");

    ui->Camerastart->setStyleSheet("QPushButton { "
                                   "background-color: #ED8817;"
                                   "border-radius:10px;"
                                   "color:#ffffff;"
                                   "outline: 0; "
                                   "font-family:Malgun Gothic;"
                                   "font-size:15px;"
                                   "font-weight:bold;"
                                   "text-decoration:none;"
                                   "min-height: 50px;"
                                   "min-width: 80px;"
                                   "}"
                                   "QPushButton:hover { "
                                   "background-color: #F2A754;"
                                   "border-radius:10px;"
                                   "color:#ffffff;"
                                   "outline: 0; "
                                   "font-family:Malgun Gothic;"
                                   "font-size:15px;"
                                   "font-weight:bold;"
                                   "text-decoration:none;"
                                   "min-height: 50px;"
                                   "min-width: 80px;"
                                   "}"
                                   "QPushButton:disabled { "
                                   "background-color: rgb(132,132,132); "      // 회색
                                   "border-radius:10px;"
                                   "border:1px solid rgb(132,132,132);"
                                   "color:#ffffff;"
                                   "font-family:Malgun Gothic;"
                                   "font-size:15px;"
                                   "font-weight:bold;"
                                   "text-decoration:none;"
                                   "min-width: 80px;"
                                   "outline: 0; "
                                   "}");
}

PatientInfo::~PatientInfo()
{
    delete ui;
}

// 서버와의 연결 해제 시 대기리스트 클리어
void PatientInfo::clearWaitingList(int)
{
    ui->WaitingList->clear();
}

// 대기 리스트에서 환자가 선택되면 해당 환자ID, 환자 이름을 전송하기 위해 멤버 변수에 저장
void PatientInfo::on_WaitingList_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    selectPatientID = item->text(0);
    selectPatientName = item->text(1);
}

// 진료 시작 버튼 클릭 시 대기 환자 리스트에서 선택된 환자의 진행 상황을 진료중으로 변경
void PatientInfo::on_Treatmentstart_clicked()
{
    // 환자를 선택하지 않고 진료 시작을 클릭 시 경고 메시지 출력
    if(selectPatientID == "" && selectPatientName == ""){
        QMessageBox:: critical(messagebox, "경고", "환자를 선택해 주세요.");
        return;
    }

    // 이미 진료중인 환자가 있을 경우 경고 메시지 출력
    if(!ui->WaitingList->findItems("진료중",  Qt::MatchCaseSensitive, 2).empty()){
        QMessageBox:: critical(messagebox, "경고", "이미 진료중인 환자가 있습니다.");
        return;
    }

    int i = 0;  // 0번째 열 (환자 ID)
    auto flag = Qt::MatchCaseSensitive|Qt::MatchContains;
    auto items = ui->WaitingList->findItems(selectPatientID, flag, i);

    foreach(auto i, items) {
        QTreeWidgetItem* c = static_cast<QTreeWidgetItem*>(i);

        // 선택된 환자가 촬영중일 경우 경고 메시지 출력
        if(c->text(2) == "촬영중"){
            QMessageBox:: critical(messagebox, "경고", "해당 환자는 촬영중 입니다.");
            return;
        }
        c->setText(2, "진료중");
    }

    // 선택된 환자의 정보(ID)를 서버로 전송
    QString Data = "SEN^VTS<CR>" + selectPatientID + "<CR>" + "";
    emit sendWaitingPatient(Data);
}

/* 진료 시작 버튼 클릭 시 서버로 보낸 환자ID, 환자명을 통해 불러온 환자 정보를
뷰어의 tableWidget에 띄워줌*/
void PatientInfo::receiveSelectPatient(QString ID, QString data)
{
    qDebug() << "진료 시작";

    // 진료 시작 버튼 클릭 시 진료 시작 버튼 비활성화, 촬영 의뢰 버튼 활성화
    ui->Treatmentstart->setDisabled(true);
    ui->Camerastart->setDisabled(false);

    patientID = ID;
    patientName = data.split("|")[0];
    patientSex = data.split("|")[1];
    QString patientBirth = data.split("|")[2];
    QString Memo = data.split("|")[3];

    // tableWidget에 서버로부터 받아온 정보를 띄워줌
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem(patientID));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem(patientName));
    ui->tableWidget->setItem(0, 2, new QTableWidgetItem(patientSex));
    ui->tableWidget->setItem(0, 3, new QTableWidgetItem(patientBirth));
    ui->tableWidget->setItem(0, 4, new QTableWidgetItem(Memo));

    // 대기 리스트에서 선택된 환자 정보를 전송 및 처방전에 환자 정보 띄워주기 위한 시그널
    emit sendPatientInfo(patientID, patientName, patientSex);
}

// 뷰어SW 로그인 시 PMS에서 추가되어 있던 기존 환자 대기 리스트를 불러와 띄워줌
void PatientInfo::receiveWaitTreatment(int iteration, QString data)
{
    for(int i = 0; i < iteration; i++){

        // <r>을 통해 구분되어 있는 대기 환자 정보들을 treeWidget에 추가
        QString temp = data.split("<r>")[i];
        QString ID = temp.split("|")[0];
        QString Name = temp.split("|")[1];
        QString Status = temp.split("|")[2];

        QTreeWidgetItem* item = new QTreeWidgetItem;
        ui->WaitingList->addTopLevelItem(item);
        item->setText(0, ID);
        item->setText(1, Name);

        if(Status == "WT"){
            item->setText(2, "진료대기");
        }
        else if(Status == "TM"){
            item->setText(2, "진료중");

            // 진료 시작 버튼 클릭 시 진료 시작 버튼 비활성화, 촬영 의뢰 버튼 활성화
            ui->Treatmentstart->setDisabled(true);
            ui->Camerastart->setDisabled(false);

            int i = 0;  // 0번째 열 (환자 ID)
            auto flag = Qt::MatchCaseSensitive|Qt::MatchContains;
            auto items = ui->WaitingList->findItems(ID, flag, i);

            foreach(auto i, items) {
                QTreeWidgetItem* c = static_cast<QTreeWidgetItem*>(i);

                // 선택된 환자가 촬영중일 경우 경고 메시지 출력
                if(c->text(2) == "촬영중"){
                    QMessageBox:: critical(messagebox, "경고", "해당 환자는 촬영중 입니다.");
                    return;
                }
                c->setText(2, "진료중");
            }

            // 특정 환자가 진료중일 경우 해당 환자의 정보를 서버에 요청
            QString Data = "SEN^VTS<CR>" + ID + "<CR>" + "";
            emit sendWaitingPatient(Data);
        }
        else {
            item->setText(2, "촬영중");
        }
    }
}

// 진료 완료 버튼 클릭 시 해당 환자의 정보 받음
void PatientInfo::receiveEndTreatment(QString Data)
{
    qDebug() << "진료 완료" << Data;

    // 진료 완료 버튼 클릭 시 진료 시작 버튼 활성화, 촬영 의뢰 버튼 비활성화
    ui->Treatmentstart->setDisabled(false);
    ui->Camerastart->setDisabled(true);

    QString ID = Data.split("<CR>")[1];

    int i = 0;  // 0번째 열 (환자 ID)
    auto flag = Qt::MatchCaseSensitive|Qt::MatchContains;
    auto items = ui->WaitingList->findItems(ID, flag, i);

    // 진료 완료 버튼 클릭시 넘어온 환자를 대기 환자 리스트에서 삭제
    foreach(auto i, items) {
        QTreeWidgetItem* c = static_cast<QTreeWidgetItem*>(i);
        ui->WaitingList->takeTopLevelItem(ui->WaitingList->indexOfTopLevelItem(c));
    }

    // 진료 완료 버튼 클릭시 환자 정보 테이블 위젯을 비워줌
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem(""));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem(""));
    ui->tableWidget->setItem(0, 2, new QTableWidgetItem(""));
    ui->tableWidget->setItem(0, 3, new QTableWidgetItem(""));
    ui->tableWidget->setItem(0, 4, new QTableWidgetItem(""));
}

// 촬영 완료된 환자ID를 받아와 해당 환자의 진행 상황 변경
void PatientInfo::receivePhotoEnd(QString ID)
{
    // 환자의 ID로 WaitingList에서 검색하여 해당 환자의 진행 상황을 "촬영중"으로 변경
    int i = 0;  // 0번째 열 (환자 ID)
    auto flag = Qt::MatchCaseSensitive|Qt::MatchContains;
    auto items = ui->WaitingList->findItems(ID, flag, i);

    // 촬영이 끝난 환자의 진행 상황을 진료대기로 변경
    foreach(auto i, items) {
        QTreeWidgetItem* c = static_cast<QTreeWidgetItem*>(i);
        c->setText(2, "진료대기");
    }
}

// 로그인 시 입력되는 의사명을 담당의사란에 입력해줌
void PatientInfo::receiveDoctorName(QString DoctorID, QString DoctorName)
{
    ui->lineEdit->setText(DoctorName);
}

// PMS에서 대기 환자 추가 시 대기 리스트에 해당 환자를 TreeWidget에 추가
void PatientInfo::receiveWaitingList(QString ID, QString Name)
{
    QTreeWidgetItem* item = new QTreeWidgetItem;
    ui->WaitingList->addTopLevelItem(item);
    item->setText(0, ID);
    item->setText(1, Name);
    item->setText(2, "진료대기");
}

// 촬영SW에서 전달받아 뷰어와 PMS에서 촬영을 시작하는 환자의 진행 상황 추가
void PatientInfo::receivePMSCameraPatient(QString ID, QString data)
{
    // 촬영 SW가 작동중이지 않을 경우 연결을 기다릴지 다른 사람 진료를 시작할지 선택
    if(data == "Null")
    {
        int ret = QMessageBox::critical(messagebox, tr("경고"), tr("촬영SW의 정상적으로 작동중이지 않습니다.\n\n"
                                                           "계속 기다리시려면 Yes,\n다른 환자를 먼저 진료보시려면 No를 눌러주세요."), QMessageBox::Yes | QMessageBox::No,
                                        QMessageBox::No);

        switch (ret) {
        // 촬영 SW를 계속 기다리는 경우
        case QMessageBox::Yes:
            return;

            // 다른 환자의 진료를 먼저 보는 경우
        case QMessageBox::No:{

            // 다시 진료 시작을 위해 촬영 의뢰 버튼 비활성화, 진료 시작 버튼 활성화
            ui->Treatmentstart->setDisabled(false);
            ui->Camerastart->setDisabled(true);

            // 환자의 ID로 WaitingList에서 검색하여 해당 환자의 진행 상황을 "진료대기"으로 변경
            int i = 0;
            auto flag = Qt::MatchCaseSensitive|Qt::MatchContains;
            auto items = ui->WaitingList->findItems(ID, flag, i);

            foreach(auto i, items) {
                QTreeWidgetItem* c = static_cast<QTreeWidgetItem*>(i);
                c->setText(2, "진료대기");
            }

            // 이미지 파일 삭제, 진료 시작을 누를때 이미지 파일 다시 생성
            QDir dir("./Image/");
            dir.removeRecursively();

            // 다시 진료 시작을 위해 환자 정보 테이블 위젯을 비워줌
            ui->tableWidget->setItem(0, 0, new QTableWidgetItem(""));
            ui->tableWidget->setItem(0, 1, new QTableWidgetItem(""));
            ui->tableWidget->setItem(0, 2, new QTableWidgetItem(""));
            ui->tableWidget->setItem(0, 3, new QTableWidgetItem(""));
            ui->tableWidget->setItem(0, 4, new QTableWidgetItem(""));

            // 다시 진료 시작을 위해 ImageAlbum 클래스에도 알려줌
            emit sendCameraStart();

            // 다음 환자의 이미지 파일을 받으면 true로 변경
            AllSendImageCheck = false;

            // 촬영 의뢰를 하지 못한 환자 정보를 서버로 전송
            QString Data = "SEN^MWL<CR>" + ID + "<CR>WT";
            emit sendCameraDelay(Data);
        }
            break;

        default:
            break;
        }
    }

    else {

        // 촬영 의뢰 버튼 클릭 시 촬영 의뢰 버튼 비활성화, 진료 시작 버튼 활성화
        ui->Treatmentstart->setDisabled(false);
        ui->Camerastart->setDisabled(true);

        // 환자의 ID로 WaitingList에서 검색하여 해당 환자의 진행 상황을 "촬영중"으로 변경
        int i = 0;
        auto flag = Qt::MatchCaseSensitive|Qt::MatchContains;
        auto items = ui->WaitingList->findItems(ID, flag, i);

        foreach(auto i, items) {
            QTreeWidgetItem* c = static_cast<QTreeWidgetItem*>(i);
            c->setText(2, "촬영중");
        }

        /*촬영의뢰 버튼 클릭 시 해당 환자의 이미지 파일이 삭제되고
            진료 시작을 누를때 다시 생성*/
        QDir dir("./Image/");
        dir.removeRecursively();

        // 촬영 의뢰 버튼 클릭시 환자 정보 테이블 위젯을 비워줌
        ui->tableWidget->setItem(0, 0, new QTableWidgetItem(""));
        ui->tableWidget->setItem(0, 1, new QTableWidgetItem(""));
        ui->tableWidget->setItem(0, 2, new QTableWidgetItem(""));
        ui->tableWidget->setItem(0, 3, new QTableWidgetItem(""));
        ui->tableWidget->setItem(0, 4, new QTableWidgetItem(""));

        // 촬영 의뢰 버튼 클릭 시 ImageAlbum 클래스에도 알려줌
        emit sendCameraStart();

        // 다음 환자의 이미지 파일을 받으면 true로 변경
        AllSendImageCheck = false;
    }
}

// 서버에서 환자의 다중 이미지 파일이 전송이 완료되면 TRUE 값 반환
void PatientInfo::receiveAllImageFileP(bool AllImageFile)
{
    AllSendImageCheck = AllImageFile;
}

// 촬영 의뢰 버튼 클릭 시 수행되는 함수
void PatientInfo::on_Camerastart_clicked()
{
    qDebug() << "촬영 시작";

    if(!AllSendImageCheck){
        QMessageBox:: critical(messagebox, "경고", "이미지 업로드 중입니다. "
                                           "잠시만 기다려주세요");
        return;
    }

    QString Photo;
    Ceph = ui->Ceph->isChecked();
    Pano = ui->Pano->isChecked();

    // Ceph, Pano의 체크박스 상태에 따라 서버로 촬영 타입 전송
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
        QMessageBox:: critical(messagebox, "경고", "CEPH/PANO 중 하나 이상을 선택하세요.");
        return;
    }

    // 촬영 의뢰 버튼 클릭 시 서버로 해당 환자의 정보를 보내줌
    QString Data = "SEN^SRQ<CR>" + patientID + "<CR>" + patientName + "|" + Photo;
    emit sendCameraPatient(Data);
}

// 진료 종료 버튼 클릭 시 촬영 의뢰 버튼 비활성화, 진료 시작 버튼 활성화
void PatientInfo::receiveEndSignal()
{
    ui->Camerastart->setDisabled(true);
    ui->Treatmentstart->setDisabled(false);
}
