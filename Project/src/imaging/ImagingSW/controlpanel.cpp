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
    emit buttonSignal(buttonGroup->id(button));     // NetworkManager::receiveButtonSignal()
}
