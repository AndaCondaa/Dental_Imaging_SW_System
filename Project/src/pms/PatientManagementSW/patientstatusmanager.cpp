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

void PatientStatusManager::PIDsendedtoWaitList(QString sendedTempPID)
{

    int inWaitListOrNot = ui->waitTreatmentTreeWidget->findItems(sendedTempPID, Qt::MatchFlags(Qt::MatchCaseSensitive)).count();
    qDebug() << "patientStatus inwaitlist: " << inWaitListOrNot;
emit inWaitListSignal(inWaitListOrNot);
}

void PatientStatusManager::waitInfoSended(QString waitInfoSended){
    //qDebug()<<"대기리스트에 올릴 환자 정보: " << waitInfoSended;


    treatPID = waitInfoSended.split("<CR>")[1];
    QString data = waitInfoSended.split("<CR>")[2];
    treatName = data.split("|")[0];

    qDebug() << "pid, name: " << treatPID << ", " << treatName;
    //qDebug("%d", __LINE__);

    qDebug() << "정보 이미 있는지 확인: " << ui->waitTreatmentTreeWidget->findItems(tempTreatPID, Qt::MatchFlags(Qt::MatchCaseSensitive)).count();
//    int inWaitListOrNot = ui->waitTreatmentTreeWidget->findItems(tempTreatPID, Qt::MatchFlags(Qt::MatchCaseSensitive)).count();
//    qDebug() << "patientStatus inwaitlist: " << inWaitListOrNot;
//emit inWaitListSignal(inWaitListOrNot);

//    if(inWaitListOrNot != 0)
//    {
//        QMessageBox::critical(this, tr("경고"), \
//                              tr("이미 대기명단에 있는 환자입니다."));

//        return;
//    }
    QTreeWidgetItem* row = new QTreeWidgetItem;
    ui->waitTreatmentTreeWidget->addTopLevelItem(row);
    row->setText(0, treatPID);
    row->setText(1, treatName);
    row->setText(2, "진료대기");

    //ui->waitTreatmentTreeWidget->findItems()
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
    int currentRow = ui->waitPaymentTreeWidget->currentIndex().row();


    if(currentRow == -1)    //환자 선택이 안되어있을 때
    {
        QMessageBox::critical(this, tr("경고"), \
                              tr("수납대기 리스트에서 수납처리 할 환자를 선택해주세요."));
        return;
    }


    //if(selectedPayRow != nullptr)
    //{                                       //아이템이 있을 때
        //int i = ui->waitPaymentTreeWidget->currentIndex().row();           //i는 현재 인덱스 줄 번호
        ui->waitPaymentTreeWidget->takeTopLevelItem(ui->waitPaymentTreeWidget->indexOfTopLevelItem(selectedPayRow));

        ui->waitPaymentTreeWidget->update();                               //treeWidget 업데이트
        QMessageBox::information(this, tr("정보"), tr("수납 처리가 완료되었습니다."));
    //}




        //메인서버로 수납완료된 환자 정보를 보내고, 메인서버에서는 대기정보 기록을 위해 만들어진 텍스트 파일에서 해당 환자의 정보를 삭제
        QString paymentFinInfo = "WPY<CR>" + payPID + "<CR>" + payName;
        emit sendPayInfo(paymentFinInfo);


}

void PatientStatusManager::on_shootRequestPushButton_clicked()
{
    qDebug("%d",__LINE__);
    QString imageType;
    int currentRow = ui->waitTreatmentTreeWidget->currentIndex().row();


    if(currentRow == -1)    //환자 선택이 안되어있을 때
    {
        QMessageBox::critical(this, tr("경고"), \
                              tr("진료대기 리스트에서 촬영할 환자를 선택해주세요."));
        return;
    }



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


    qDebug() << "currentRow: " << currentRow;


    qDebug() << selectedTreatRow->text(2);
    if(selectedTreatRow->text(2)=="촬영중")
    {
        QMessageBox::critical(this, tr("경고"), \
                              tr("이미 촬영 중인 환자입니다."));
        return;
    }


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
            item->setText(2, "진료대기");
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

void PatientStatusManager::delPIDSended(QString pid)
{
    int delFlag;
    int inTreatListOrNot = ui->waitTreatmentTreeWidget->findItems(pid, Qt::MatchFlags(Qt::MatchCaseSensitive)).count();
    int inPaymentListOrNot = ui->waitPaymentTreeWidget->findItems(pid, Qt::MatchFlags(Qt::MatchCaseSensitive)).count();

    if(inTreatListOrNot+inPaymentListOrNot==0)
        delFlag = 0;
    else
        delFlag = 1;

    emit sendDelFlag(delFlag);

}


//오류로 인해 꺼졌을 때 다시 프로그램 실행시키면 이전의 대기 리스트가 보여지도록 만드는 부분
void PatientStatusManager::oldListSended(QString sendedData)
{
    qDebug() << sendedData.split("<CR>")[2];
    QString data = sendedData.split("<CR>")[2];
    qDebug() << sendedData.count("<r>")+1;
    int tempCount = sendedData.split("<CR>")[1].toInt();

    for(int i=0; i<tempCount; i++)
    {
        oldList.insert(i, data.split("<r>")[i]);
    }


    for(int i=0; i<tempCount; i++)
    {
        QString tempLine = oldList.value(i);
        QString tempPID = tempLine.split("|")[0];
        QString tempName = tempLine.split("|")[1];
        QString tempStatus = tempLine.split("|")[2];

        //수납대기 상태는 수납대기 리스트에 들어가야 함
        if(tempStatus == "수납대기")
        {
            QTreeWidgetItem* row = new QTreeWidgetItem;
            ui->waitPaymentTreeWidget->addTopLevelItem(row);
            row->setText(0, tempPID);
            row->setText(1, tempName);
        }
        //나머지 상태는 진료대기 리스트에 들어가야 함
        else
        {
            QTreeWidgetItem* row = new QTreeWidgetItem;
            ui->waitTreatmentTreeWidget->addTopLevelItem(row);
            row->setText(0, tempPID);
            row->setText(1, tempName);
            row->setText(2, tempStatus);
        }

    }

}
