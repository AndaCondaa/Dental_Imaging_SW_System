#include "patientstatusmanager.h"
#include "ui_patientstatusmanager.h"
#include <QTreeWidget>

PatientStatusManager::PatientStatusManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientStatusManager)
{
    ui->setupUi(this);

    QTreeWidgetItem* paymentRow = new QTreeWidgetItem;
        ui->waitPaymentTreeWidget->addTopLevelItem(paymentRow);
        paymentRow->setText(0, "P00001");
        paymentRow->setText(1, "김유선");
}

PatientStatusManager::~PatientStatusManager()
{
    delete ui;
}

//void PatientStatusManager::waitInfoSended(QString pidSended, QString nameSended){
//    pid = pidSended;
//    name = nameSended;
//    qDebug() << "pid, name: " << pid << ", " << name;

//    QTreeWidgetItem* row = new QTreeWidgetItem;
//        ui->waitTreatmentTreeWidget->addTopLevelItem(row);
//        row->setText(0, pid);
//        row->setText(1, name);
//        row->setText(2, "대기");
//}

void PatientStatusManager::waitInfoSended(QString waitInfoSended){
    qDebug()<<"대기리스트에 올릴 환자 정보: " << waitInfoSended;
    pid = waitInfoSended.split("<CR>")[1];
    QString data = waitInfoSended.split("<CR>")[2];
    name = data.split("|")[0];

    qDebug() << "pid, name: " << pid << ", " << name;

    QTreeWidgetItem* row = new QTreeWidgetItem;
        ui->waitTreatmentTreeWidget->addTopLevelItem(row);
        row->setText(0, pid);
        row->setText(1, name);
        row->setText(2, "대기");
}

//waitPaymentPID, waitPaymentName 전역변수로 만들어서 저장하고,
//수납 버튼이 눌리면(if문) qDebug로 삭제되는 pid와 이름 띄워주고
//treewidget에서 해당 데이터 삭제
void PatientStatusManager::on_waitPaymentTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{

}



void PatientStatusManager::on_waitTreatmentTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{

}


void PatientStatusManager::on_shootRequestPushButton_clicked()
{

}

