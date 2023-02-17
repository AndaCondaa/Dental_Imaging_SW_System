/*
 * 프로그램명 : PatientManagementSW
 * 파일명 : choosePatientManager.h
 * 설명 : 동명이인 처리
 * 작성자 : 김유선
 * 최종 수정일 : 2023.02.16
 */


#ifndef CHOOSEPATIENTMANAGER_H
#define CHOOSEPATIENTMANAGER_H
#include <QWidget>
#include <QTreeWidget>

#include <QtTest/QtTest>
#include <QTest>

namespace Ui {
class ChoosePatientManager;
}

class ChoosePatientManager : public QWidget
{
    Q_OBJECT

public:
    explicit ChoosePatientManager(QWidget *parent = nullptr);
    ~ChoosePatientManager();

private:
    Ui::ChoosePatientManager *ui;

private slots:
    void sendedTempData(QString, QString);

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

signals:
    void choosePatientSignal(QString);
};

#endif // CHOOSEPATIENTMANAGER_H
