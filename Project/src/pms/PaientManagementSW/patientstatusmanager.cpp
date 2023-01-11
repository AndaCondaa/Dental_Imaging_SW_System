#include "patientstatusmanager.h"
#include "ui_patientstatusmanager.h"

PatientStatusManager::PatientStatusManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientStatusManager)
{
    ui->setupUi(this);
}

PatientStatusManager::~PatientStatusManager()
{
    delete ui;
}
