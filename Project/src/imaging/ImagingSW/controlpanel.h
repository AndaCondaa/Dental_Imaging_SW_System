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
    void readyButtonClicked();
    bool startButtonClicked();
    void stopButtonClicked();
private:
    Ui::ControlPanel *ui;

    QString currentType = "NULL";
    QButtonGroup *controlButtonGroup;

    QImage panoAct;


signals:
    void buttonSignal(int, QString);
    void readySignal(QString);
    void startSignal();
};

#endif // CONTROLPANEL_H
