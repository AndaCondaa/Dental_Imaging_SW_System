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

    ui->panoButton->setStyleSheet("QPushButton {"
                                  "border-style: solid;"
                                  "border-width: 1px;"
                                  "border-color: gray;"
                                  "}"
                                  "QPushButton:checked {"
                                  "border-width: 3px;"
                                  "border-color: blue;"
                                  "}");
    ui->cephButton->setStyleSheet("QPushButton {"
                                  "border-style: solid;"
                                  "border-width: 1px;"
                                  "border-color: gray;"
                                  "}"
                                  "QPushButton:checked {"
                                  "border-width: 3px;"
                                  "border-color: blue;"
                                  "}");

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
        if (!readyControl()) return;
        break;
    case START:
        startControl();
        break;
    case STOP:
        stopControl();
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
    ui->panoButton->setCheckable(true);
    ui->cephButton->setCheckable(true);

    if (requestType == "PANO") {
        ui->panoButton->setChecked(true);
        ui->cephButton->setChecked(false);
    } else if (requestType == "CEPH") {
        ui->panoButton->setChecked(false);
        ui->cephButton->setChecked(true);
    } else if (requestType == "BOTH") {
        ui->panoButton->setChecked(false);
        ui->cephButton->setChecked(false);
    }

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
