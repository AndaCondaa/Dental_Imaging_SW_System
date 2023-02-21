/*
 * 프로그램명 : PatientManagementSW
 * 파일명 : patientStatusManager.cpp
 * 설명 : 대기 환자 목록 확인, 촬영요청, 수납처리
 * 작성자 : 김유선
 * 최종 수정일 : 2023.02.21
 */



#include "patientstatusmanager.h"
#include "ui_patientstatusmanager.h"
#include <QTreeWidget>
#include <QMessageBox>
#include <QGraphicsEffect>
PatientStatusManager::PatientStatusManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientStatusManager)
{
    ui->setupUi(this);


    // 라벨 스타일 설정
    QString labelStyle = "QLabel { "
                         "background-color: rgb(150, 150, 150);"
                         "border-radius:10px;"
                         "color:#ffffff;"
                         "outline: 0; "
                         "}";

    // 버튼 스타일 설정
    QString pushButtonStyle = "QPushButton { "
                              "background-color: #ED8817;"
                              "border-radius:10px;"
                              "color:#ffffff;"
                              "outline: 0; "
                              "}"
                              "QPushButton:hover { "
                              "background-color: #F2A754;"
                              "border-radius:10px;"
                              "color:#ffffff;"
                              "outline: 0; "
                              "}"
                              "QPushButton:disabled { "
                              "background-color: rgb(220, 220, 220);"
                              "border-radius:10px;"
                              "border:1px solid rgb(220, 220, 220);"
                              "color:#ffffff;"
                              "outline: 0; "
                              "}";

    ui->label_5->setStyleSheet(labelStyle);
    ui->label_6->setStyleSheet(labelStyle);
    ui->paymentPushButton->setStyleSheet(pushButtonStyle);
    ui->shootRequestPushButton->setStyleSheet(pushButtonStyle);


    // 라벨 그림자 효과 설정 1
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(5);
    effect->setXOffset(5);
    effect->setYOffset(5);
    effect->setColor(QColor(220,220,220));
    ui->label_5->setGraphicsEffect(effect);

    // 라벨 그림자 효과 설정 2
    QGraphicsDropShadowEffect *effect2 = new QGraphicsDropShadowEffect;
    effect2->setBlurRadius(5);
    effect2->setXOffset(5);
    effect2->setYOffset(5);
    effect2->setColor(QColor(220,220,220));
    ui->label_6->setGraphicsEffect(effect2);


    QString msgBoxStyle = "QMessageBox { "
                          "background-color: rgb(255, 255, 255);"
                          "color:#000000;"
                          "}"
                          "QPushButton { "
                          "background-color: rgb(250, 250, 250);"
                          "border-radius:2px;"
                          "color:#ffffff;"
                          "border-color: blue"
                          "font-weight: bold;"
                          "padding-top: 3px;"
                          "padding-bottom: 3px;"
                          "outline: 0; "
                          "}";



    qmsgBox = new QMessageBox();
    qmsgBox->setStyleSheet(msgBoxStyle);


    ui->shootRequestPushButton->setDisabled(true);
    ui->paymentPushButton->setDisabled(true);

}

PatientStatusManager::~PatientStatusManager()
{
    delete ui;
}

// PatientInfoManager쪽에서 대기환자 추가 버튼을 클릭했을 때 추가할지 말지를 결정하고 inWaitListSignal을 보냄. 1이면 이미 대기명단에 있는 환자, 0이면 대기명단에 없는 환자
void PatientStatusManager::PIDsendedtoWaitList(QString sendedTempPID)
{

    int inTreatListOrNot = ui->waitTreatmentTreeWidget->findItems(sendedTempPID, Qt::MatchFlags(Qt::MatchCaseSensitive)).count();
    int inPayListOrNot = ui->waitPaymentTreeWidget->findItems(sendedTempPID, Qt::MatchFlags(Qt::MatchCaseSensitive)).count();


    qDebug() << "patientStatus inlists: " << inTreatListOrNot + inPayListOrNot;
    emit inWaitListSignal(inTreatListOrNot + inPayListOrNot);
}

// 대기환자에 추가해야 하는 환자라면 waitTreatmentTreeWidget에 추가해줌
void PatientStatusManager::waitInfoSended(QString pid, QString name){

    QTreeWidgetItem* row = new QTreeWidgetItem;
    ui->waitTreatmentTreeWidget->addTopLevelItem(row);
    row->setText(0, pid);
    row->setText(1, name);
    row->setText(2, "진료대기");
}

// waitPaymentPID, waitPaymentName 전역변수로 만들어서 저장
void PatientStatusManager::on_waitPaymentTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    payPID = item->text(0);
    payName = item->text(1);

    selectedPayRow = item;

    ui->paymentPushButton->setDisabled(false);
}

// treatPID, treatName 전역변수로 만들어서 저장하고,
void PatientStatusManager::on_waitTreatmentTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    treatPID = item->text(0);
    treatName = item->text(1);

    selectedTreatRow = item;

    ui->shootRequestPushButton->setDisabled(false);
}

// 수납 버튼이 클릭되었을 때
void PatientStatusManager::on_paymentPushButton_clicked()
{
    int currentRow = ui->waitPaymentTreeWidget->currentIndex().row();


    if(currentRow == -1)    // 환자 선택이 안되어있을 때
    {
        QMessageBox::critical(qmsgBox, tr("경고"), \
                              tr("수납대기 리스트에서 수납처리 할 환자를 선택해주세요."));
        return;
    }

    ui->waitPaymentTreeWidget->takeTopLevelItem(ui->waitPaymentTreeWidget->indexOfTopLevelItem(selectedPayRow));

    ui->waitPaymentTreeWidget->update();                               // treeWidget 업데이트
    QMessageBox::information(qmsgBox, tr("정보"), tr("수납 처리가 완료되었습니다."));

    // 메인서버로 수납완료된 환자 정보를 보내고, 메인서버에서는 대기정보 기록을 위해 만들어진 텍스트 파일에서 해당 환자의 정보를 삭제
    QString paymentFinInfo = "SEN^WPY<CR>" + payPID + "<CR>" + payName;
    emit sendPayInfo(paymentFinInfo);


}

// 촬영 요청 버튼이 클릭되었을 때
void PatientStatusManager::on_shootRequestPushButton_clicked()
{
    QString imageType;
    int currentRow = ui->waitTreatmentTreeWidget->currentIndex().row();

    if(currentRow == -1)    // 환자 선택이 안되어있을 때
    {
        QMessageBox::critical(qmsgBox, tr("경고"), \
                              tr("진료대기 리스트에서 촬영할 환자를 선택해주세요."));
        return;
    }

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
    {
        QMessageBox::critical(qmsgBox, tr("경고"), \
                              tr("CEPH/PANO 중 하나를 이상을 선택하세요."));
        return;
    }


    int i = 0;     // 0번째 컬럼(id)
    auto flag = Qt::MatchCaseSensitive;                    // i가 0이 아닌 값일 때는 입력한 값이 정확할 때만 검색이 되도록 만듦
    auto items = ui->waitTreatmentTreeWidget->findItems(treatPID, flag, i);    // flag와 i값에 해당하는 정보를 searchLineEdit에 입력한 텍스트를 찾고, items에 해당 값을 저장해준다

    foreach(auto i, items)
    {
        QTreeWidgetItem* item = static_cast<QTreeWidgetItem *>(i);

        if(item->text(2)=="진료중")
        {
            QMessageBox::critical(qmsgBox, tr("경고"), \
                                  tr("진료중인 환자에 대해서는 촬영요청이 불가능합니다."));
            return;
        }
    }



    if(selectedTreatRow->text(2)=="촬영중")
    {
        QMessageBox::critical(qmsgBox, tr("경고"), \
                              tr("이미 촬영 중인 환자입니다."));
        return;
    }

    // 촬영SW와 확인 필요!
//     if(header != "ERR")
//         selectedTreatRow->setText(2,"촬영중");

    QString shootRequestInfo = "SEN^SRQ<CR>" + treatPID + "<CR>" + treatName + "|" + imageType;
    qDebug() << shootRequestInfo;
    emit sendRequest(shootRequestInfo);

}

// 촬영SW가 정상적으로 작동하고 있는지를 확인하고, 작동중이라면 촬영요청했던 환자의 상태를 촬영중으로 변경해줌
void PatientStatusManager::statusRequestSended(QString sendedRequestData)
{
    QString event = sendedRequestData.split("<CR>")[0];
    header = event.split("^")[0];

    if(header == "ERR")
    {
        QMessageBox::critical(qmsgBox, tr("경고"), tr("촬영SW가 정상작동하지 않습니다.\n"));
        return;
    }

    event = event.split("^")[1];
    QString id = sendedRequestData.split("<CR>")[1];    // pid

    qDebug() << "sendedRequestData: " << sendedRequestData;

    int i = 0;     // 0번째 컬럼(id)
    auto flag = Qt::MatchCaseSensitive;                    // i가 0이 아닌 값일 때는 입력한 값이 정확할 때만 검색이 되도록 만듦
    auto items = ui->waitTreatmentTreeWidget->findItems(id, flag, i);    // flag와 i값에 해당하는 정보를 searchLineEdit에 입력한 텍스트를 찾고, items에 해당 값을 저장해준다
    qDebug() << items;

    if(event == "SRQ"){
        foreach(auto i, items)
        {                                            // 아이템들을 하나씩 꺼내옴
            QTreeWidgetItem* item = static_cast<QTreeWidgetItem *>(i);
            item->setText(2, "촬영중");
        }
    }
    else if(event == "VTS"){
        foreach(auto i, items)
        {                                            // 아이템들을 하나씩 꺼내옴
            QTreeWidgetItem* item = static_cast<QTreeWidgetItem *>(i);
            item->setText(2, "진료중");
        }
    }
    else if(event == "ISV"){
        foreach(auto i, items)
        {                                            // 아이템들을 하나씩 꺼내옴
            QTreeWidgetItem* item = static_cast<QTreeWidgetItem *>(i);
            item->setText(2, "진료대기");
        }
    }
    else if(event == "VTF"){
        foreach(auto i, items)
        {
            // 아이템들을 하나씩 꺼내옴
            QTreeWidgetItem* item = static_cast<QTreeWidgetItem *>(i);
            // ui->waitPaymentTreeWidget->addTopLevelItem(item);   // 일단 수납대기 리스트에 올려줌
            QTreeWidgetItem* payList = new QTreeWidgetItem;
            ui->waitPaymentTreeWidget->addTopLevelItem(payList);
            payList->setText(0, item->text(0));
            payList->setText(1, item->text(1));

            ui->waitTreatmentTreeWidget->takeTopLevelItem(ui->waitTreatmentTreeWidget->indexOfTopLevelItem(item));  // 진료 상태 리스트에서는 삭제해줌

        }
    }
    else if(event == "MWL"){    // 진료중이었던 환자 상태를 진료대기로 변경
        foreach(auto i, items)  // 아이템들을 하나씩 꺼내옴
        {
            QTreeWidgetItem* item = static_cast<QTreeWidgetItem *>(i);
            item->setText(2, "진료대기");
        }
    }
}

// 삭제요청했던 pid가 진료대기/진료중/촬영중/수납대기 4가지 상태 중 하나면 삭제할 수 없음을 알리는 flag값을 patientInfoManager쪽으로 반환
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


// 오류로 인해 꺼졌을 때 다시 프로그램 실행시키면 이전의 대기 리스트가 보여지도록 만드는 부분
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

        if(tempStatus == "WT")
            tempStatus = "진료대기";
        else if(tempStatus == "TM")
            tempStatus = "진료중";
        else if(tempStatus == "CE" || tempStatus == "PA" || tempStatus =="BO")
            tempStatus = "촬영중";
        else if(tempStatus == "WP")
            tempStatus = "수납대기";


        // 수납대기 상태는 수납대기 리스트에 들어가야 함
        if(tempStatus == "수납대기")
        {
            QTreeWidgetItem* row = new QTreeWidgetItem;
            ui->waitPaymentTreeWidget->addTopLevelItem(row);
            row->setText(0, tempPID);
            row->setText(1, tempName);
        }
        // 나머지 상태는 진료대기 리스트에 들어가야 함
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


// 재접속했을 때 waitlist 초기화
void PatientStatusManager::waitListClearSlot()
{
    ui->waitPaymentTreeWidget->clear();
    ui->waitTreatmentTreeWidget->clear();
}
