#include "cbctrawimageviewer.h"
#include <QFile>
#include <QImage>
//#include <QGraphicsPixmapItem>
//#include <QGraphicsView>
//#include <QGraphicsScene>
#include <QtWidgets>
#include <QTimer>
#include <QPixmap>
#include <QDebug>


#define update_image();

CBCTRawImageViewer::CBCTRawImageViewer()
{
    m_parentUI = new Ui::MainWindow;
    QDir panoDir("C:/Qt_VTK_CT/RawImages/Pano_Frame(1152x64)");

    panoImageIterator = new QDirIterator(panoDir, QDirIterator::Subdirectories);
    panoImageTimer = new QTimer(this);
    panoImageTimer->setInterval(5);
    connect(panoImageTimer, &QTimer::timeout, this, &CBCTRawImageViewer::timeoutPanoTimer);

    QDir cephDir("C:/Qt_VTK_CT/RawImages/Ceph_Frame(48x2400)");

    cephImageIterator = new QDirIterator(cephDir, QDirIterator::Subdirectories);
    cephImageTimer = new QTimer(this);
    cephImageTimer->setInterval(5);
    connect(cephImageTimer, &QTimer::timeout, this, &CBCTRawImageViewer::timeoutCephTimer);
}

void CBCTRawImageViewer::viewer_Output()
{
    if( m_parentUI->PanoCheckBox->isChecked())
    {
        startPanoTimer();
    }
    if( m_parentUI->CephCheckBox->isChecked())
    {
        startCephTimer();
    }
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
    qDebug() << __FUNCTION__;
    QDir panoDir("C:/Qt_VTK_CT/RawImages/Pano_Frame(1152x64)");

    panoImageIterator = new QDirIterator(panoDir, QDirIterator::Subdirectories);
    panoImageTimer->start();
    panoImageTimer->stop();
}

void CBCTRawImageViewer::resetCephTimer()
{
    qDebug() << __FUNCTION__;
    QDir cephDir("C:/Qt_VTK_CT/RawImages/Ceph_Frame(48x2400)");

    cephImageIterator = new QDirIterator(cephDir, QDirIterator::Subdirectories);
    cephImageTimer->start();
    cephImageTimer->stop();
}

void CBCTRawImageViewer::readyPanoTimer()
{
    qDebug() << __FUNCTION__;
}

void CBCTRawImageViewer::readyCephTimer()
{
    qDebug() << __FUNCTION__;

}
void CBCTRawImageViewer::startPanoTimer()
{
    qDebug() << __FUNCTION__;
    // 타이머 시작
    panoImageTimer->start();
}

void CBCTRawImageViewer::startCephTimer()
{
    qDebug() << __FUNCTION__;
    // 타이머 시작
    cephImageTimer->start();
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

void CBCTRawImageViewer::timeoutPanoTimer()
{
    qDebug("find bug test 0");
    // 타이머 함수 -> mainwindow로 이미지 파일 하나씩 읽어 보내주도록 한다.
    if (panoImageIterator->hasNext())
    {
        panoImageIterator->next();

        QString panoPath = panoImageIterator->filePath();
        if (panoPath.contains(".raw"))
        {
            qDebug("find bug test 1");
            qDebug() << __FUNCTION__ << panoPath;

            QFile panoFile(panoPath);
            if (!panoFile.open(QFile::ReadOnly))
                return;
            QByteArray pBa = panoFile.readAll();
            const uchar* pData = (const uchar*)pBa.constData();;
            panoFile.close();
            panoFile.deleteLater();
            QImage* panoImage = new QImage(pData, 740, 100, QImage::Format_Grayscale16);
            emit signals_panoImage(panoImage);

            qDebug("find bug test 2");
        }
        else 
        {
            qDebug() << "not Find Raw file.";
        }
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
        if (cephPath.contains(".raw"))
        {
            qDebug() << __FUNCTION__ << cephPath;

            QFile cephFile(cephPath);
            if (!cephFile.open(QFile::ReadOnly))
                return;
            QByteArray cBa = cephFile.readAll();
            const uchar* cData = (const uchar*)cBa.constData();;
            cephFile.close();
            cephFile.deleteLater();
            QImage* cephImage = new QImage(cData, 100, 740, QImage::Format_Grayscale16);
            emit signals_cephImage(cephImage);
        }
        else {
            qDebug() << "not Find Raw file.";
        }
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
