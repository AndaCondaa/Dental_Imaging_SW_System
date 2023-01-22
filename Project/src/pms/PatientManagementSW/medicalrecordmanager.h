#ifndef MEDICALRECORDMANAGER_H
#define MEDICALRECORDMANAGER_H

#include <QWidget>

namespace Ui {
class MedicalRecordManager;
}

class MedicalRecordManager : public QWidget
{
    Q_OBJECT

public:
    explicit MedicalRecordManager(QWidget *parent = nullptr);
    ~MedicalRecordManager();

private slots:
    void recordDataSended(QString, QString);

private:
    Ui::MedicalRecordManager *ui;
};

#endif // MEDICALRECORDMANAGER_H
