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
    QString pid, name, sex, birthdate, tel, address, memo;
    QPixmap *pixmap;

    int waitSignal;
    int count =0;

private slots:
    //void on_modifyPatientInfo_pushButton_clicked();
    void on_searchPushButton_clicked();
    void searchDataSended(QString, QString);

    void on_deletePushButton_clicked();

    void on_WaitPushButton_clicked();

    void on_modifyPushButton_clicked();

    void inWaitListSlot(int);

signals:
    void sendSearchData(QString);
    void sendDelData(QString);
//    void sendWaitInfo(QString, QString);
    void sendWaitInfo(QString);
    //void sendWaitToServer(QString);
    void sendModifyData(QString);
    void sendPIDtoWaitList(QString);
};

#endif // PATIENTINFOMANAGER_H
