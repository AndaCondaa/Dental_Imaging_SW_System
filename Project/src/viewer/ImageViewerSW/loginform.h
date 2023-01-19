#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>

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

signals:
    void SendDoctorCheck(QString);
    void LogInSuccess(QString, QString);

private:
    Ui::LoginForm *ui;
    QString Check;
};

#endif // LOGINFORM_H
