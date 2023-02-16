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

// 제어명령 버튼그룹 인덱스
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


// 촬영타입에 따른 버튼 조작
// QString data: PID|TYPE (ex: P00001|PANO)
void ControlPanel::checkTypeButton(QString data)
{
    currentPID = data.split("|")[0];
    requestType = data.split("|")[1];

    if (requestType == "BOTH") {
        ui->panoButton->setEnabled(true);
        ui->cephButton->setEnabled(true);
    } else if (requestType == "PANO") {
        ui->panoButton->setEnabled(true);
        ui->cephButton->setEnabled(false);
    } else if (requestType == "CEPH") {
        ui->panoButton->setEnabled(false);
        ui->cephButton->setEnabled(true);
    }

    resetControl();
}

// 제어명령 요청에 따른 패킷 전송
// QAbstractButton* button: 제어명령 버튼 (RESET, READY, START, STOP)
void ControlPanel::controlButtonClicked(QAbstractButton* button)
{
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

// 제어명령 패킷 수신
// int buttonIdx : (0: RESET, 1: READY, 2: START, 3: STOP)
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

// RESET 동작
void ControlPanel::resetControl()
{   
    modeButtonGroup->setExclusive(false);
    if (requestType == "PANO") {
        ui->panoButton->setEnabled(true);
        ui->cephButton->setEnabled(false);

        ui->panoButton->setCheckable(true);
        ui->cephButton->setCheckable(false);

        ui->panoButton->setChecked(true);
        ui->cephButton->setChecked(false);
    } else if (requestType == "CEPH") {
        ui->panoButton->setEnabled(false);
        ui->cephButton->setEnabled(true);

        ui->panoButton->setCheckable(false);
        ui->cephButton->setCheckable(true);

        ui->panoButton->setChecked(false);
        ui->cephButton->setChecked(true);
    } else {
        ui->panoButton->setEnabled(true);
        ui->cephButton->setEnabled(true);

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

// READY 동작
bool ControlPanel::readyControl()
{
    if (currentPID == "NULL") {
        QMessageBox messageBox(QMessageBox::NoIcon, "NO TYPE",
                             QString("촬영환자를 선택해주세요."),
                             QMessageBox::Ok, 0, Qt::Window);
        messageBox.exec();
        return false;
    }

    if (ui->panoButton->isChecked()) {
        currentType = "PANO";
        ui->cephButton->setEnabled(false);
        ui->cephButton->setCheckable(false);
    }
    else if (ui->cephButton->isChecked()) {
        currentType = "CEPH";
        ui->panoButton->setEnabled(false);
        ui->panoButton->setCheckable(false);
    } else {
        QMessageBox messageBox(QMessageBox::NoIcon, "NO TYPE",
                             QString("촬영타입을 선택해주세요."),
                             QMessageBox::Ok, nullptr, Qt::Window);
        messageBox.exec();
        return false;
    }

    ui->readyButton->setEnabled(false);
    ui->startButton->setEnabled(true);

    emit readySignal(currentType);      // 촬영타입 전송

    return true;
}

// START 동작
void ControlPanel::startControl()
{
    ui->resetButton->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);

    emit startSignal(currentPID, currentType);   // 프레임데이터 수신용 쓰레드 준비 요청
}

// STOP 동작
void ControlPanel::stopControl()
{
    currentType = "NULL";

    ui->panoButton->setCheckable(false);
    ui->cephButton->setCheckable(false);

    ui->resetButton->setEnabled(true);
    ui->readyButton->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(false);

    emit stopSignal();      // 프레임데이터 수신용 쓰레드 중지 요청
}

// 촬영완료 동작 (PANO, CEPH)
void ControlPanel::shootingEndSlot(QString type)
{
    ui->resetButton->setEnabled(true);
    ui->readyButton->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
}

// SAVE 동작 후 버튼 리셋
void ControlPanel::saveSlot(QString)
{
    ui->panoButton->setCheckable(false);
    ui->cephButton->setCheckable(false);

    ui->resetButton->setEnabled(true);
    ui->readyButton->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
}

// 특정 환자의 촬영프로세스 종료
void ControlPanel::finishSlot(QString pid, QString type)
{
    Q_UNUSED(pid);
    Q_UNUSED(type);

    currentType = "NULL";
    requestType = "NULL";
    currentPID = "NULL";

    ui->panoButton->setEnabled(false);
    ui->cephButton->setEnabled(false);
    ui->panoButton->setCheckable(false);
    ui->cephButton->setCheckable(false);

    ui->resetButton->setEnabled(true);
    ui->readyButton->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
}

// GUI 스타일시트 설정
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
                                  "QPushButton:hover {"
                                  "background-color: rgb(250, 250, 250);"
                                  "border-style: outset;"
                                  "border-width: 3px;"
                                  "border-color: #ED8817;"
                                  "}"
                                  "QPushButton:checked {"
                                  "background-color: rgb(250, 250, 250);"
                                  "border-style: outset;"
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
                                  "QPushButton:hover {"
                                  "background-color: rgb(250, 250, 250);"
                                  "border-style: outset;"
                                  "border-width: 3px;"
                                  "border-color: #ED8817;"
                                  "}"
                                  "QPushButton:checked {"
                                  "background-color: rgb(250, 250, 250);"
                                  "border-style: outset;"
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
