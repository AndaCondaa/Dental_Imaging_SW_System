#ifndef PATIENTINFOMANAGER_H
#define PATIENTINFOMANAGER_H

#include <QWidget>
#include <QtCore>
#include <QtNetwork>

namespace Ui {
class PatientInfoManager;
}

class PatientInfoManager : public QWidget
{
    Q_OBJECT

public:
    explicit PatientInfoManager(QWidget *parent = nullptr);
    ~PatientInfoManager();

private:
    Ui::PatientInfoManager *ui;
//    bool fd_flag = false;
//    bool send_flag = false;
//    int pid = 1; //임시. pid는 최종적으로 P00001과 같은 형태가 되어야 함

private slots:
    void on_modifyPatientInfo_pushButton_clicked();
    void on_pushButton_clicked();

signals:
    void sendSearchData(QString);
};

#endif // PATIENTINFOMANAGER_H
