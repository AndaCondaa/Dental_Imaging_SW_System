#include "cbctrawimageviewer.h"
#include <QFile>
#include <QImage>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QtWidgets>
#include <QTimer>
#include <QPixmap>
#include <QDebug>


#define update_image();

CBCTRawImageViewer::CBCTRawImageViewer()
{
    QString panoFolder = "C:/Qt_VTK_CT/build/Debug/Pano_Frame(1152x64)";
    panoImageIterator = new QDirIterator(panoFolder, QDirIterator::Subdirectories);
    panoImageTimer = new QTimer(this);
    connect(panoImageTimer, &QTimer::timeout, this, &CBCTRawImageViewer::timeoutPanoTimer);

    QString cephFolder = "C:/Qt_VTK_CT/build/Debug/Ceph_Frame(48x2400)";
    cephImageIterator = new QDirIterator(cephFolder, QDirIterator::Subdirectories);
    cephImageTimer = new QTimer(this);
    connect(cephImageTimer, &QTimer::timeout, this, &CBCTRawImageViewer::timeoutCephTimer);
}



QPixmap CBCTRawImageViewer::PanoImageViewer()
{
    return QPixmap();
}

QPixmap CBCTRawImageViewer::CephImageViewer()
{
    return QPixmap();
}
void CBCTRawImageViewer::resetPanoTimer()
{

    QString panoFolder = "C:/Qt_VTK_CT/build/Debug/Pano_Frame(1152x64)";
    panoImageIterator = new QDirIterator(panoFolder, QDirIterator::Subdirectories);
    panoImageTimer->start();
    panoImageTimer->stop();


    //QGraphicsScene* panoScene = new QGraphicsScene();
    //m_mainwindowUi->PanoGraphicsView->resetTransform();
    //panoScene->clear();
 //   m_mainwindowUi->PanoGraphicsView->setScene(panoScene);
    
}

void CBCTRawImageViewer::resetCephTimer()
{

    QString cephFolder = "C:/Qt_VTK_CT/build/Debug/Ceph_Frame(48x2400)";
    cephImageIterator = new QDirIterator(cephFolder, QDirIterator::Subdirectories);
    cephImageTimer->start();
    cephImageTimer->stop();
  /*  QGraphicsScene* cephScene = new QGraphicsScene();

    m_mainwindowUi->CephGraphicsView->setScene(cephScene);
    cephScene->clear();*/
}

void CBCTRawImageViewer::readyPanoTimer()
{

}

void CBCTRawImageViewer::readyCephTimer()
{


}
void CBCTRawImageViewer::startPanoTimer()
{
    qDebug() << __FUNCTION__;
    // 타이머 시작
    panoImageTimer->start(10);
}

void CBCTRawImageViewer::startCephTimer()
{
    qDebug() << __FUNCTION__;
    // 타이머 시작
    cephImageTimer->start(10);
}

void CBCTRawImageViewer::stopPanoTimer()
{
    // 타이머 종료
    qDebug() << __FUNCTION__;
    panoImageTimer->stop();
//
}

void CBCTRawImageViewer::stopCephTimer()
{
    // 타이머 종료
    qDebug() << __FUNCTION__;
    cephImageTimer->stop();
}

//void CBCTRawImageViewer::slot_panoImage(QImage* panoImage)
//{
//    qDebug() << __FUNCTION__;
//    QGraphicsScene* panoScene = new QGraphicsScene();


//    QImage pano_Image(*panoImage);
//    QPixmap panoPix;
//    panoPix = QPixmap::fromImage(pano_Image, Qt::AutoColor);

//    /* 파노라마 이미지가 90도 회전되어 있으므로, 출력시 원상복구한다 */
//    QTransform panoTransform;
//    panoTransform.rotate(90);
//    panoScene->addPixmap(panoPix.transformed(panoTransform));
//    m_mainwindowUi->PanoLabel->setPixmap(panoPix.transformed(panoTransform));
//    m_mainwindowUi->PanoGraphicsView->setScene(panoScene);
//    /* 파노라마 Raw Image 전송상태를 표시해주는 ProgressBar */
//    int panoValue = m_mainwindowUi->PanoProgressBar->value();
//    panoValue++;
//    m_mainwindowUi->PanoProgressBar->setValue(panoValue);
//}

//void CBCTRawImageViewer::slot_cephImage(QImage* cephImage)
//{
//    qDebug() << __FUNCTION__;
//    QGraphicsScene* cephScene = new QGraphicsScene();


//    QImage ceph_Image(*cephImage);
//    QPixmap cephPix;
//    cephPix = QPixmap::fromImage(ceph_Image, Qt::AutoColor);
//    cephScene->addPixmap(cephPix);
//    m_mainwindowUi->CephLabel->setPixmap(cephPix);
//    m_mainwindowUi->CephGraphicsView->setScene(cephScene);
//    /* 세팔로 Raw Image 전송상태를 표시해주는 ProgressBar */
//    int cephValue = m_mainwindowUi->CephProgressBar->value();
//    cephValue++;
//    m_mainwindowUi->CephProgressBar->setValue(cephValue);

//}

void CBCTRawImageViewer::timeoutPanoTimer()
{


    // 타이머 함수 -> mainwindow로 이미지 파일 하나씩 읽어 보내주도록 한다.
    if (panoImageIterator->hasNext())
    {
        panoImageIterator->next();

        QString panoPath = panoImageIterator->filePath();

        qDebug() << __FUNCTION__ << panoPath;

        QFile panoFile(panoPath);
        if (!panoFile.open(QFile::ReadOnly))
            return;
        QByteArray pBa = panoFile.readAll();
        const uchar* pData = (const uchar*) pBa.constData();;
        panoFile.close();
        QImage* panoImage = new QImage(pData, 740, 100, QImage::Format_RGB555);
        emit signals_panoImage(panoImage);
    }
    else
    {
        //imageIterator->rewind();
    }


//return QImage();

}

void CBCTRawImageViewer::timeoutCephTimer()
{


    // 타이머 함수 -> mainwindow로 이미지 파일 하나씩 읽어 보내주도록 한다.
    if (cephImageIterator->hasNext())
    {
        cephImageIterator->next();

        QString cephPath = cephImageIterator->filePath();

        qDebug() << __FUNCTION__ << cephPath;

        QFile cephFile(cephPath);
        if (!cephFile.open(QFile::ReadOnly))
            return;
        QByteArray cBa = cephFile.readAll();
        const uchar* cData = (const uchar*) cBa.constData();;
        cephFile.close();
//        QImage* cephImage = new QImage(cData, 150, 470, QImage::Format_Grayscale16);
        QImage* cephImage = new QImage(cData, 100, 740, QImage::Format_RGB555);
        emit signals_cephImage(cephImage);
    }
    else
    {
        //imageIterator->rewind();
    }
 //   return QImage();

}

CBCTRawImageViewer::~CBCTRawImageViewer()
{

}
