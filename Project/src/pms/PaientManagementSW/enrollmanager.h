#ifndef ENROLLMANAGER_H
#define ENROLLMANAGER_H

#include <QWidget>


namespace Ui {
class EnrollManager;
}

class EnrollManager : public QWidget
{
    Q_OBJECT

public:
    explicit EnrollManager(QWidget *parent = nullptr);
    ~EnrollManager();

private:
    Ui::EnrollManager *ui;

};

#endif // ENROLLMANAGER_H
