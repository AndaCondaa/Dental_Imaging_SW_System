#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QObject>

class ControlPanel : public QObject
{
    Q_OBJECT
public:
    explicit ControlPanel(QObject *parent = nullptr);

signals:

};

#endif // CONTROLPANEL_H
