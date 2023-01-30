#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H

#include <QWidget>

namespace Ui {
class Prescription;
}

class Prescription : public QWidget
{
    Q_OBJECT

public:
    explicit Prescription(QWidget *parent = nullptr);
    ~Prescription();


private:
    Ui::Prescription *ui;
    QString DoctorID;
    QString DoctorName;
    QString PatientID;
    QString PatientName;
    QString PatientSex;
    QString CurrentDate;

private slots:
    void receivePrescription(QString, QString, QString, QString, QString);
    void on_Finish_clicked();

signals:
    void sendPrescriptionFinish(QString);
};

#endif // PRESCRIPTION_H
