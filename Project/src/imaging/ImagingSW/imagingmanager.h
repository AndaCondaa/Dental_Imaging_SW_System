/*
 * 프로그램명 : ImagingSW
 * 파일명 : imagingmanager.h
 * 설명 : 이미지 정합 및 저장 구현
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.02.13
 */

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
    void startSetting(QString pid, QString type);
    void saveButtonSlot();
    void stopButtonSlot();

    void isProgressMaximum(int);

    void recvFrameImg(int);

    QString makeFileName(QString type, int count);
    void histoStretch(unsigned short *input, int inputSize, int min, int max, double maxValue);
    void CLAHE(unsigned short *input, int rows, int cols, double clipLimit, int gridX, int gridY);
    void medianFilter(unsigned short *input, int rows, int cols, int ksize);
    void gammaCorrection(unsigned short *input, int inputSize, double maxValue, double gamma);
    void unsharpFilter(unsigned short *input, int rows, int cols, double maxValue);
    void invertImage(unsigned short *input, int inputSize);
    void saveAsJpg(unsigned short *input, int rows, int cols);
    void viewReconImage(unsigned short *input, int rows, int cols);

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
