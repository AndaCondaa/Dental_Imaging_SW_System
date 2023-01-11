#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include "protocol.h"

#include <QWidget>

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

private:
    Ui::ControlPanel *ui;
};

#endif // CONTROLPANEL_H
