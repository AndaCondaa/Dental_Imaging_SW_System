#ifndef PATIENTSTATUSMANAGER_H
#define PATIENTSTATUSMANAGER_H

#include <QWidget>

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
};

#endif // PATIENTSTATUSMANAGER_H
