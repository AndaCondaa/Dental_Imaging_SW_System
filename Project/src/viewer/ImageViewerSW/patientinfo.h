#ifndef PATIENTINFO_H
#define PATIENTINFO_H

#include <QWidget>
#include <QTreeWidgetItem>

namespace Ui {
class PatientInfo;
}

class PatientInfo : public QWidget
{
    Q_OBJECT

public:
    explicit PatientInfo(QWidget *parent = nullptr);
    ~PatientInfo();

private:
    Ui::PatientInfo *ui;
    QString selectPatientID;
    QString selectPatientName;
    QString patientSex;
    QString patientID;
    QString patientName;
    bool Ceph;
    bool Pano;
    bool AllSendImageCheck;

private slots:
    void receiveWaitingList(QString, QString);
    void on_WaitingList_itemClicked(QTreeWidgetItem *item, int column);
    void on_Treatmentstart_clicked();

    void receiveSelectPatient(QString, QString);
    void receivePMSCameraPatient(QString, QString);

    void on_Camerastart_clicked();

    void receiveDoctorName(QString, QString);
    void receivePhotoEnd(QString);
    void receiveEndTreatment(QString);
    void receiveWaitTreatment(int, QString);
    void receiveEndSignal();
    void receiveAllImageFileP(bool);

signals:
    void sendWaitingPatient(QString);
    void sendCameraPatient(QString);
    void sendPatientInfo(QString, QString, QString);
    void sendCameraStart();
};

#endif // PATIENTINFO_H
