#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "imagemanager.h"
#include <QMainWindow>
#include <QtCore>
#include <QtNetwork>

namespace Ui { class MainWindow ; }

class EnrollManager;
class ImageManager;
class MedicalRecordManager;
class PatientInfoManager;
class PatientStatusManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    bool connectToHost(QString host);
    bool writeData(QByteArray data);




private slots:
    void on_enrollButton_clicked();
    //void sendProtocol(int, char *name);

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    bool fd_flag = false;
    bool send_flag = false;
    int pid = 1; // 임시
    char name[10] = "김유선"; //임시

    EnrollManager *enrollManager;
    ImageManager *imageManager;
    MedicalRecordManager *medicalRecordManager;
    PatientInfoManager *patientInfoManager;
    PatientStatusManager *patientStatusManager;

    QTcpSocket *PMSocket;

signals:
    //void enrollSignal(int, char name[10]);
};
#endif // MAINWINDOW_H
