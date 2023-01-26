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
    void raw16ToBmp8();

    void setPID(QString);
    void setType(QString);

    void loadImage();
    void reconImage();
    void saveButtonSlot();
    void finishButtonSlot();

    void stopButtonSlot();

    void isProgressMaximum(int);

    void on_filter1Button_clicked();

    void on_filter2Button_2_clicked();

private:
    Ui::ImagingManager *ui;

    QString currentPID = "NULL";
    QString currentType = "NULL";

signals:
    void shootingEndSignal(QString);
    void saveSignal(QString);
    void finishSignal(QString);
    void stopThread();
};

#endif // IMAGINGMANAGER_H
