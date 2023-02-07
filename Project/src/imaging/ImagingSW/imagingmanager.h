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
    void setPID(QString);
    void setType(QString);

    void reconImage();
    void saveButtonSlot();
    void stopButtonSlot();

    void isProgressMaximum(int);

    void recvFrameImg(int);

    QString makeFileName(QString type, int count);
    void histoStretch(unsigned short *input, int inputSize, int min, int max, double valueMax);
    void CLAHE(unsigned short *input, int rows, int cols, int clipLimit, int gridX, int gridY);
    void medianFilter(unsigned short *input, int rows, int cols, int ksize);
    void gammaCorrection(unsigned short *input, int inputSize, double valueMax, double gamma);
    void unsharpFilter(unsigned short *input, int rows, int cols, int valueMax);
    void invertImage(unsigned short *input, int inputSize);
    void saveAsJpg(unsigned short *input, int rows, int cols);
    void viewReconImage(int rows, int cols);

private:
    Ui::ImagingManager *ui;

    QString currentPID = "NULL";
    QString currentType = "NULL";

    ImageThread *thread;

signals:
    void sendCount(int);
    void shootingEndSignal(QString);
    void saveSignal(QString);
    void stopThread();
};

#endif // IMAGINGMANAGER_H
