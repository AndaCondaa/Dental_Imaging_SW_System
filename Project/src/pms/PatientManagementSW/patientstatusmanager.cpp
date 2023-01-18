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
    qDebug("%d", __LINE__);
    QTreeWidgetItem* row = new QTreeWidgetItem;
    ui->waitTreatmentTreeWidget->addTopLevelItem(row);
    row->setText(0, pid);
    row->setText(1, name);
    row->setText(2, "대기중");
    qDebug("%d", __LINE__);
}

//waitPaymentPID, waitPaymentName 전역변수로 만들어서 저장하고,
//수납 버튼이 눌리면(if문) qDebug로 삭제되는 pid와 이름 띄워주고
//treewidget에서 해당 데이터 삭제
void PatientStatusManager::on_waitPaymentTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{

}



void PatientStatusManager::on_waitTreatmentTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    pid = item->text(0);
    qDebug() << "clicked pid: " << pid;

    name = item->text(1);
    qDebug() << "clicked name: " << name;



}


void PatientStatusManager::on_shootRequestPushButton_clicked()
{
    QString imageType;

    if(ui->cephCheckBox->isChecked() == true && ui->panoCheckBox->isChecked() == true)
    {
        imageType = "BOTH";

    }
    else if(ui->cephCheckBox->isChecked() == true && ui->panoCheckBox->isChecked() == false)
    {
        imageType = "CEPH";
    }
    else if(ui->cephCheckBox->isChecked() == false && ui->panoCheckBox->isChecked() == true)
    {
        imageType = "PANO";
    }
    else
        return;

//클릭한 아이템의 3번째 컬럼을 대기중에서 촬영중으로 바꿔야함
    //ui->waitTreatmentTreeWidget->setItemWidget()

    int currentRow = ui->waitTreatmentTreeWidget->currentIndex().row();
    qDebug() << "currentRow: " << currentRow;



    //ui->waitTreatmentTreeWidget->setCurrentItem(2)

    //근데 서버 왜 죽지->정연이 모듈이 안열려있어서
    QString shootRequestInfo = "SRQ<CR>" + pid + "<CR>" + name + "|" + imageType;
    qDebug() << shootRequestInfo;
    emit sendRequest(shootRequestInfo);
    //슛리퀘스트 네트워크매니저로 보내고 메인윈도우에서 연결해서 서버로 보내고 서버에서는 촬영요청메세지 촬영SW와 뷰어SW와 환자관리SW에 모두 보내주기

}

