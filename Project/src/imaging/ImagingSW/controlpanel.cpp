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

    ui->panoButton->setStyleSheet("QRadioButton::indicator::checked {image: url(:/icon/PANO_ACT.png)}"
                                  "QRadioButton::indicator::unchecked {image: url(:/icon/PANO_DIS.png)};");
    ui->cephButton->setStyleSheet("QRadioButton::indicator::checked {image: url(:/icon/CEPH_ACT.png)}"
                                  "QRadioButton::indicator::unchecked {image: url(:/icon/CEPH_DIS.png)};");

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
void ControlPanel::checkTypeButton(QString type)
{
    if (type == "PANO") {
        currentType = "PANO";
        ui->panoButton->setCheckable(true);
        ui->cephButton->setCheckable(false);
        ui->panoButton->setChecked(true);
        ui->cephButton->setChecked(false);
    } else if (type == "CEPH") {
        currentType = "CEPH";
        ui->panoButton->setCheckable(false);
        ui->cephButton->setCheckable(true);
        ui->panoButton->setChecked(false);
        ui->cephButton->setChecked(true);
    } else if (type == "BOTH") {
        currentType = "PANO";
        ui->panoButton->setCheckable(true);
        ui->cephButton->setCheckable(true);
        ui->panoButton->setChecked(true);
        ui->cephButton->setChecked(false);
    }
}

void ControlPanel::controlButtonClicked(QAbstractButton* button)
{
    switch (controlButtonGroup->id(button)) {
    case RESET:
        resetButtonClicked();
        break;
    case READY:
        if (!readyButtonClicked()) return;
        break;
    case START:
        startButtonClicked();
        break;
    case STOP:
        stopButtonClicked();
        break;
    }

    emit buttonSignal(controlButtonGroup->id(button), currentType);
}

void ControlPanel::receiveButtonControl(int buttonIdx)
{
    switch (buttonIdx) {
    case RESET:
        resetButtonClicked();
        break;
    case READY:
        readyButtonClicked();
        break;
    case START:
        startButtonClicked();
        break;
    case STOP:
        stopButtonClicked();
        break;
    }
}

void ControlPanel::resetButtonClicked()
{
    checkTypeButton("BOTH");

    ui->resetButton->setEnabled(true);
    ui->readyButton->setEnabled(true);
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
}

bool ControlPanel::readyButtonClicked()
{
    if (ui->panoButton->isChecked()) {
        currentType = "PANO";
        ui->panoButton->setChecked(true);
        ui->cephButton->setChecked(false);
        ui->cephButton->setCheckable(false);
    }
    else if (ui->cephButton->isChecked()) {
        currentType = "CEPH";
        ui->cephButton->setChecked(true);
        ui->panoButton->setChecked(false);
        ui->panoButton->setCheckable(false);
    }
    else {
        qDebug("%d : 타입을 선택하고 ready버튼을 누르세요.", __LINE__);
        return false;
    }

    ui->readyButton->setEnabled(false);
    ui->startButton->setEnabled(true);

    return true;
}

void ControlPanel::startButtonClicked()
{
    QMessageBox startBox(QMessageBox::NoIcon, "START",
                         QString("%1 촬영을 시작하시겠습니까?").arg(currentType),
                         QMessageBox::Yes|QMessageBox::No, this, Qt::Dialog);
    startBox.exec();

    if(startBox.clickedButton()->text() == "&No") return;

    ui->resetButton->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);

    emit startSignal();
}

void ControlPanel::stopButtonClicked()
{
    ui->resetButton->setEnabled(true);
    ui->readyButton->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
    emit stopSignal();
}
