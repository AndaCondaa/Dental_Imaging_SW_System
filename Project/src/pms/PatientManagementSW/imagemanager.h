/*
 * 프로그램명 : PatientManagementSW
 * 파일명 : imageManager.h
 * 설명 : 환자 검색 시 해당 환자의 촬영 이미지를 띄워줌
 * 작성자 : 김유선
 * 최종 수정일 : 2023.02.16
 */


#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <QWidget>

#include <QtTest/QtTest>
#include <QTest>

namespace Ui {
class ImageManager;
}

class ImageManager : public QWidget
{
    Q_OBJECT

public:
    explicit ImageManager(QWidget *parent = nullptr);
    ~ImageManager();

private slots:
    void reloadImages(QString);
    void PSEDataSended(QString);
    void delPIDSendedToImg(QString);
    void cleanImageSlot();

private:
    Ui::ImageManager *ui;
    QString pid;
};

#endif // IMAGEMANAGER_H
