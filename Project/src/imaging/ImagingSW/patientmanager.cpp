/*
 * 프로그램명 : ImagingSW
 * 파일명 : patientmanager.cpp
 * 설명 : 대기 환자 및 촬영 중인 환자 정보 관리
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.02.13
 */


#include "patientmanager.h"
#include "ui_patientmanager.h"

#include <QMessageBox>

PatientManager::PatientManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientManager)
{
    ui->setupUi(this);
    ui->infoTableWidget->horizontalHeader()->setStretchLastSection(true);

    connect(ui->patientReadyButton, SIGNAL(clicked()), this, SLOT(readyButtonSlot()));
    connect(ui->finishButton, SIGNAL(clicked()), this, SLOT(finishButtonSlot()));
    connect(ui->waitTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(waitDoubleClicked(QTreeWidgetItem*,int)));

    settingStyleSheet();
}

PatientManager::~PatientManager()
{
    delete ui;
}

void PatientManager::receiveWaitList(int waitCount, QString data)   // data : pid|name|status<r>pid|name|status<r>...
{
    if (ui->infoTableWidget->item(0,0) != nullptr) {
        delete ui->infoTableWidget->takeItem(0,0);
        delete ui->infoTableWidget->takeItem(0,1);
        delete ui->infoTableWidget->takeItem(0,2);
        delete ui->infoTableWidget->takeItem(0,3);
        delete ui->infoTableWidget->takeItem(0,4);
    }
    ui->waitTreeWidget->clear();
    typeMap.clear();

    for (int i = 0; i < waitCount; i++) {
        QString newPatient = data.split("<r>")[i];

        QString type;
        if (newPatient.split("|")[2] == "BO")
            type = "BOTH";
        else if (newPatient.split("|")[2] == "PA")
            type = "PANO";
        else if (newPatient.split("|")[2] == "CE")
            type = "CEPH";

        QStringList insertList;
        insertList << newPatient.split("|")[0] << newPatient.split("|")[1] << type;
        receiveWaitPatient(insertList);
    }
}

void PatientManager::receiveWaitPatient(QStringList dataList)
{
    QString pid = dataList[0];
    QString name = dataList[1];
    QString type = dataList[2];

    typeMap.insert(pid, type);

    QTreeWidgetItem *waitItem = new QTreeWidgetItem(ui->waitTreeWidget);
    waitItem->setText(0, QString::number(typeMap.count()));
    waitItem->setText(1, pid);
    waitItem->setText(2, name);
    ui->waitTreeWidget->addTopLevelItem(waitItem);

    for (int i = 0; i < 3; i++) {
        waitItem->setTextAlignment(i, Qt::AlignHCenter);
        ui->waitTreeWidget->resizeColumnToContents(i);
        waitItem->setSizeHint(i, QSize(0, 25));
        waitItem->setTextAlignment(i, Qt::AlignHCenter|Qt::AlignVCenter);
    }

//    ui->waitTreeWidget->setCurrentItem(nullptr);    // 자동으로 처음 등록한 item이 currentitem으로 설정되지 않게 함
}


void PatientManager::readyButtonSlot()
{
    if (ui->waitTreeWidget->currentItem() == nullptr) return;

    emit sendPid(ui->waitTreeWidget->currentItem()->text(1));
}

void PatientManager::waitDoubleClicked(QTreeWidgetItem* item, int col)
{
    Q_UNUSED(col);

    emit sendPid(item->text(1));
}

void PatientManager::receivePatientInfo(QStringList dataList)           // pid -> name -> sex -> birth
{
    for (int i = 0; i < 5; i++) {
        delete ui->infoTableWidget->takeItem(0,i);
    }

    QTableWidgetItem *pid = new QTableWidgetItem(dataList[0]);
    QTableWidgetItem *type = new QTableWidgetItem(typeMap.value(dataList[0]));
    QTableWidgetItem *name = new QTableWidgetItem(dataList[1]);
    QTableWidgetItem *sex = new QTableWidgetItem(dataList[2]);
    QTableWidgetItem *birth = new QTableWidgetItem(dataList[3]);

    pid->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    type->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    name->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    sex->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    birth->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

    ui->infoTableWidget->setItem(0, 0, pid);
    ui->infoTableWidget->setItem(0, 1, type);
    ui->infoTableWidget->setItem(0, 2, name);
    ui->infoTableWidget->setItem(0, 3, sex);
    ui->infoTableWidget->setItem(0, 4, birth);

    shootingStatus = "NULL";
    ui->finishButton->setEnabled(true);

    emit sendType(dataList[0] + "|" + typeMap.value(dataList[0]));
    emit sendPidToImagingManager(dataList[0]);
}

void PatientManager::saveSlot(QString data) // pid|type
{
//    QString savePid = data.split("|")[0];
    QString saveType = data.split("|")[1];

    if (saveType == "PANO") {
        if (shootingStatus == "CEPH")
            shootingStatus = "BOTH";
        else
            shootingStatus = "PANO";
    } else if (saveType == "CEPH") {
        if (shootingStatus == "PANO")
            shootingStatus = "BOTH";
        else
            shootingStatus = "CEPH";
    }
}

void PatientManager::finishButtonSlot()
{
    if (ui->infoTableWidget->item(0,0) == nullptr) {
        return;
    }

    QString pid = ui->infoTableWidget->item(0,0)->text();
    QString requestType = typeMap.value(pid);
    QString type;
    if (requestType == "BOTH") type = "BO";
    else if (requestType == "PANO") type = "PA";
    else if (requestType == "CEPH") type = "CE";

    QMessageBox finishBox(QMessageBox::NoIcon, "FINISH",
                          "", QMessageBox::Yes|QMessageBox::No,
                          this, Qt::Dialog);

    if (shootingStatus == "BOTH") {
        finishBox.setText(QString("%1 환자의 촬영을 완료합니까?").arg(pid));
    } else {    // NULL, PANO, CEPH
        if (shootingStatus == typeMap.value(pid)) {
            finishBox.setText(QString("%1 환자의 촬영을 완료합니까?").arg(pid));
        } else {
            finishBox.setIcon(QMessageBox::Warning);
            finishBox.setText(QString("%1 환자의 요청된 촬영이 모두 수행되지 않았습니다. \n"
                                      "그래도 완료하십니까?").arg(pid));
        }
    }

    finishBox.exec();
    if(finishBox.clickedButton()->text() == "&No") return;

    typeMap.remove(pid);

    foreach (auto item, ui->waitTreeWidget->findItems(pid, Qt::MatchExactly, 1)) {
        delete ui->waitTreeWidget->takeTopLevelItem(ui->waitTreeWidget->indexOfTopLevelItem(item));
    }

    for (int idx = 0; idx < typeMap.count() ; idx++) {
        ui->waitTreeWidget->topLevelItem(idx)->setText(0, QString::number(idx+1));
    }

    for (int i = 0; i < 5; i++) {
        delete ui->infoTableWidget->takeItem(0,i);
    }

    shootingStatus = "NULL";
    ui->finishButton->setEnabled(false);

    emit finishSignal(pid, type);
}

void PatientManager::settingStyleSheet()
{
//    ui->patientReadyButton->setStyleSheet("QPushButton { "
//                                          "background-color:rgb(200,200,200);"
//                                          "border-radius:10px;"
//                                          "color:#ffffff;"
//                                          "}"
//                                          "QPushButton:hover { "
//                                          "background-color: rgb(241,156,72);"
//                                          "color:#ffffff;"
//                                          "}"
//                                          "QPushButton:disabled { "
//                                          "background-color: rgb(100,100,100);"
//                                          "}");

//    ui->finishButton->setStyleSheet("QPushButton { "
//                                    "background-color:rgb(200,200,200);"
//                                    "border-radius:10px;"
//                                    "color:#ffffff;"
//                                    "}"
//                                    "QPushButton:hover { "
//                                    "background-color: rgb(241,156,72);"
//                                    "color:#ffffff;"
//                                    "}"
//                                    "QPushButton:disabled { "
//                                    "background-color: rgb(100,100,100);"
//                                    "}");

    ui->patientReadyButton->setStyleSheet("QPushButton { "
                                          "background-color:rgb(241,156,72);"
                                          "border-radius:10px;"
                                          "color:#ffffff;"
                                          "}"
                                          "QPushButton:hover { "
                                          "background-color: rgb(241,156,72);"
                                          "font-weight:bold;"
                                          "color:#ffffff;"
                                          "}"
                                          "QPushButton:disabled { "
                                          "background-color: rgb(100,100,100);"
                                          "}");

    ui->finishButton->setStyleSheet("QPushButton { "
                                    "background-color:rgb(255,255,255);"
                                    "border-style: outset;"
                                    "border-width: 2px;"
                                    "border-radius: 10px;"
                                    "border-color: orange;"
                                    "color:orange;"
                                    "}"
                                    "QPushButton:hover { "
                                    "background-color: rgb(255,255,255);"
                                    "border-style: outset;"
                                    "border-width: 3px;"
                                    "border-radius: 10px;"
                                    "border-color: orange;"
                                    "font-weight: bold;"
                                    "color:orange;"
                                    "}"
                                    "QPushButton:disabled { "
                                    "background-color: rgb(100,100,100);"
                                    "border-radius: 10px;"
                                    "border-width: 10px;"
                                    "border-color: rgb(241,156,72);"
                                    "}");
}
