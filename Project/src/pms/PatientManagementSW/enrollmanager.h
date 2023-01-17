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
    void newPIDSended(QString);

private:
    Ui::EnrollManager *ui;
    QString newPID;

signals:
    void sendNewData(QString);
    void sendNewDataForShow(QString, QString);

};

#endif // ENROLLMANAGER_H
