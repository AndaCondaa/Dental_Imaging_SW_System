#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "prescription.h"
#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>
#include <QWheelEvent>
#include <QGraphicsView>
#include <QObject>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ImageView;
class ImageAlbum;
class PatientInfo;
class NetworkManager;
class LoginForm;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QImage *img;
    ImageAlbum* m_imageAlbum;
    PatientInfo* m_patientInfo;
    ImageView* m_imageView;
    NetworkManager* m_networkManager;
    LoginForm* m_logIn;

private slots:
    void ViewerOpen(QString, QString);
};
#endif // MAINWINDOW_H
