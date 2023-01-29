#ifndef PATIENTSTATUSMANAGER_H
#define PATIENTSTATUSMANAGER_H

#include <QWidget>
#include <QTreeWidget>

namespace Ui {
class PatientStatusManager;
}

class PatientStatusManager : public QWidget
{
    Q_OBJECT

public:
    explicit PatientStatusManager(QWidget *parent = nullptr);
    ~PatientStatusManager();

private:
    Ui::PatientStatusManager *ui;
    QString treatPID, treatName, payPID, payName;

    //QTreeWidgetItem* row;
    QTreeWidgetItem* selectedTreatRow, *selectedPayRow;
    QString tempTreatPID;
private slots:
    //void waitInfoSended(QString, QString);
    void waitInfoSended(QString);
    void on_waitPaymentTreeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_waitTreatmentTreeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_shootRequestPushButton_clicked();
    //void sendSRQRequest(QString);


    //void SRQRequestSended(QString);

    void statusRequestSended(QString);

    void on_paymentPushButton_clicked();

    void PIDsendedtoWaitList(QString);

signals:
    void sendRequest(QString);
    void inWaitListSignal(int);
};

#endif // PATIENTSTATUSMANAGER_H
