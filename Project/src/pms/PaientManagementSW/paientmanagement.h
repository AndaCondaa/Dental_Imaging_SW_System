#ifndef PAIENTMANAGEMENT_H
#define PAIENTMANAGEMENT_H

#include <QMainWindow>
#include <QtCore>
#include <QtNetwork>

namespace Ui { class PaientManagement; }

class PaientManagement : public QMainWindow
{
    Q_OBJECT

public:
    PaientManagement(QWidget *parent = nullptr);
    ~PaientManagement();

public slots:
    bool connectToHost(QString host);
    bool writeData(QByteArray data);


private slots:
    void on_modifyPatientInfo_pushButton_clicked();

private:
    Ui::PaientManagement *ui;
    QTcpSocket *socket;
    bool fd_flag = false;
    bool send_flag = false;
    int cnt = 0;
};
#endif // PAIENTMANAGEMENT_H
