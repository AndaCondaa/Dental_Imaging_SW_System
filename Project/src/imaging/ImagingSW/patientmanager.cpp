#include "patientmanager.h"
#include "ui_patientmanager.h"

#include <QMessageBox>

PatientManager::PatientManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientManager)
{
    ui->setupUi(this);
    ui->waitTreeWidget->hideColumn(1);
    ui->infoTableWidget->horizontalHeader()->setStretchLastSection(true);

    connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(deletePatient()));
    connect(ui->patientReadyButton, SIGNAL(clicked()), this, SLOT(slotPatientReady()));
    connect(ui->waitTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(waitDoubleClicked(QTreeWidgetItem*,int)));

//    QStringList dataList;
//    dataList << "P00001" << "김유선" << "CEPH";
//    receiveWaitPatient(dataList);
//    dataList.clear();
//    dataList << "P00004" << "김도예" << "BOTH";
//    receiveWaitPatient(dataList);
//    dataList.clear();
//    dataList << "P00005" << "김영희" << "PANO";
//    receiveWaitPatient(dataList);
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
    QMessageBox deleteBox(QMessageBox::Warning, "DELETE",
                          QString("%1 환자를 대기목록에서 삭제하시겠습니까?").arg(ui->waitTreeWidget->currentItem()->text(2)),
                          QMessageBox::Yes | QMessageBox::No, this, Qt::Dialog);
    deleteBox.exec();

    if (deleteBox.clickedButton()->text() == "&No") return;


    if (ui->waitTreeWidget->currentItem()->text(1) == ui->infoTableWidget->item(0,0)->text()) {
        delete ui->infoTableWidget->takeItem(0,0);
        delete ui->infoTableWidget->takeItem(0,1);
        delete ui->infoTableWidget->takeItem(0,2);
        delete ui->infoTableWidget->takeItem(0,3);
        delete ui->infoTableWidget->takeItem(0,4);
    }

    typeMap.remove(ui->waitTreeWidget->currentItem()->text(1));
    delete ui->waitTreeWidget->takeTopLevelItem(ui->waitTreeWidget->currentIndex().row());

    for (int idx = 0; idx < typeMap.count() ; idx++) {
        ui->waitTreeWidget->topLevelItem(idx)->setText(0, QString::number(idx+1));
    }
}

void PatientManager::slotPatientReady()
{
    emit sendPid(ui->waitTreeWidget->currentItem()->text(1));
}

void PatientManager::receivePatientInfo(QStringList dataList)           // pid -> name -> sex -> birth
{
    delete ui->infoTableWidget->item(0,0);
    delete ui->infoTableWidget->item(0,1);
    delete ui->infoTableWidget->item(0,2);
    delete ui->infoTableWidget->item(0,3);
    delete ui->infoTableWidget->item(0,4);

    QTableWidgetItem *pid = new QTableWidgetItem(dataList[0]);
    QTableWidgetItem *type = new QTableWidgetItem(typeMap.value(dataList[0]));
    QTableWidgetItem *name = new QTableWidgetItem(dataList[1]);
    QTableWidgetItem *sex = new QTableWidgetItem(dataList[2]);
    QTableWidgetItem *birth = new QTableWidgetItem(dataList[3]);

    pid->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    type->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    name->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    sex->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    birth->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

    ui->infoTableWidget->setItem(0, 0, pid);
    ui->infoTableWidget->setItem(0, 1, type);
    ui->infoTableWidget->setItem(0, 2, name);
    ui->infoTableWidget->setItem(0, 3, sex);
    ui->infoTableWidget->setItem(0, 4, birth);

    emit sendType(dataList[0] + "|" + typeMap.value(dataList[0]));
    emit sendPidToImagingManager(dataList[0]);
}

void PatientManager::waitDoubleClicked(QTreeWidgetItem*, int)
{
    emit sendPid(ui->waitTreeWidget->currentItem()->text(1));
}

void PatientManager::finishSlot(QString)
{
    delete ui->infoTableWidget->item(0,0);
    delete ui->infoTableWidget->item(0,1);
    delete ui->infoTableWidget->item(0,2);
    delete ui->infoTableWidget->item(0,3);
    delete ui->infoTableWidget->item(0,4);
}
