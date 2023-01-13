#include "patientmanager.h"
#include "ui_patientmanager.h"

PatientManager::PatientManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientManager)
{
    ui->setupUi(this);
    ui->waitTreeWidget->hideColumn(1);

    connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(deletePatient()));
    connect(ui->patientReadyButton, SIGNAL(clicked()), this, SLOT(slotPatientReady()));
}

PatientManager::~PatientManager()
{
    delete ui;
}

void PatientManager::receiveWaitPatient(QStringList dataList)
{
    QString pid = dataList[0];
    QString name = dataList[1];
    QString type = dataList[2];

    typeMap.insert(pid, type);

    QTreeWidgetItem *waitItem = new QTreeWidgetItem(ui->waitTreeWidget);
    waitItem->setText(0, QString::number(typeMap.count()));
    waitItem->setText(1, pid);
    waitItem->setText(2, name);
    ui->waitTreeWidget->addTopLevelItem(waitItem);

    waitItem->setTextAlignment(2, Qt::AlignHCenter);
    ui->waitTreeWidget->resizeColumnToContents(0);
}

void PatientManager::deletePatient()
{
    typeMap.remove(ui->waitTreeWidget->currentItem()->text(1));
    delete ui->waitTreeWidget->takeTopLevelItem(ui->waitTreeWidget->currentIndex().row());

    for (int idx = 0; idx < typeMap.count() ; idx++) {
        ui->waitTreeWidget->topLevelItem(idx)->setText(0, QString::number(idx+1));
    }
}

void PatientManager::slotPatientReady()
{
    emit sendPid(ui->waitTreeWidget->current)
}
