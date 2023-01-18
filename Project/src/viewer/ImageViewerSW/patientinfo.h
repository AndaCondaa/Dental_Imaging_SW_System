#ifndef PATIENTINFO_H
#define PATIENTINFO_H

#include <QWidget>

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
};

#endif // PATIENTINFO_H
