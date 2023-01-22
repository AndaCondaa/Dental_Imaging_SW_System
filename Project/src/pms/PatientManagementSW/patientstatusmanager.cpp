#include "patientstatusmanager.h"
#include "ui_patientstatusmanager.h"
#include <QTreeWidget>
#include <QMessageBox>

PatientStatusManager::PatientStatusManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientStatusManager)
{
    ui->setupUi(this);

//    QTreeWidgetItem* paymentRow = new QTreeWidgetItem;
//    ui->waitPaymentTreeWidget->addTopLevelItem(paymentRow);
//    paymentRow->setText(0, "P00001");
//    paymentRow->setText(1, "김유선");
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
    treatPID = waitInfoSended.split("<CR>")[1];
    QString data = waitInfoSended.split("<CR>")[2];
    treatName = data.split("|")[0];

    qDebug() << "pid, name: " << treatPID << ", " << treatName;
    qDebug("%d", __LINE__);
    QTreeWidgetItem* row = new QTreeWidgetItem;
    ui->waitTreatmentTreeWidget->addTopLevelItem(row);
    row->setText(0, treatPID);
    row->setText(1, treatName);
    row->setText(2, "대기중");
    qDebug("%d", __LINE__);
}

//waitPaymentPID, waitPaymentName 전역변수로 만들어서 저장하고,
//수납 버튼이 눌리면(if문) qDebug로 삭제되는 pid와 이름 띄워주고
//treewidget에서 해당 데이터 삭제
void PatientStatusManager::on_waitPaymentTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    payPID = item->text(0);
    qDebug() << "clicked pid: " << payPID;

    payName = item->text(1);
    qDebug() << "clicked name: " << payName;

    selectedPayRow = item;
}
//이제 지불버튼클릭하면 사라지는거 만들기


void PatientStatusManager::on_waitTreatmentTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    treatPID = item->text(0);
    qDebug() << "clicked pid: " << treatPID;

    treatName = item->text(1);
    qDebug() << "clicked name: " << treatName;

    selectedTreatRow = item;

}

void PatientStatusManager::on_paymentPushButton_clicked()
{
    if(selectedPayRow != nullptr)
    {                                       //아이템이 있을 때
        //int i = ui->waitPaymentTreeWidget->currentIndex().row();           //i는 현재 인덱스 줄 번호
        ui->waitPaymentTreeWidget->takeTopLevelItem(ui->waitPaymentTreeWidget->indexOfTopLevelItem(selectedPayRow));

        ui->waitPaymentTreeWidget->update();                               //treeWidget 업데이트
    }

}

void PatientStatusManager::on_shootRequestPushButton_clicked()
{
    qDebug("%d",__LINE__);
    QString imageType;

    if(ui->cephCheckBox->isChecked() == true && ui->panoCheckBox->isChecked() == true)
    {
        imageType = "BOTH";
        qDebug("%d",__LINE__);
    }
    else if(ui->cephCheckBox->isChecked() == true && ui->panoCheckBox->isChecked() == false)
    {
        imageType = "CEPH";
        qDebug("%d",__LINE__);
    }
    else if(ui->cephCheckBox->isChecked() == false && ui->panoCheckBox->isChecked() == true)
    {
        imageType = "PANO";
        qDebug("%d",__LINE__);
    }
    else
    {
        qDebug("%d",__LINE__);
        QMessageBox::critical(this, tr("경고"), \
                              tr("CEPH/PANO 중 하나를 이상을 선택하세요."));

        return;
    }

    //클릭한 아이템의 3번째 컬럼을 대기중에서 촬영중으로 바꿔야함
    //ui->waitTreatmentTreeWidget->setItemWidget()

    int currentRow = ui->waitTreatmentTreeWidget->currentIndex().row();
    qDebug() << "currentRow: " << currentRow;
    qDebug("%d",__LINE__);
    selectedTreatRow->setText(2,"촬영중");

    //ui->waitTreatmentTreeWidget->setCurrentItem(2)

    //근데 서버 왜 죽지->정연이 모듈이 안열려있어서
    QString shootRequestInfo = "SRQ<CR>" + treatPID + "<CR>" + treatName + "|" + imageType;
    qDebug() << shootRequestInfo;
    emit sendRequest(shootRequestInfo);
    //슛리퀘스트 네트워크매니저로 보내고 메인윈도우에서 연결해서 서버로 보내고 서버에서는 촬영요청메세지 촬영SW와 뷰어SW와 환자관리SW에 모두 보내주기

}

//void PatientStatusManager::SRQRequestSended(QString sendedRequestData)
//{
//    QString id = sendedRequestData.split("<CR>")[1];    //pid
//    QString data = sendedRequestData.split("<CR>")[2];  //name|imageType
//    QString name = data.split("|")[0];    //name

//    qDebug() << "sendedRequestData: " << sendedRequestData;

//    int i = 0;     //0번째 컬럼(id)
//    auto flag = Qt::MatchCaseSensitive;                    //i가 0이 아닌 값일 때는 입력한 값이 정확할 때만 검색이 되도록 만듦
//    auto items = ui->waitTreatmentTreeWidget->findItems(id, flag, i);    //flag와 i값에 해당하는 정보를 searchLineEdit에 입력한 텍스트를 찾고, items에 해당 값을 저장해준다
//    qDebug() << items;

//    foreach(auto i, items) {                                            //아이템들을 하나씩 꺼내옴
//        QTreeWidgetItem* item = static_cast<QTreeWidgetItem *>(i);                    //i의 자료형을 ClientItem이라는 형식으로 변환하고 고정
//        item->setText(2, "촬영중");
//    }
//}

void PatientStatusManager::statusRequestSended(QString sendedRequestData)
{
    QString event = sendedRequestData.split("<CR>")[0];
    QString id = sendedRequestData.split("<CR>")[1];    //pid
    QString data = sendedRequestData.split("<CR>")[2];  //name|imageType
    QString name = data.split("|")[0];    //name

    qDebug() << "sendedRequestData: " << sendedRequestData;

    int i = 0;     //0번째 컬럼(id)
    auto flag = Qt::MatchCaseSensitive;                    //i가 0이 아닌 값일 때는 입력한 값이 정확할 때만 검색이 되도록 만듦
    auto items = ui->waitTreatmentTreeWidget->findItems(id, flag, i);    //flag와 i값에 해당하는 정보를 searchLineEdit에 입력한 텍스트를 찾고, items에 해당 값을 저장해준다
    qDebug() << items;

    if(event == "SRQ"){
        foreach(auto i, items)
        {                                            //아이템들을 하나씩 꺼내옴
            QTreeWidgetItem* item = static_cast<QTreeWidgetItem *>(i);
            item->setText(2, "촬영중");
        }
    }
    else if(event == "VTS"){
        foreach(auto i, items)
        {                                            //아이템들을 하나씩 꺼내옴
            QTreeWidgetItem* item = static_cast<QTreeWidgetItem *>(i);
            item->setText(2, "진료중");
        }
    }
    else if(event == "ISV"){
        foreach(auto i, items)
        {                                            //아이템들을 하나씩 꺼내옴
            QTreeWidgetItem* item = static_cast<QTreeWidgetItem *>(i);
            item->setText(2, "대기중");
        }
    }
    else if(event == "VTF"){     //@@@@@@뷰어와 연동했을 때 확인 필요
        foreach(auto i, items)
        {
            //아이템들을 하나씩 꺼내옴
            QTreeWidgetItem* item = static_cast<QTreeWidgetItem *>(i);
            //ui->waitPaymentTreeWidget->addTopLevelItem(item);   //일단 수납대기 리스트에 올려줌
            QTreeWidgetItem* payList = new QTreeWidgetItem;
            ui->waitPaymentTreeWidget->addTopLevelItem(payList);
            payList->setText(0, item->text(0));
            payList->setText(1, item->text(1));

            ui->waitTreatmentTreeWidget->takeTopLevelItem(ui->waitTreatmentTreeWidget->indexOfTopLevelItem(item));  //진료 상태 리스트에서는 삭제해줌

        }
    }
}

