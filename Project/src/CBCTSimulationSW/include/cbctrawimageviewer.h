#ifndef CBCTRAWIMAGEVIEWER_H
#define CBCTRAWIMAGEVIEWER_H

#include <QObject>
#include <qdebug.h>
#include "ui_mainwindow.h"

class QDir;
class QDirIterator;
class QTimer;
class QElapsedTimer;

class CBCTRawImageViewer : public QObject
{
    Q_OBJECT
public:
    explicit CBCTRawImageViewer();
    ~CBCTRawImageViewer();
    Ui::MainWindow* m_parentUI = nullptr;

    QPixmap PanoImageViewer();
    QPixmap CephImageViewer();

//    QImage timeoutPanoTimer();
//    QImage timeoutCephTimer();

    void resetPanoTimer();
    void resetCephTimer();
    void readyPanoTimer();
    void readyCephTimer();
    void startPanoTimer();
    void stopPanoTimer();

    void startCephTimer();
    void stopCephTimer();

private:


    QDirIterator* panoImageIterator;
    QDirIterator* cephImageIterator;
    QTimer* panoImageTimer;
    QTimer* cephImageTimer;
public slots:
    void viewer_Output();

private slots:
        void timeoutPanoTimer();
        void timeoutCephTimer();
//    void slot_panoImage(QImage* panoImage);
//    void slot_cephImage(QImage* cephImage);

signals:
    void signals_panoImage(QImage*);
    void signals_cephImage(QImage*);

};

#endif // CBCTRAWIMAGEVIEWER_H
