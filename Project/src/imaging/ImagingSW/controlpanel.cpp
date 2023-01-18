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
