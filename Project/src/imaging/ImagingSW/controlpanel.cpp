/*
 * 프로그램명 : ImagingSW
 * 파일명 : controlpanel.cpp
 * 설명 : CT 장비 제어동작 구현
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.02.13
 */


#include "controlpanel.h"
#include "ui_controlpanel.h"

#include <QButtonGroup>
#include <QMessageBox>

#define RESET 0
#define READY 1
#define START 2
#define STOP 3

ControlPanel::ControlPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlPanel)
{
    ui->setupUi(this);
    settingStyleSheet();

    modeButtonGroup = new QButtonGroup(this);
    modeButtonGroup->addButton(ui->panoButton);
    modeButtonGroup->addButton(ui->cephButton);
    modeButtonGroup->setExclusive(true);

    controlButtonGroup = new QButtonGroup(this);
    controlButtonGroup->addButton(ui->resetButton, RESET);
    controlButtonGroup->addButton(ui->readyButton, READY);
    controlButtonGroup->addButton(ui->startButton, START);
    controlButtonGroup->addButton(ui->stopButton, STOP);

    connect(controlButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(controlButtonClicked(QAbstractButton*)));
}

ControlPanel::~ControlPanel()
{
    delete ui;
}

//촬영요청 타입확인 후 , 촬영타입에 따라서 타입버튼 조작
void ControlPanel::checkTypeButton(QString data)
{
    currentPID = data.split("|")[0];
    requestType = data.split("|")[1];

    //******************************************************************************
    // 대기환자 변경할 때마다, 바뀌는지 체크하기
    resetControl();
//    modeButtonGroup->setExclusive(false);
//    if (requestType == "PANO") {
//        ui->panoButton->setCheckable(true);
//        ui->cephButton->setCheckable(false);
//        ui->panoButton->setChecked(true);
//        ui->cephButton->setChecked(false);
//    } else if (requestType == "CEPH") {
//        ui->panoButton->setCheckable(false);
//        ui->cephButton->setCheckable(true);
//        ui->panoButton->setChecked(false);
//        ui->cephButton->setChecked(true);
//    } else if (requestType == "BOTH") {
//        ui->panoButton->setCheckable(true);
//        ui->cephButton->setCheckable(true);
//        ui->panoButton->setChecked(false);
//        ui->cephButton->setChecked(false);
//    }
//    modeButtonGroup->setExclusive(false);
    //*******************************************************************************
}

void ControlPanel::controlButtonClicked(QAbstractButton* button)
{
//    switch (controlButtonGroup->id(button)) {
//    case RESET:
//        resetControl();
//        break;
//    case READY:
//        if (!readyControl()) return;
//        break;
//    case START:
//        startControl();
//        break;
//    case STOP:
//        stopControl();
//        break;
//    }
    if (ui->panoButton->isChecked()) {
        currentType = "PANO";
    } else if (ui->cephButton->isChecked()) {
        currentType = "CEPH";
    } else {
        currentType = "NULL";
    }

    if (controlButtonGroup->id(button) == 1 && currentType == "NULL") {
        QMessageBox messageBox(QMessageBox::NoIcon, "NO TYPE",
                             QString("촬영타입을 선택해주세요."),
                             QMessageBox::Ok, this, Qt::Window);
        messageBox.exec();
        return;
    }

    emit buttonSignal(controlButtonGroup->id(button), currentPID + "|" + currentType);
}

void ControlPanel::receiveButtonControl(int buttonIdx)
{
    switch (buttonIdx) {
    case RESET:
        resetControl();
        break;
    case READY:
        if (!readyControl()) return;
        break;
    case START:
        startControl();
        break;
    case STOP:
        stopControl();
        break;
    }
}

void ControlPanel::resetControl()
{   
    modeButtonGroup->setExclusive(false);
    if (requestType == "PANO") {
        ui->panoButton->setCheckable(true);
        ui->cephButton->setCheckable(false);

        ui->panoButton->setChecked(true);
        ui->cephButton->setChecked(false);
    } else if (requestType == "CEPH") {
        ui->panoButton->setCheckable(false);
        ui->cephButton->setCheckable(true);

        ui->panoButton->setChecked(false);
        ui->cephButton->setChecked(true);
    } else {
        ui->panoButton->setCheckable(true);
        ui->cephButton->setCheckable(true);

        ui->panoButton->setChecked(false);
        ui->cephButton->setChecked(false);
    }
    modeButtonGroup->setExclusive(true);

    ui->resetButton->setEnabled(true);
    ui->readyButton->setEnabled(true);
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
}

bool ControlPanel::readyControl()
{
    if (currentPID == "NULL") {
        QMessageBox messageBox(QMessageBox::NoIcon, "NO TYPE",
                             QString("촬영환자를 선택해주세요."),
                             QMessageBox::Ok, this, Qt::Window);
        messageBox.exec();
        return false;
    }

    if (ui->panoButton->isChecked()) {
        currentType = "PANO";
        ui->cephButton->setCheckable(false);
    }
    else if (ui->cephButton->isChecked()) {
        currentType = "CEPH";
        ui->panoButton->setCheckable(false);
    } else {
        QMessageBox messageBox(QMessageBox::NoIcon, "NO TYPE",
                             QString("촬영타입을 선택해주세요."),
                             QMessageBox::Ok, this, Qt::Window);
        messageBox.exec();
        return false;
    }

    ui->readyButton->setEnabled(false);
    ui->startButton->setEnabled(true);

    emit readySignal(currentType);

    return true;
}

void ControlPanel::startControl()
{
    ui->resetButton->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);

    emit startSignal(currentPID, currentType);
}

void ControlPanel::stopControl()
{
    currentType = "NULL";

    ui->panoButton->setCheckable(false);
    ui->cephButton->setCheckable(false);

    ui->resetButton->setEnabled(true);
    ui->readyButton->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(false);

    emit stopSignal();
}

void ControlPanel::shootingEndSlot(QString type)
{
    ui->resetButton->setEnabled(true);
    ui->readyButton->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
}

void ControlPanel::saveSlot(QString)
{
    ui->panoButton->setCheckable(false);
    ui->cephButton->setCheckable(false);

    ui->resetButton->setEnabled(true);
    ui->readyButton->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
}

void ControlPanel::finishSlot(QString pid, QString type)
{
    Q_UNUSED(pid);
    Q_UNUSED(type);

    currentType = "NULL";
    requestType = "NULL";
    currentPID = "NULL";

    ui->panoButton->setCheckable(false);
    ui->cephButton->setCheckable(false);

    ui->resetButton->setEnabled(true);
    ui->readyButton->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
}

void ControlPanel::settingStyleSheet()
{
    ui->panoButton->setStyleSheet("QPushButton:disabled {"
                                  "background-color: rgb(150, 150, 150);"
                                  "border-radius: 10px;"
                                  "border-style: solid;"
                                  "}"
                                  "QPushButton {"
                                  "background-color: rgb(200, 200, 200);"
                                  "border-radius: 10px;"
                                  "border-style: solid;"
                                  "}"
                                  "QPushButton:checked {"
                                  "background-color: rgb(250, 250, 250);"
                                  "border-width: 3px;"
                                  "border-color: #ED8817;"
                                  "}");
    ui->cephButton->setStyleSheet("QPushButton:disabled {"
                                  "background-color: rgb(150, 150, 150);"
                                  "border-radius: 10px;"
                                  "border-style: solid;"
                                  "}"
                                  "QPushButton {"
                                  "background-color: rgb(200, 200, 200);"
                                  "border-radius: 10px;"
                                  "border-style: solid;"
                                  "}"
                                  "QPushButton:checked {"
                                  "background-color: rgb(250, 250, 250);"
                                  "border-width: 3px;"
                                  "border-color: #ED8817;"
                                  "}");

    ui->resetButton->setStyleSheet("QPushButton:disabled {"
                                   "background-color: rgb(150, 150, 150);"
                                   "border-radius: 10px;"
                                   "border-style: solid;"
                                   "}"
                                   "QPushButton {"
                                   "background-color: rgb(200, 200, 200);"
                                   "border-radius: 10px;"
                                   "border-style: solid;"
                                   "}"
                                   "QPushButton:hover {"
                                   "background-color:rgb(241,156,72);"
                                   "border-radius:10px;"
                                   "color:#ffffff;"
                                   "}");

    ui->readyButton->setStyleSheet("QPushButton:disabled {"
                                   "background-color: rgb(150, 150, 150);"
                                   "border-radius: 10px;"
                                   "border-style: solid;"
                                   "}"
                                   "QPushButton {"
                                   "background-color: rgb(200, 200, 200);"
                                   "border-radius: 10px;"
                                   "border-style: solid;"
                                   "}"
                                   "QPushButton:hover {"
                                   "background-color:rgb(241,156,72);"
                                   "border-radius:10px;"
                                   "color:#ffffff;"
                                   "}");

    ui->startButton->setStyleSheet("QPushButton:disabled {"
                                   "background-color: rgb(150, 150, 150);"
                                   "border-radius: 10px;"
                                   "border-style: solid;"
                                   "}"
                                   "QPushButton {"
                                   "background-color: rgb(200, 200, 200);"
                                   "border-radius: 10px;"
                                   "border-style: solid;"
                                   "}"
                                   "QPushButton:hover {"
                                   "background-color:rgb(241,156,72);"
                                   "border-radius:10px;"
                                   "color:#ffffff;"
                                   "}");

    ui->stopButton->setStyleSheet("QPushButton:disabled {"
                                  "background-color: rgb(150, 150, 150);"
                                  "border-radius: 10px;"
                                  "border-style: solid;"
                                  "}"
                                  "QPushButton {"
                                  "background-color: rgb(200, 200, 200);"
                                  "border-radius: 10px;"
                                  "border-style: solid;"
                                  "}"
                                  "QPushButton:hover {"
                                  "background-color:rgb(241,156,72);"
                                  "border-radius:10px;"
                                  "color:#ffffff;"
                                  "}");

}
