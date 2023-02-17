/*프로그램명 : ImageViewerSW
파일명 : loginform.cpp
설명 : 뷰어 SW 프로그램 시작 시 의사ID와 의사명으로 로그인 후 다음창으로 넘어갈 수 있도록 구성된 클래스
작성자 : 이정연
최종 수정 날짜 : 2023.02.11*/


#include "loginform.h"
#include "ui_loginform.h"

#include <QMessageBox>
#include <QGraphicsDropShadowEffect>

LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);

    // 로그인 창에서 아이콘 이미지 설정
    QPixmap pixmap(":/Icon/Icon/login.png");
    ui->Icon->setPixmap(pixmap.scaled(200, 200));

    ui->DoctorID->setPlaceholderText(" 아이디를 입력해주세요. . .");
    ui->DoctorName->setPlaceholderText(" 이름을 입력해주세요. . .");

    messagebox = new QMessageBox();
    messagebox->setStyleSheet("QMessageBox { "
                              "background-color: rgb(255, 255, 255);"
                              "color:#000000;"
                              "}"
                              "QPushButton { "
                              "background-color: rgb(250, 250, 250);"
                              "border-radius:2px;"
                              "font: bold;"
                              "color:rgb(0, 0, 0);"
                              "border-color: blue"
                              "outline: 0;"
                              "}");

    // 로그인 창에 그림자 효과 생성
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(5);
    effect->setXOffset(7);
    effect->setYOffset(7);
    effect->setColor(QColor(190, 190, 190));
    ui->widget->setGraphicsEffect(effect);

    ui->connectToServer->setStyleSheet("QPushButton { "
                                       "background-color: #ED8817;"
                                       "border-color: #ED8817;"
                                       "border-radius:20px;"
                                       "color:#ffffff;"
                                       "outline: 0; "
                                       "font-family:Malgun Gothic;"
                                       "font-size:30px;"
                                       "font-weight:bold;"
                                       "}"
                                       "QPushButton:hover { "
                                       "background-color: #F2A754;"
                                       "border-color: #F2A754;"
                                       "border-radius:20px;"
                                       "color:#ffffff;"
                                       "outline: 0; "
                                       "font-family:Malgun Gothic;"
                                       "font-size:30px;"
                                       "font-weight:bold;"
                                       "}"
                                       "QPushButton:disabled { "
                                       "background-color: rgb(132,132,132); "      // 회색
                                       "border-radius:20px;"
                                       "border:1px solid rgb(132,132,132);"
                                       "color:#ffffff;"
                                       "font-family:Malgun Gothic;"
                                       "font-size:30px;"
                                       "font-weight:bold;"
                                       "}");

    ui->LogIn->setStyleSheet("QPushButton { "
                             "background-color: #ED8817;"
                             "border-color: #ED8817;"
                             "border-radius:20px;"
                             "color:#ffffff;"
                             "outline: 0; "
                             "font-family:Malgun Gothic;"
                             "font-size:30px;"
                             "font-weight:bold;"
                             "}"
                             "QPushButton:hover { "
                             "background-color: #F2A754;"
                             "border-color: #F2A754;"
                             "border-radius:20px;"
                             "color:#ffffff;"
                             "outline: 0; "
                             "font-family:Malgun Gothic;"
                             "font-size:30px;"
                             "font-weight:bold;"
                             "}"
                             "QPushButton:disabled { "
                             "background-color: rgb(132,132,132); "      // 회색
                             "border-radius:20px;"
                             "border:1px solid rgb(132,132,132);"
                             "color:#ffffff;"
                             "font-family:Malgun Gothic;"
                             "font-size:30px;"
                             "font-weight:bold;"
                             "}");


    // 접속 버튼 클릭 시 서버로 접속 시도 및 소켓 생성
    connect(ui->connectToServer, &QPushButton::clicked, this, [=](){
        emit reconnect(ui->IPNumber->text());
    });
}

LoginForm::~LoginForm()
{
    delete ui;
}

// 로그인 창에서 입력한 의사 ID와 의사명이 DB 내에 존재하는 정보인지 확인
void LoginForm::receiveLogInCheck(QString data)
{
    Check = data;

    // 의사 ID, 의사 명으로 로그인 되었을 때
    if(Check == "True")
    {
        QString DoctorID = ui->DoctorID->text();
        QString DoctorName = ui->DoctorName->text();
        // 로그인 성공 시 ImageAlbum 클래스에 담당의사 정보 전송
        emit LogInSuccess(DoctorID, DoctorName);
    }

    // 의사 ID, 의사 명으로 로그인 안되었을 때
    else if(Check == "False")
    {
        QMessageBox:: critical(messagebox, "경고", "아이디/이름을 잘못 입력했습니다");
        ui->DoctorID->clear();
        ui->DoctorName->clear();
    }
}

//로그인 버튼 클릭 시 서버로 해당 의사 정보가 일치하는지 확인하기 위해 시그널 전송
void LoginForm::on_LogIn_clicked()
{
    QString DoctorID = ui->DoctorID->text();
    QString DoctorName = ui->DoctorName->text();

    // 아이디와 이름 중 하나라도 입력하지 않고 로그인 버튼 클릭 시 경고문 출력
    if(DoctorID == "" || DoctorName == "")
    {
        QMessageBox::critical(messagebox, "경고", "아이디/이름을 입력하세요");
        return;
    }

    QString Data = "SEN^VLG<CR>" + DoctorID + "<CR>" + DoctorName;
    // 로그인 페이지에서 입력한 의사 ID, 의사 명을 서버에서 체크하기 위해 데이터 전송
    emit SendDoctorCheck(Data);
}

// 서버에 접속 여부에 따라 버튼의 활성화 설정
void LoginForm::serverConnected(int flags)
{
    // 서버 접속 실패
    if (flags == 0) {
        ui->IPNumber->setEnabled(true);
        ui->connectToServer->setEnabled(true);
        ui->LogIn->setEnabled(false);
    }

    // 서버 접속 성공
    else if (flags == 1) {
        ui->IPNumber->setEnabled(false);
        ui->connectToServer->setEnabled(false);
        ui->LogIn->setEnabled(true);
    }

}

