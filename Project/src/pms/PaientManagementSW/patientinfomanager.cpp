#include "patientinfomanager.h"
#include "ui_patientinfomanager.h"

PatientInfoManager::PatientInfoManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientInfoManager)
{
    ui->setupUi(this);
}

PatientInfoManager::~PatientInfoManager()
{
    delete ui;
}
