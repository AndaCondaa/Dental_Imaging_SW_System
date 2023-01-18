#include "patientinfo.h"
#include "ui_patientinfo.h"

PatientInfo::PatientInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientInfo)
{
    ui->setupUi(this);
}

PatientInfo::~PatientInfo()
{
    delete ui;
}
