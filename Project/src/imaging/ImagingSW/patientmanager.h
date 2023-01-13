#ifndef PATIENTMANAGER_H
#define PATIENTMANAGER_H

#include <QWidget>
#include <QTreeWidget>

namespace Ui {
class PatientManager;
}

class PatientManager : public QWidget
{
    Q_OBJECT

public:
    explicit PatientManager(QWidget *parent = nullptr);
    ~PatientManager();

private slots:
    void receiveWaitPatient(QStringList dataList);
    void deletePatient();
    void slotPatientReady();

private:
    Ui::PatientManager *ui;

    QMap<QString, QString> typeMap;

signals:
    void sendPid(QString);
};

#endif // PATIENTMANAGER_H
