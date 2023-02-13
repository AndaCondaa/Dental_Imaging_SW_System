/*
 * 프로그램명 : ImagingSW
 * 파일명 : controlpanel.h
 * 설명 : CT 장비 제어동작 구현
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.02.13
 */

#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>

class QAbstractButton;
class QButtonGroup;

namespace Ui {
class ControlPanel;
}

class ControlPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ControlPanel(QWidget *parent = nullptr);
    ~ControlPanel();

private slots:
    void settingStyleSheet();
    void checkTypeButton(QString);
    void controlButtonClicked(QAbstractButton*);
    void receiveButtonControl(int);

    void resetControl();
    bool readyControl();
    void startControl();
    void stopControl();
    void saveSlot(QString);
    void shootingEndSlot(QString);
    void finishSlot(QString, QString);

private:
    Ui::ControlPanel *ui;

    QString requestType = "NULL";
    QString currentPID = "NULL";
    QString currentType = "NULL";

    QButtonGroup *modeButtonGroup;
    QButtonGroup *controlButtonGroup;

signals:
    void buttonSignal(int, QString);
    void readySignal(QString);
    void startSignal(QString, QString);
    void stopSignal();
};

#endif // CONTROLPANEL_H
