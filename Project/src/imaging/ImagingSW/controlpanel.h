#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include <QButtonGroup>
#include <QImage>

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
    void checkTypeButton(QString);
    void controlButtonClicked(QAbstractButton*);
    void receiveButtonControl(int);
    void resetButtonClicked();
    bool readyButtonClicked();
    bool startButtonClicked();
    void stopButtonClicked();

    void shootingEndSlot(QString);

private:
    Ui::ControlPanel *ui;

    QString requestType = "NULL";
    QString currentPID = "NULL";
    QString currentType = "NULL";
    QButtonGroup *controlButtonGroup;

signals:
    void buttonSignal(int, QString);
    void readySignal(QString);
    void startSignal();
    void stopSignal();
};

#endif // CONTROLPANEL_H
