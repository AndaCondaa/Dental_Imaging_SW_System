#include "controlpanel.h"
#include "ui_controlpanel.h"

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

    ui->panoButton->setStyleSheet("QRadioButton::indicator::unchecked {image: url(:/icon/PANO_DIS.png)}"
                                  "QRadioButton::indicator::checked {image: url(:/icon/PANO_ACT.png)};");
    ui->cephButton->setStyleSheet("QRadioButton::indicator::unchecked {image: url(:/icon/CEPH_DIS.png)}"
                                  "QRadioButton::indicator::checked {image: url(:/icon/CEPH_ACT.png)};");

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

    if (requestType == "PANO") {
        currentType = "PANO";
        ui->panoButton->setCheckable(true);
        ui->cephButton->setCheckable(false);
    } else if (requestType == "CEPH") {
        currentType = "CEPH";
        ui->panoButton->setCheckable(false);
        ui->cephButton->setCheckable(true);
    } else if (requestType == "BOTH") {
        currentType = "PANO";
        ui->panoButton->setCheckable(true);
        ui->cephButton->setCheckable(true);
    }
}

void ControlPanel::controlButtonClicked(QAbstractButton* button)
{
    switch (controlButtonGroup->id(button)) {
    case RESET:
        resetControl();
        break;
    case READY:
        if (!readyControl(true)) return;
        break;
    case START:
        if (!startControl(true)) return;
        break;
    case STOP:
        stopControl(true);
        break;
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
        if (!readyControl(false)) return;
        break;
    case START:
        if (!startControl(false)) return;
        break;
    case STOP:
        stopControl(false);
        break;
    }
}

void ControlPanel::resetControl()
{
//    checkTypeButton("NULL|BOTH");

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
    } else if (requestType == "BOTH") {
        ui->panoButton->setCheckable(true);
        ui->cephButton->setCheckable(true);
        ui->panoButton->setChecked(false);
        ui->cephButton->setChecked(false);
    }

    ui->resetButton->setEnabled(true);
    ui->readyButton->setEnabled(true);
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
}

bool ControlPanel::readyControl(bool signal)
{
    if (ui->panoButton->isChecked()) {
        currentType = "PANO";
        ui->cephButton->setCheckable(false);
    }
    else if (ui->cephButton->isChecked()) {
        currentType = "CEPH";
        ui->panoButton->setCheckable(false);
    } else {
        qDebug("%d : 타입을 선택하고 ready버튼을 누르세요.", __LINE__);
        return false;
    }
    if (currentPID == "NULL") {
        qDebug("%d : 촬영할 환자를 선택하고 ready버튼을 누르세요.", __LINE__);
        return false;
    }
    ui->readyButton->setEnabled(false);
    ui->startButton->setEnabled(true);

    if (signal)
        emit readySignal(currentType);

    return true;
}

bool ControlPanel::startControl(bool signal)
{
    QMessageBox startBox(QMessageBox::NoIcon, "START",
                         QString("%1의 %2 촬영을 시작하시겠습니까?").arg(currentPID, currentType),
                         QMessageBox::Yes|QMessageBox::No, this, Qt::Dialog);
    startBox.exec();

    if(startBox.clickedButton()->text() == "&No") return false;

    ui->resetButton->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);

    if (signal)
        emit startSignal();

    return true;
}

void ControlPanel::stopControl(bool signal)
{
    currentType = "NULL";

    ui->panoButton->setCheckable(false);
    ui->cephButton->setCheckable(false);

    ui->resetButton->setEnabled(true);
    ui->readyButton->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(false);

    if (signal)
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

void ControlPanel::finishSlot(QString pid)
{
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

void ControlPanel::deleteSlot(QString pid)
{
    if (currentPID == pid)
        currentPID = "NULL";
}
