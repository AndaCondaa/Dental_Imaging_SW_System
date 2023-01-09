#ifndef PATIENTMANAGER_H
#define PATIENTMANAGER_H

#include <QWidget>

namespace Ui {
class PatientManager;
}

class PatientManager : public QWidget
{
    Q_OBJECT

public:
    explicit PatientManager(QWidget *parent = nullptr);
    ~PatientManager();

private:
    Ui::PatientManager *ui;
};

#endif // PATIENTMANAGER_H
