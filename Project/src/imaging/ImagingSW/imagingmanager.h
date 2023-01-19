#ifndef IMAGINGMANAGER_H
#define IMAGINGMANAGER_H

#include <QWidget>

namespace Ui {
class ImagingManager;
}

class ImagingManager : public QWidget
{
    Q_OBJECT

public:
    explicit ImagingManager(QWidget *parent = nullptr);
    ~ImagingManager();

private slots:
    void setPID(QString);
    void setType(QString);

    void on_reconCancelButton_clicked();
    void raw16ToBmp8();
    void simpleStiching();
    void reconImage();
    void saveButtonSlot();

private:
    Ui::ImagingManager *ui;

    QString m_pid = "NULL";
    QString m_type = "NULL";

signals:
    void saveSignal(QString);
};

#endif // IMAGINGMANAGER_H
