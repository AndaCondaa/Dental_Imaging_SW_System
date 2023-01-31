#ifndef IMAGINGMANAGER_H
#define IMAGINGMANAGER_H

#include <QWidget>

namespace Ui {
class ImagingManager;
}

class ImageThread;

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
    void stopButtonSlot();

    void isProgressMaximum(int);

    void on_tempReconButton_clicked();
    void on_tempFilterButton_2_clicked();

private:
    Ui::ImagingManager *ui;

    QString currentPID = "NULL";
    QString currentType = "NULL";

    ImageThread *thread;

signals:
    void shootingEndSignal(QString);
    void saveSignal(QString);
    void stopThread();
};

#endif // IMAGINGMANAGER_H
