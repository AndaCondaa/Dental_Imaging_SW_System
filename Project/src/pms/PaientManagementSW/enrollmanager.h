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

private slots:
    void on_pushButton_clicked();

private:
    Ui::EnrollManager *ui;

signals:
    void sendNewData(QString);

};

#endif // ENROLLMANAGER_H
