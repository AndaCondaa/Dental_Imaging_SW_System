#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "imagemanager.h"
#include <QMainWindow>


namespace Ui { class MainWindow ; }

class EnrollManager;
class ImageManager;
class MedicalRecordManager;
class PatientInfoManager;
class PatientStatusManager;
class NetworkManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:
    void on_enrollButton_clicked();
    //void sendProtocol(int, char *name);


private:
    Ui::MainWindow *ui;

    EnrollManager *enrollManager;
    ImageManager *imageManager;
    MedicalRecordManager *medicalRecordManager;
    PatientInfoManager *patientInfoManager;
    PatientStatusManager *patientStatusManager;
    NetworkManager *networkManager;


signals:
    //void enrollSignal(int, char name[10]);
    void requestPID(QString);
};
#endif // MAINWINDOW_H
