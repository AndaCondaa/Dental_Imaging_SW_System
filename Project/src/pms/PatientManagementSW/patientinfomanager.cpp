/*
 * 프로그램명 : PatientManagementSW
 * 파일명 : patientInfoManager.cpp
 * 설명 : 환자 정보를 검색하고 확인, 수정/삭제, 검색한 환자를 대기 목록에 추가
 * 작성자 : 김유선
 * 최종 수정일 : 2023.02.16
 */


#include "patientinfomanager.h"
#include "ui_patientinfomanager.h"

#include <QMessageBox>
#include <QGraphicsEffect>
#include <QFile>
#include <QFileDialog>

PatientInfoManager::PatientInfoManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientInfoManager)
{
    ui->setupUi(this);
    ui->clientInfoTableWidget->setColumnWidth(0,290);
    ui->searchPushButton->setIcon(QIcon(":/img/search.png"));

    connect(ui->searchLineEdit, SIGNAL(returnPressed()), this, SLOT(on_searchPushButton_clicked()));

    // 각 ui 요소들에 대한 styleSheet 저장
    QString pushButtonStyle1 = "QPushButton { "
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
                               "}";

    QString pushButtonStyle2 = "QPushButton { "
                               "border-style: outset;"
                               "border-width: 3px;"
                               "border-radius: 10px;"
                               "border-color: orange;"
                               "color:#ED8817;"
                               "outline: 0; "
                               "}"
                               "QPushButton:hover { "
                               "background-color: #FFCF77;"
                               "border-radius:10px;"
                               "color:#ffffff;"
                               "outline: 0; "
                               "}";

    QString comboBoxStyle = "QComboBox { "
                            "border-style: outset;"
                            "border-width: 3px;"
                            "border-radius: 10px;"
                            "border-color: orange;"
                            "color:#ED8817;"
                            "outline: 0; "
                            "}"
                            "QComboBox QAbstractItemView {"
                            "background-color: white;"
                            "selection-background-color: rgb(0, 0, 0);"
                            "border: 1px solid #FFCF77;"
                            "border-radius: 4px;"
                            "padding: 4px;"
                            "outline: 0;"
                            "color: orange;"
                            "}"
                            "QComboBox::drop-down {"
                            "background-color: orange;"
                            "subcontrol-origin: padding;"
                            "subcontrol-position: top right;"
                            "border-radius: 4px;"
                            "width: 16px;"
                            "}"
                            "QComboBox::down-arrow:on {"
                            "image: url(:/img/dropdown.png);"
                            "width: 16px;"
                            "height: 16px;"
                            "}"
                            "QComboBox::down-arrow {"
                            "image: url(:/img/dropdown.png);"
                            "width: 16px;"
                            "height: 16px;"
                            "}";

    // 라벨 스타일 설정
    QString labelStyle = "QLabel { "
                         "background-color: rgb(150, 150, 150);"
                         "border-radius:10px;"
                         "color:#ffffff;"
                         "outline: 0; "
                         "}";

    // 사진 변경 버튼 스타일 설정
    QString changePhotoPushButtonStyle = "QPushButton { "
                                         "background-color: rgb(170, 170, 170);"
                                         "border-radius:5px;"
                                         "color:#ffffff;"
                                         "outline: 0; "
                                         "}"
                                         "QPushButton:hover { "
                                         "background-color: rgb(200, 200, 200);"
                                         "border-radius:5px;"
                                         "color:#ffffff;"
                                         "outline: 0; "
                                         "}"
                                         "QPushButton:disabled { "
                                         "background-color: #ffffff;"
                                         "border-radius:5px;"
                                         "border:1px solid rgb(220, 220, 220);"
                                         "color:rgb(220, 220, 220);"
                                         "outline: 0; "
                                         "}";

    // 각 ui 요소들에 대한 styleSheet 적용
    ui->modifyPushButton->setStyleSheet(pushButtonStyle2);
    ui->deletePushButton->setStyleSheet(pushButtonStyle2);
    ui->WaitPushButton->setStyleSheet(pushButtonStyle1);
    ui->searchPushButton->setStyleSheet(pushButtonStyle1);
    ui->searchComboBox->setStyleSheet(comboBoxStyle);
    ui->label_2->setStyleSheet(labelStyle);
    ui->changePhotoPushButton->setStyleSheet(changePhotoPushButtonStyle);

    // 환자정보 label에 그림자 효과
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(5);
    effect->setXOffset(5);
    effect->setYOffset(5);
    effect->setColor(QColor(220,220,220));
    ui->label_2->setGraphicsEffect(effect);

    // 초기 화면에서는 사진 변경 기능은 비활성화 상태
    ui->changePhotoPushButton->setDisabled(true);

    // 기본 이미지 설정
    pixmap = new QPixmap();
    pixmap->load(":/img/man.png");
    pixmap->scaled(200,180,Qt::IgnoreAspectRatio);
    ui->patientFace->setPixmap(pixmap->scaled(ui->patientFace->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));



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


}

PatientInfoManager::~PatientInfoManager()
{
    delete ui;
}

// 검색 버튼 클릭 시
void PatientInfoManager::on_searchPushButton_clicked()
{
    int comboBoxIndex = ui->searchComboBox->currentIndex();       // i에 검색콤보박스의 현재인덱스 번호를 저장

    QString searchInfo = ui->searchLineEdit->text();
    QString searchData = "SEN^PSE<CR>" + QString::number(comboBoxIndex) + "<CR>" + searchInfo;

    ui->clientInfoTableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);     // 검색하면 테이블 정보들 더블클릭해서 수정가능하도록 만듦
    qDebug() <<"searchData: " << searchData;

    // 서버로 검색할 환자의 이름이나 patient id를 보냄
    emit sendSearchData(searchData);

    // 새로운 환자 이미지의 다운로드가 가능한 상태인지 확인을 위해 networkManager쪽으로 signal 보내기
    emit downloadOrNotSignal();

    // 검색 후에는 searchLineEdit clear
    ui->searchLineEdit->clear();
}

// 검색한 환자의 데이터를 서버로부터 전달받아 화면에 띄울지 말지를 결정하는 함수
void PatientInfoManager::searchDataSended(QString id, QString data)
{
    // 받은 데이터에 P가 들어가 있지 않고(pid가 오면 동명이인이 아닌 사람이 정상적으로 검색된 것임) id가 2이상일 경우(즉, 동명이인일 경우)
    if(id.toInt() > 1 && id.contains("P", Qt::CaseInsensitive) == false)
    {
        // 시그널로 정보를 보내줌. id는 검색된 환자수, data는 검색된 환자정보들의 모음
        emit sendTempData(id, data);
        return;
    }

    qDebug() << "searchButtonClicked "<<searchButtonClicked;
    if(searchButtonClicked != 0)      // 이전에 검색한 환자의 이미지 파일이 다 다운로드 된 상태가 아닐 때는 return
    {
        return;
    }

    // 없는 환자 검색했을 때
    if(data=="<NEL>" || id == "NULL")
    {
        patientInDB = 0;

        // 없는 환자 정보를 찾으면 환자 정보도 다 clear
        ui->searchLineEdit->clear();
        ui->patientFace->clear();

        // 테이블도 같이 clear
        for(int i = 0 ; i<7; i++)
        {
            ui->clientInfoTableWidget->item(i, 0)->setText("");
        }

        // 사진 변경 불가능하게 만들기
        ui->changePhotoPushButton->setDisabled(true);

        // 여기에 QMessageBox 띄우기
        QMessageBox::critical(qmsgBox, tr("경고"), tr("해당 환자의 정보를 찾을 수 없습니다.\n"
                                                 "검색할 내용을 다시 한번 확인해 주세요."));

        // search버튼 클릭되면 이전 환자의 이미지가 보이지 않도록 imageManager쪽으로 signal 보내줌
        emit cleanImageSignal();

        return;
    }

    // 있는 환자면 활성화
    ui->changePhotoPushButton->setDisabled(false);

    // 환자가 있을 때는 patientInDB를 1로 만듦
    patientInDB = 1;

    // 환자정보를 멤버변수에 저장
    pid = id;
    name = data.split("|")[0];
    sex = data.split("|")[1];
    birthdate = data.split("|")[2];
    tel = data.split("|")[3];
    address = data.split("|")[4];
    memo = data.split("|")[5];

    // 환자정보를 환자정보를 보여주는 부분에 띄워줌
    ui->clientInfoTableWidget->setItem(0, 0, new QTableWidgetItem(pid));
    ui->clientInfoTableWidget->setItem(1, 0, new QTableWidgetItem(name));
    ui->clientInfoTableWidget->setItem(2, 0, new QTableWidgetItem(sex));
    ui->clientInfoTableWidget->setItem(3, 0, new QTableWidgetItem(birthdate));
    ui->clientInfoTableWidget->setItem(4, 0, new QTableWidgetItem(tel));
    ui->clientInfoTableWidget->setItem(5, 0, new QTableWidgetItem(address));
    ui->clientInfoTableWidget->setItem(6, 0, new QTableWidgetItem(memo));

    // 환자 사진 띄우기
    pixmap = new QPixmap();
    pixmap->load(QString("./Face/%1.png").arg(pid));
    pixmap->scaled(200,180,Qt::IgnoreAspectRatio);

    ui->patientFace->setPixmap(pixmap->scaled(ui->patientFace->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // search버튼 클릭할 때마다 1씩 추가. 1 이상일 때는 새로운 이미지 불러올 수 없음
    searchButtonClicked+=1;


    // search버튼 클릭되면 이전 환자의 이미지가 보이지 않도록 imageManager쪽으로 signal 보내줌
    emit cleanImageSignal();

}

// 환자 삭제 버튼을 클릭했을 때 Yes를 누르면 삭제할 환자의 정보를 patientStatusManager쪽으로 보내줌(대기리스트에 있는지 없는지 확인하기 위함)
void PatientInfoManager::on_deletePushButton_clicked()
{
    int delButtonNum = QMessageBox::critical(this, tr("경고"),
                                             tr("해당 환자와 연관된 사진이 모두 삭제됩니다.\n"
                                                "계속하시겠습니까?"), QMessageBox::Yes | QMessageBox::No);

    switch (delButtonNum) {
    case QMessageBox::Yes:
    {
        // patientStatusManager쪽으로 pid를 보내줌. patientStatusManager에서는 이 pid를 검색해 대기/수납 명단에 없다면 0, 있다면 1을 반환해줄 것
        emit sendDelPID(pid);

        break;
    }
    case QMessageBox::No:
    {
        return;
        break;
    }
    }

}

// 환자 정보를 지울수 있는 상태면 0, 대기리스트에 올라가 있는 환자라서 지울 수 없으면 1
// =>int값을 반환받아 삭제할지 말지를 결정. 삭제할 것이라면 환자 얼굴 파일을 삭제하고, patientStatusManager쪽에서는 삭제할 환자 정보를 서버로 전송
void PatientInfoManager::delFlagSended(int delFlag)
{
    if(delFlag == 0)
    {
        QString delData = "SEN^PDE<CR>" + pid + "<CR> ";   // 지울 pid를 emit으로 네트워크 매니저로 보냄/네트워크 매니저는 서버로 삭제할 데이터 전달
        emit sendDelData(delData);

        ui->searchLineEdit->clear();
        ui->patientFace->clear();

        // 환자 정보 clear
        for(int i=0; i<7;i++)
        {
            ui->clientInfoTableWidget->item(i, 0)->setText("");
        }

        // 얼굴 사진도 삭제
        QFile::remove(QString("./Face/%1.png").arg(pid));


        QMessageBox::information(this, tr("정보"), tr("환자 정보가 삭제되었습니다."));


    }
    else if(delFlag == 1)
    {
        QMessageBox::warning(this, tr("경고"), \
                             tr("대기리스트에 추가되어 있는 환자의 정보는 삭제하실 수 없습니다.\n"));
    }
}


// 대기리스트에 추가 시도
void PatientInfoManager::on_WaitPushButton_clicked()
{
    // 이름과 pid는 바뀌지 않는 정보지만 나머지 정보는 검색 후에 수정했을 수도 있으니 현재 테이블에 저장되어있던 값을 가지고 와 저장해준후 서버로 전송
    pid = ui->clientInfoTableWidget->item(0, 0)->text();
    name = ui->clientInfoTableWidget->item(1, 0)->text();
    sex = ui->clientInfoTableWidget->item(2,0)->text();
    birthdate = ui->clientInfoTableWidget->item(3,0)->text();
    tel = ui->clientInfoTableWidget->item(4,0)->text();
    address = ui->clientInfoTableWidget->item(5,0)->text();
    memo = ui->clientInfoTableWidget->item(6,0)->text();

    emit sendPIDtoWaitList(pid);

}

// 대기명단에 있는지 patientStatusManager쪽에서 확인 후 1이면 이미 대기명단에 있는 환자이므로 return, 0이면 대기명단에 없는 환자이므로 서버로 해당 정보 보내줌(patientStatusManager쪽에서는 확인 후 대기명단에 정보를 추가했을 것)
void PatientInfoManager::inWaitListSlot(int inWaitListOrNot)
{
    if(inWaitListOrNot == 1)
    {
        QMessageBox::critical(this, tr("경고"), tr("이미 대기명단에 있는 환자입니다."));
        return;
    }
    if(inWaitListOrNot == 0)
    {
        QString sendData = "SEN^AWL<CR>" + pid + "<CR>" + name + "|" + sex + "|" + birthdate + "|" + tel + "|" + address + "|" + memo;
        emit sendWaitInfo(sendData);
    }
}

// 환자 수정 버튼을 클릭했을 때. 정보를 수정하고 DB쪽으로 수정된 정보를 보냄
void PatientInfoManager::on_modifyPushButton_clicked()
{
    // 네트워크쪽으로 정보 넘겨주도록 signal emit하고 mainwindow에서 연결하고 서버에 넘겨서 update문으로 db 테이블 수정
    qDebug()<< "modify Button clicked";
    pid = ui->clientInfoTableWidget->item(0, 0)->text();
    name = ui->clientInfoTableWidget->item(1, 0)->text();
    sex = ui->clientInfoTableWidget->item(2,0)->text();
    birthdate = ui->clientInfoTableWidget->item(3,0)->text();
    tel = ui->clientInfoTableWidget->item(4,0)->text();
    address = ui->clientInfoTableWidget->item(5,0)->text();
    memo = ui->clientInfoTableWidget->item(6,0)->text();

    QString modifyData = "SEN^PMO<CR>" + pid + "<CR>" + name + "|" + sex + "|" + birthdate + "|" + tel + "|" + address + "|" + memo;
    emit sendModifyData(modifyData);

    QMessageBox::information(this, tr("정보"), tr("정보가 수정되었습니다."));


}

// 이전에 검색한 환자의 이미지 파일이 완전히 다운로드 된 상태인지를 확인. 아직 다운로드되기 전이라면 새로운 환자 검색이 불가능하도록 함
void PatientInfoManager::fileSendedSlot(int fileSendedSignal)
{
    // 파일이 완전히 전송되었다면 Flag가 0에서 1로 바뀌었을 것

    // DB에서 검색한 환자를 찾을 수 없으면 파일 받지 않기
    if(patientInDB == 0)
        return;
    else{
        fileSendedFlag = fileSendedSignal;

        if(fileSendedFlag==0)
        {
            QMessageBox::critical(this, tr("경고"), tr("이전에 검색한 환자 이미지를 불러오는 중입니다.\n"
                                                     "다운로드가 완료되면 다시 검색해주세요."));
            return;
        }
        else if(fileSendedFlag ==1)
        {
            // 없는 환자면 pid가 NULL이니 QMessageBox를 띄우지 않고 return해줌
            if(pid=="NULL")
                return;

            QMessageBox::information(this, tr("정보"), tr("검색이 완료되었습니다."));

            // 환자정보 테이블에 띄울 수 있는 상태로 초기화
            searchButtonClicked=0;
        }

    }

}



// 환자 등록시 환자정보를 화면에 바로 띄워줌
void PatientInfoManager::sendedNewDataForShow(QString id, QString data)
{
    pid = id;
    name = data.split("|")[0];
    sex = data.split("|")[1];
    birthdate = data.split("|")[2];
    tel = data.split("|")[3];
    address = data.split("|")[4];
    memo = data.split("|")[5];
    fileName = data.split("|")[6];

    // 등록이 되었을 때부터 수정이 가능하도록 만드는 부분
    ui->clientInfoTableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->changePhotoPushButton->setDisabled(false);
    ui->clientInfoTableWidget->setItem(0, 0, new QTableWidgetItem(pid));
    ui->clientInfoTableWidget->setItem(1, 0, new QTableWidgetItem(name));
    ui->clientInfoTableWidget->setItem(2, 0, new QTableWidgetItem(sex));
    ui->clientInfoTableWidget->setItem(3, 0, new QTableWidgetItem(birthdate));
    ui->clientInfoTableWidget->setItem(4, 0, new QTableWidgetItem(tel));
    ui->clientInfoTableWidget->setItem(5, 0, new QTableWidgetItem(address));
    ui->clientInfoTableWidget->setItem(6, 0, new QTableWidgetItem(memo));

    pixmap = new QPixmap();
    pixmap->load(QString("./Face/%1.png").arg(pid));
    pixmap->scaled(200,180,Qt::IgnoreAspectRatio);

    ui->patientFace->setPixmap(pixmap->scaled(ui->patientFace->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // 환자가 등록되는 당시에는 환자 이미지가 없는 상태임. 따라서 searchButtonClicked를 0으로 만들어주어야 다음의 새로운 환자 검색이 가능함
    searchButtonClicked=0;

    // search버튼 클릭되면 이전 환자의 이미지가 보이지 않도록 imageManager쪽으로 signal 보내줌
    emit cleanImageSignal();
}

// 환자 사진 변경
void PatientInfoManager::on_changePhotoPushButton_clicked()
{
    QDir dir(QString("./Face"));
    if (!dir.exists())
        dir.mkpath(".");

    pidPhoto = ui->clientInfoTableWidget->item(0, 0)->text();
    currentFileName = QString("./Face/%1.png").arg(pidPhoto);
    changeFileName = QFileDialog::getOpenFileName(this, tr("Open Image"),
                                                  QString("./Face/%1").arg(pidPhoto), tr("Image Files (*.png *.jpg *.bmp)"));



    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg)"));

    QString saveFileName;

    // 선택했던 파일을 디버그 파일의 Face폴더에 저장
    if(changeFileName.length()>0 && currentFileName.right(15) != changeFileName.right(15))
    {
        QFile::remove(QString("%1").arg(currentFileName));
        saveFileName = QString("%1").arg(currentFileName);
        QFile::copy(changeFileName, currentFileName);

    }

    QPixmap pix(QString("%1").arg(currentFileName));

    pix.scaled(150,180,Qt::IgnoreAspectRatio);
    ui->patientFace->setPixmap(pix.scaled(ui->patientFace->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    if(changeFileName!="")
        QMessageBox::information(this, tr("정보"), tr("사진 변경이 완료되었습니다."));

}

// 대기명단 추가에 대해 서버로부터 응답받는 부분
void PatientInfoManager::sendedAWLRequest(QString saveData)
{
    QString tempData = saveData.split("^")[0];
    if(tempData=="ERR")
    {
        QMessageBox::critical(nullptr, tr("경고"), tr("뷰어가 켜져있지 않습니다.\n"
                                                    "대기명단에 환자를 추가하기 위해 뷰어를 켜 주세요."));
        waitError = 1;
    }
    else
    {
        waitError = 0;
        emit sendWaitInfoToWaitList(pid, name);
    }
}
