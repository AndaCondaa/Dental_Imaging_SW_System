#ifndef MEDICALINFOMANAGER_H
#define MEDICALINFOMANAGER_H

#include <QWidget>

namespace Ui {
class MedicalInfoManager;
}

class MedicalInfoManager : public QWidget
{
    Q_OBJECT

public:
    explicit MedicalInfoManager(QWidget *parent = nullptr);
    ~MedicalInfoManager();

private:
    Ui::MedicalInfoManager *ui;
};

#endif // MEDICALINFOMANAGER_H
