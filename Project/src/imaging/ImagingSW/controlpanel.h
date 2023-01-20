#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include <QButtonGroup>

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
    void controlButtonClicked(QAbstractButton*);
    void receiveButtonControl(int);
    void checkTypeButton(QString);
    void readyButtonClicked();
    void startButtonClicked();

private:
    Ui::ControlPanel *ui;

    QButtonGroup *buttonGroup;

signals:
    void buttonSignal(int, QString);
    void readyType(QString);
    void startSignal();
};

#endif // CONTROLPANEL_H
