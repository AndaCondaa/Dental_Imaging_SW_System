#include "controlpanel.h"
#include "ui_controlpanel.h"

#define RESET 0
#define READY 1
#define START 2
#define STOP 3

ControlPanel::ControlPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlPanel)
{
    ui->setupUi(this);
    ui->panoButton->setStyleSheet("background-color: rgb(109, 109, 109)");
    ui->cephButton->setStyleSheet("background-color: rgb(109, 109, 109)");

    buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->resetButton, RESET);
    buttonGroup->addButton(ui->readyButton, READY);
    buttonGroup->addButton(ui->startButton, START);
    buttonGroup->addButton(ui->stopButton, STOP);

    connect(buttonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(controlButtonClicked(QAbstractButton*)));
}

ControlPanel::~ControlPanel()
{
    delete ui;
}

void ControlPanel::controlButtonClicked(QAbstractButton* button)
{
    QString modality;
    if (ui->panoButton->isChecked() && !ui->cephButton->isChecked()) {
        modality = "PANO";
    } else if (!ui->panoButton->isChecked() && ui->cephButton->isChecked()) {
        modality = "CEPH";
    } else {
        // 아무것도 선택하지 않았을 경우
    }
    emit buttonSignal(buttonGroup->id(button), modality);     // NetworkManager::receiveButtonSignal()
}

void ControlPanel::receiveButtonControl(int buttonIdx)
{
    switch (buttonIdx) {
    case RESET:

        break;
    case READY:

        break;
    case START:

        break;
    case STOP:

        break;
    }
}

void ControlPanel::checkTypeButton(QString type)
{
    if (type == "PANO") {
        ui->panoButton->setStyleSheet("background-color: rgb(0, 150, 0)");         // 녹색
        ui->cephButton->setStyleSheet("background-color: rgb(109, 109, 109)");
    } else if (type == "CEPH") {
        ui->panoButton->setStyleSheet("background-color: rgb(109, 109, 109)");
        ui->cephButton->setStyleSheet("background-color: rgb(0, 150, 0)");
    } else if (type == "BOTH") {
        ui->panoButton->setStyleSheet("background-color: rgb(0, 150, 0)");
        ui->cephButton->setStyleSheet("background-color: rgb(196, 183, 59)");
    }
}
