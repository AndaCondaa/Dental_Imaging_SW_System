/*프로그램명 : ImageViewerSW
파일명 : loginform.h
설명 : 뷰어 SW 프로그램 시작 시 의사ID와 의사명으로 로그인 후 다음창으로 넘어갈 수 있도록 구성된 클래스
작성자 : 이정연
최종 수정 날짜 : 2023.02.11*/

#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>
#include <QMessageBox>

namespace Ui {
class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = nullptr);
    ~LoginForm();

private slots:
    void on_LogIn_clicked();
    void receiveLogInCheck(QString);
    void serverConnected(int);

signals:
    void SendDoctorCheck(QString);
    void LogInSuccess(QString, QString);
    void LoginToServer();
    void reconnect(QString);

private:
    Ui::LoginForm *ui;
    QString Check;
    QMessageBox* messagebox;
};

#endif // LOGINFORM_H
