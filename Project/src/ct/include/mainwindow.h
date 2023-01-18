#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow.h>
#include <QObject.h>

class CBCTFileTransfer;
class CBCTLogThread;
class CBCTModelController;
class CBCTRawImageViewer;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_MainPushButton_clicked();

    void on_SubPushButton_clicked();

    void on_CaptureStartPushButton_clicked();
    void on_CaptureStopPushButton_clicked();

    //    void on_AscendingPushButton_pressed();

    //    void on_AscendingPushButton_released();

    //    void on_DescendingPushButton_pressed();

    //    void on_DescendingPushButton_released();

private:
    void loadObj();

    CBCTFileTransfer * m_fileTransfer;
    CBCTLogThread * m_logThread;
    CBCTModelController * m_modelController;
    CBCTRawImageViewer * m_rawImageViewer;

    Ui::MainWindow *ui;

private slots:
    void slot_panoImage(QImage* pImg);
    void slot_cephImage(QImage* cImg);

};
#endif // MAINWINDOW_H
