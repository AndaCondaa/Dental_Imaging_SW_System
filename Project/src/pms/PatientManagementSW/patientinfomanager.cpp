#include "patientinfomanager.h"
#include "ui_patientinfomanager.h"
#include <QMessageBox>

PatientInfoManager::PatientInfoManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientInfoManager)
{
    ui->setupUi(this);
    ui->clientInfoTableWidget->setColumnWidth(0,290);

    pixmap = new QPixmap();
    pixmap->load("./Face/default.png");
    pixmap->scaled(200,180,Qt::IgnoreAspectRatio);

    ui->patientFace->setPixmap(pixmap->scaled(ui->patientFace->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    //ui->patientFace->setAlignment(Qt::AlignBottom | Qt::AlignCenter);

}

PatientInfoManager::~PatientInfoManager()
{
    delete ui;
}



void PatientInfoManager::on_searchPushButton_clicked()
{
    int comboBoxIndex = ui->searchComboBox->currentIndex();     //i에 검색콤보박스의 현재인덱스 번호를 저장
    //qDebug()<< comboBoxIndex;
    QString searchInfo = ui->searchLineEdit->text();
    qDebug()<< searchInfo;
    QString searchData = "PSE<CR>" + QString::number(comboBoxIndex) + "<CR>" + searchInfo;

    ui->clientInfoTableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);   //검색하면 테이블 정보들 더블클릭해서 수정가능하도록 만듦
    qDebug() <<"searchData: " << searchData;





    emit sendSearchData(searchData);



    emit downloadOrNotSignal(); //새로운 환자 이미지 다운로드 해도 되는지 확인을 위해 signal 보내기
}

void PatientInfoManager::searchDataSended(QString id, QString data)
{

    qDebug() << "searchButtonClicked"<<searchButtonClicked;
    if(searchButtonClicked != 0)    //이전에 검색한 환자의 이미지 파일이 다 다운로드 된 상태가 아닐 때는 return
        return;

//    if(fileSendedFlag==0)   // 파일이 다 다운로드 된 상태가 아닐 때는 재검색한 환자의 정보를 띄우면 안됨
//        return;

    //없는 환자 검색했을 때
    if(data=="<NEL>")
    {
        patientInDB = 0;
        //여기에 QMessageBox 띄우기
        QMessageBox::critical(this, tr("경고"), tr("해당 환자의 정보를 찾을 수 없습니다.\n"
                                                 "검색할 내용을 다시 한번 확인해 주세요."));
        ui->searchLineEdit->clear();
        return;
    }
//    else

    patientInDB = 1;

    pid = id;
    name = data.split("|")[0];
    //qDebug() << "name in searchDataSended: " << name;
    sex = data.split("|")[1];
    birthdate = data.split("|")[2];
    tel = data.split("|")[3];
    address = data.split("|")[4];
    memo = data.split("|")[5];

    qDebug("%d", __LINE__);

    //    QTableWidgetItem *item = new QTableWidgetItem;
    //    item->setText(pid);
    ui->clientInfoTableWidget->setItem(0, 0, new QTableWidgetItem(pid)/*item*/);
    ui->clientInfoTableWidget->setItem(1, 0, new QTableWidgetItem(name));
    //    qDebug() << "name in searchDataSended(in table):" << ui->clientInfoTableWidget->itemAt(1)->currentItem()->row()->text();
    //qDebug() << "name in searchDataSended(in table):"<< ui->clientInfoTableWidget->item(1, 0)->text();
    ui->clientInfoTableWidget->setItem(2, 0, new QTableWidgetItem(sex));
    ui->clientInfoTableWidget->setItem(3, 0, new QTableWidgetItem(birthdate));
    ui->clientInfoTableWidget->setItem(4, 0, new QTableWidgetItem(tel));
    ui->clientInfoTableWidget->setItem(5, 0, new QTableWidgetItem(address));
    ui->clientInfoTableWidget->setItem(6, 0, new QTableWidgetItem(memo));

    qDebug("%d", __LINE__);
    pixmap = new QPixmap();
    pixmap->load(QString("./Face/%1.png").arg(pid));
    pixmap->scaled(200,180,Qt::IgnoreAspectRatio);

    qDebug("%d", __LINE__);
    ui->patientFace->setPixmap(pixmap->scaled(ui->patientFace->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));


    qDebug("%d", __LINE__);

    searchButtonClicked+=1;


    //search버튼 클릭되면 이전 환자의 이미지가 보이지 않도록 imageManager쪽으로 signal 보내줌
    emit cleanImageSignal();


}


void PatientInfoManager::on_deletePushButton_clicked()
{
    int delButtonNum = QMessageBox::critical(this, tr("경고"),
                                   tr("해당 환자와 연관된 사진이 모두 삭제됩니다.\n"
                                      "계속하시겠습니까?"), QMessageBox::Yes | QMessageBox::No);


    switch (delButtonNum) {
    case QMessageBox::Yes:
    {
        //patientStatusManager쪽으로 pid를 보내줌. patientStatusManager에서는 이 pid를 검색해 대기/수납 명단에 없다면 0, 있다면 1을 반환해줄 것
        emit sendDelPID(pid);


        /*이 부분 슬롯에 넣기
        QString delData = "PDE<CR>" + pid + "<CR> "; //지울 pid를 emit으로 네트워크 매니저로 보냄/네트워크 매니저는 서버로 삭제할 데이터 전달
        emit sendDelData(delData);

        ui->searchLineEdit->clear();
        ui->patientFace->clear();

        //환자 정보 clear
        for(int i=0; i<7;i++)
        {
            ui->clientInfoTableWidget->item(i, 0)->setText("");
        }
        */

        break;
    }
    case QMessageBox::No:
    {
        return;
        break;
    }
    }



    //    QMessageBox::critical(this, tr("경고"), \
    //                          tr("해당 환자와 연관된 사진이 모두 삭제됩니다. 계속하시겠습니까?"));



}

void PatientInfoManager::delFlagSended(int delFlag)
{
    if(delFlag == 0)
    {
        QString delData = "PDE<CR>" + pid + "<CR> "; //지울 pid를 emit으로 네트워크 매니저로 보냄/네트워크 매니저는 서버로 삭제할 데이터 전달
        emit sendDelData(delData);

        ui->searchLineEdit->clear();
        ui->patientFace->clear();

        //환자 정보 clear
        for(int i=0; i<7;i++)
        {
            ui->clientInfoTableWidget->item(i, 0)->setText("");
        }

        //얼굴 사진도 삭제
        QFile::remove(QString("./Face/%1.png").arg(pid));

    }
    else if(delFlag == 1)
    {
        QMessageBox::warning(this, tr("경고"), \
                                      tr("대기리스트에 추가되어 있는 환자의 정보는 삭제하실 수 없습니다.\n"));
    }
}



void PatientInfoManager::on_WaitPushButton_clicked()
{
    //이름과 pid는 바뀌지 않는 정보지만 나머지 정보는 검색 후에 수정했을 수도 있으니 현재 테이블에 저장되어있던 값을 가지고 와 저장해준후 서버로 보내기
    //qDebug() << "대기명단에 추가한 pid: " << ui->clientInfoTableWidget->itemAt(0,0)->text();
    pid = ui->clientInfoTableWidget->item(0, 0)->text();
    name = ui->clientInfoTableWidget->item(1, 0)->text();
    //qDebug() << "name: " << ui->clientInfoTableWidget->itemAt(5,0)->text();

    sex = ui->clientInfoTableWidget->item(2,0)->text();
    birthdate = ui->clientInfoTableWidget->item(3,0)->text();
    tel = ui->clientInfoTableWidget->item(4,0)->text();
    address = ui->clientInfoTableWidget->item(5,0)->text();
    memo = ui->clientInfoTableWidget->item(6,0)->text();

    emit sendPIDtoWaitList(pid);

    //qDebug() << "waitSignal: " << waitSignal;


}

void PatientInfoManager::inWaitListSlot(int inWaitListOrNot)
{
    //qDebug()<<"inWaitLsit: " << inWaitListOrNot;

    if(inWaitListOrNot == 1)
    {
        QMessageBox::critical(this, tr("경고"), \
                              tr("이미 대기명단에 있는 환자입니다."));
        return;
    }


    if(inWaitListOrNot == 0)
    {
        QString sendData = "AWL<CR>" + pid + "<CR>" + name + "|" + sex + "|" + birthdate + "|" + tel + "|" + address + "|" + memo;
        //qDebug() << "서버로 보낼 대기환자 데이터: " <<sendData;

        emit sendWaitInfo(sendData);
    }
    //qDebug()<<" ";

}

void PatientInfoManager::on_modifyPushButton_clicked()
{
    //네트워크쪽으로 정보 넘겨주도록 signal emit하고 mainwindow에서 연결하고 서버에 넘겨서 update문으로 db 테이블 수정
    qDebug()<< "modify Button clicked";
    pid = ui->clientInfoTableWidget->item(0, 0)->text();
    name = ui->clientInfoTableWidget->item(1, 0)->text();
    sex = ui->clientInfoTableWidget->item(2,0)->text();
    birthdate = ui->clientInfoTableWidget->item(3,0)->text();
    tel = ui->clientInfoTableWidget->item(4,0)->text();
    address = ui->clientInfoTableWidget->item(5,0)->text();
    memo = ui->clientInfoTableWidget->item(6,0)->text();

    QString modifyData = "PMO<CR>" + pid + "<CR>" + name + "|" + sex + "|" + birthdate + "|" + tel + "|" + address + "|" + memo;
    //qDebug()<< "modifyData: " << modifyData;
    emit sendModifyData(modifyData);

}

void PatientInfoManager::fileSendedSlot(int fileSendedSignal)
{
    //파일이 완전히 전송되었다면 Flag가 0에서 1로 바뀌었을 것.

    fileSendedFlag = fileSendedSignal;

    if(fileSendedFlag==0)
    {
        QMessageBox::critical(this, tr("경고"), tr("이전에 검색한 환자 이미지를 불러오는 중입니다.\n"
                                                 "다운로드가 완료되면 다시 검색해주세요."));
        return;
    }
    else if(fileSendedFlag ==1)
    {
        //없는 환자 정보
        if(patientInDB ==0)
            return;

        QMessageBox::information(this, tr("정보"), tr("검색한 환자 이미지가 전부 로드되었습니다."));

        //환자정보 테이블에 띄울 수 있는 상태로 초기화
        searchButtonClicked=0;
    }



}

