#ifndef PATIENTINFOMANAGER_H
#define PATIENTINFOMANAGER_H

#include <QWidget>

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
};

#endif // PATIENTINFOMANAGER_H
