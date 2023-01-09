#include "patientmanager.h"
#include "ui_patientmanager.h"

PatientManager::PatientManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientManager)
{
    ui->setupUi(this);
}

PatientManager::~PatientManager()
{
    delete ui;
}
