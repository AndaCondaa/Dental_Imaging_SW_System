#include "loginform.h"
#include "ui_loginform.h"

#include <QMessageBox>

LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::receiveLogInCheck(QString data)
{
    Check = data;

    //의사 ID, 의사 명으로 로그인 되었을 때
    if(Check == "True")
    {
        QString DoctorID = ui->DoctorID->text();
        QString DoctorName = ui->DoctorName->text();
        emit LogInSuccess(DoctorID, DoctorName);
    }

    //의사 ID, 의사 명으로 로그인 안되었을 때
    else if(Check == "False")
    {
        QMessageBox:: critical(this, "경고", "아이디/이름을 잘못 입력했습니다");
        ui->DoctorID->clear();
        ui->DoctorName->clear();
    }
}

void LoginForm::on_LogIn_clicked()
{
    QString DoctorID = ui->DoctorID->text();
    QString DoctorName = ui->DoctorName->text();

    if(DoctorID == "" || DoctorName == "")
    {
        QMessageBox:: critical(this, "경고", "아이디/이름을 입력하세요");
    }

    QString Data = "VLG<CR>" + DoctorID + "<CR>" + DoctorName;
    //로그인 페이지에서 입력한 의사 ID, 의사 명을 서버에서 체크하기 위해 데이터 전송
    emit SendDoctorCheck(Data);
}

