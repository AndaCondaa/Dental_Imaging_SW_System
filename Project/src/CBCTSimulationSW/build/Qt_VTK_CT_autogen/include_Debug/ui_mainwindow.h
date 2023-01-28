/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qvtkopenglstereowidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QCheckBox *PanoCheckBox;
    QCheckBox *CephCheckBox;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *ControlVerticalLayout;
    QGroupBox *ControlGroupBox;
    QPushButton *CaptureResetPushButton;
    QPushButton *CaptureReadyPushButton;
    QPushButton *CaptureStartPushButton;
    QPushButton *CaptureStopPushButton;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *CBCTVerticalLayout;
    QGroupBox *CBCTGroupBox;
    QPushButton *SubPushButton;
    QPushButton *MainPushButton;
    QPushButton *AscendingPushButton;
    QPushButton *DescendingPushButton;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *RAWHorizontalLayout;
    QGroupBox *RawGroupBox;
    QLabel *CephTextLabel;
    QLabel *PanoTextLabel;
    QLabel *PanoLabel;
    QLabel *CephLabel;
    QProgressBar *PanoProgressBar;
    QProgressBar *CephProgressBar;
    QWidget *gridLayoutWidget;
    QGridLayout *VTKLayout;
    QGroupBox *VTKGroupBox;
    QVTKOpenGLStereoWidget *openGLWidget_All;
    QVTKOpenGLStereoWidget *openGLWidget_Main;
    QVTKOpenGLStereoWidget *openGLWidget_Sub;
    QWidget *widget;
    QGridLayout *gridLayout;
    QTableWidget *tableWidget;
    QTableWidget *tableWidget_2;
    QMenuBar *menubar;
    QMenu *menuCBCT_Simulator;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1248, 812);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        QBrush brush1(QColor(240, 240, 240, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        MainWindow->setPalette(palette);
        QIcon icon;
        QString iconThemeName = QString::fromUtf8("applications-graphics");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        MainWindow->setWindowIcon(icon);
        MainWindow->setToolButtonStyle(Qt::ToolButtonIconOnly);
        MainWindow->setTabShape(QTabWidget::Rounded);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        PanoCheckBox = new QCheckBox(centralwidget);
        PanoCheckBox->setObjectName("PanoCheckBox");
        PanoCheckBox->setGeometry(QRect(50, 10, 91, 20));
        sizePolicy.setHeightForWidth(PanoCheckBox->sizePolicy().hasHeightForWidth());
        PanoCheckBox->setSizePolicy(sizePolicy);
        CephCheckBox = new QCheckBox(centralwidget);
        CephCheckBox->setObjectName("CephCheckBox");
        CephCheckBox->setGeometry(QRect(190, 10, 91, 20));
        sizePolicy.setHeightForWidth(CephCheckBox->sizePolicy().hasHeightForWidth());
        CephCheckBox->setSizePolicy(sizePolicy);
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(160, 40, 151, 91));
        ControlVerticalLayout = new QVBoxLayout(verticalLayoutWidget);
        ControlVerticalLayout->setObjectName("ControlVerticalLayout");
        ControlVerticalLayout->setContentsMargins(0, 0, 0, 0);
        ControlGroupBox = new QGroupBox(verticalLayoutWidget);
        ControlGroupBox->setObjectName("ControlGroupBox");
        sizePolicy.setHeightForWidth(ControlGroupBox->sizePolicy().hasHeightForWidth());
        ControlGroupBox->setSizePolicy(sizePolicy);
        CaptureResetPushButton = new QPushButton(ControlGroupBox);
        CaptureResetPushButton->setObjectName("CaptureResetPushButton");
        CaptureResetPushButton->setGeometry(QRect(10, 10, 60, 30));
        sizePolicy.setHeightForWidth(CaptureResetPushButton->sizePolicy().hasHeightForWidth());
        CaptureResetPushButton->setSizePolicy(sizePolicy);
        CaptureResetPushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    border: 2px solid #8f8f91;\n"
"    border-radius: 6px;\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
"    min-width: 40px;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
"}\n"
"\n"
"QPushButton:flat {\n"
"    border: none; /* no border for a flat push button */\n"
"}\n"
"\n"
"QPushButton:default {\n"
"    border-color: navy; /* make the default button prominent */\n"
"}"));
        CaptureReadyPushButton = new QPushButton(ControlGroupBox);
        CaptureReadyPushButton->setObjectName("CaptureReadyPushButton");
        CaptureReadyPushButton->setGeometry(QRect(80, 10, 60, 30));
        sizePolicy.setHeightForWidth(CaptureReadyPushButton->sizePolicy().hasHeightForWidth());
        CaptureReadyPushButton->setSizePolicy(sizePolicy);
        CaptureReadyPushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    border: 2px solid #8f8f91;\n"
"    border-radius: 6px;\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
"    min-width: 40px;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
"}\n"
"\n"
"QPushButton:flat {\n"
"    border: none; /* no border for a flat push button */\n"
"}\n"
"\n"
"QPushButton:default {\n"
"    border-color: navy; /* make the default button prominent */\n"
"}"));
        CaptureStartPushButton = new QPushButton(ControlGroupBox);
        CaptureStartPushButton->setObjectName("CaptureStartPushButton");
        CaptureStartPushButton->setGeometry(QRect(10, 50, 60, 30));
        sizePolicy.setHeightForWidth(CaptureStartPushButton->sizePolicy().hasHeightForWidth());
        CaptureStartPushButton->setSizePolicy(sizePolicy);
        CaptureStartPushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    border: 2px solid #8f8f91;\n"
"    border-radius: 6px;\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
"    min-width: 40px;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
"}\n"
"\n"
"QPushButton:flat {\n"
"    border: none; /* no border for a flat push button */\n"
"}\n"
"\n"
"QPushButton:default {\n"
"    border-color: navy; /* make the default button prominent */\n"
"}"));
        CaptureStopPushButton = new QPushButton(ControlGroupBox);
        CaptureStopPushButton->setObjectName("CaptureStopPushButton");
        CaptureStopPushButton->setGeometry(QRect(80, 50, 60, 30));
        sizePolicy.setHeightForWidth(CaptureStopPushButton->sizePolicy().hasHeightForWidth());
        CaptureStopPushButton->setSizePolicy(sizePolicy);
        CaptureStopPushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    border: 2px solid #8f8f91;\n"
"    border-radius: 6px;\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
"    min-width: 40px;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
"}\n"
"\n"
"QPushButton:flat {\n"
"    border: none; /* no border for a flat push button */\n"
"}\n"
"\n"
"QPushButton:default {\n"
"    border-color: navy; /* make the default button prominent */\n"
"}"));

        ControlVerticalLayout->addWidget(ControlGroupBox);

        verticalLayoutWidget_2 = new QWidget(centralwidget);
        verticalLayoutWidget_2->setObjectName("verticalLayoutWidget_2");
        verticalLayoutWidget_2->setGeometry(QRect(9, 40, 151, 91));
        CBCTVerticalLayout = new QVBoxLayout(verticalLayoutWidget_2);
        CBCTVerticalLayout->setObjectName("CBCTVerticalLayout");
        CBCTVerticalLayout->setContentsMargins(0, 0, 0, 0);
        CBCTGroupBox = new QGroupBox(verticalLayoutWidget_2);
        CBCTGroupBox->setObjectName("CBCTGroupBox");
        SubPushButton = new QPushButton(CBCTGroupBox);
        SubPushButton->setObjectName("SubPushButton");
        SubPushButton->setGeometry(QRect(80, 50, 60, 30));
        sizePolicy.setHeightForWidth(SubPushButton->sizePolicy().hasHeightForWidth());
        SubPushButton->setSizePolicy(sizePolicy);
        SubPushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    border: 2px solid #8f8f91;\n"
"    border-radius: 6px;\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
"    min-width: 40px;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
"}\n"
"\n"
"QPushButton:flat {\n"
"    border: none; /* no border for a flat push button */\n"
"}\n"
"\n"
"QPushButton:default {\n"
"    border-color: navy; /* make the default button prominent */\n"
"}"));
        MainPushButton = new QPushButton(CBCTGroupBox);
        MainPushButton->setObjectName("MainPushButton");
        MainPushButton->setGeometry(QRect(10, 50, 60, 30));
        sizePolicy.setHeightForWidth(MainPushButton->sizePolicy().hasHeightForWidth());
        MainPushButton->setSizePolicy(sizePolicy);
        MainPushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    border: 2px solid #8f8f91;\n"
"    border-radius: 6px;\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
"    min-width: 40px;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
"}\n"
"\n"
"QPushButton:flat {\n"
"    border: none; /* no border for a flat push button */\n"
"}\n"
"\n"
"QPushButton:default {\n"
"    border-color: navy; /* make the default button prominent */\n"
"}"));
        AscendingPushButton = new QPushButton(CBCTGroupBox);
        AscendingPushButton->setObjectName("AscendingPushButton");
        AscendingPushButton->setGeometry(QRect(10, 10, 60, 30));
        sizePolicy.setHeightForWidth(AscendingPushButton->sizePolicy().hasHeightForWidth());
        AscendingPushButton->setSizePolicy(sizePolicy);
        AscendingPushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    border: 2px solid #8f8f91;\n"
"    border-radius: 6px;\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
"    min-width: 40px;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
"}\n"
"\n"
"QPushButton:flat {\n"
"    border: none; /* no border for a flat push button */\n"
"}\n"
"\n"
"QPushButton:default {\n"
"    border-color: navy; /* make the default button prominent */\n"
"}"));
        DescendingPushButton = new QPushButton(CBCTGroupBox);
        DescendingPushButton->setObjectName("DescendingPushButton");
        DescendingPushButton->setGeometry(QRect(80, 10, 60, 30));
        sizePolicy.setHeightForWidth(DescendingPushButton->sizePolicy().hasHeightForWidth());
        DescendingPushButton->setSizePolicy(sizePolicy);
        DescendingPushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    border: 2px solid #8f8f91;\n"
"    border-radius: 6px;\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
"    min-width: 40px;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
"}\n"
"\n"
"QPushButton:flat {\n"
"    border: none; /* no border for a flat push button */\n"
"}\n"
"\n"
"QPushButton:default {\n"
"    border-color: navy; /* make the default button prominent */\n"
"}"));

        CBCTVerticalLayout->addWidget(CBCTGroupBox);

        horizontalLayoutWidget = new QWidget(centralwidget);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(310, 0, 291, 771));
        RAWHorizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        RAWHorizontalLayout->setObjectName("RAWHorizontalLayout");
        RAWHorizontalLayout->setContentsMargins(0, 0, 0, 0);
        RawGroupBox = new QGroupBox(horizontalLayoutWidget);
        RawGroupBox->setObjectName("RawGroupBox");
        sizePolicy.setHeightForWidth(RawGroupBox->sizePolicy().hasHeightForWidth());
        RawGroupBox->setSizePolicy(sizePolicy);
        RawGroupBox->setAutoFillBackground(false);
        RawGroupBox->setFlat(false);
        RawGroupBox->setCheckable(false);
        CephTextLabel = new QLabel(RawGroupBox);
        CephTextLabel->setObjectName("CephTextLabel");
        CephTextLabel->setGeometry(QRect(150, 3, 81, 16));
        sizePolicy.setHeightForWidth(CephTextLabel->sizePolicy().hasHeightForWidth());
        CephTextLabel->setSizePolicy(sizePolicy);
        CephTextLabel->setStyleSheet(QString::fromUtf8("font: 14pt \"\354\212\244\354\233\250\352\261\260 TTF\";"));
        PanoTextLabel = new QLabel(RawGroupBox);
        PanoTextLabel->setObjectName("PanoTextLabel");
        PanoTextLabel->setGeometry(QRect(40, 3, 101, 16));
        sizePolicy.setHeightForWidth(PanoTextLabel->sizePolicy().hasHeightForWidth());
        PanoTextLabel->setSizePolicy(sizePolicy);
        PanoTextLabel->setStyleSheet(QString::fromUtf8("font: 14pt \"\354\212\244\354\233\250\352\261\260 TTF\";"));
        PanoLabel = new QLabel(RawGroupBox);
        PanoLabel->setObjectName("PanoLabel");
        PanoLabel->setGeometry(QRect(40, 20, 100, 740));
        sizePolicy.setHeightForWidth(PanoLabel->sizePolicy().hasHeightForWidth());
        PanoLabel->setSizePolicy(sizePolicy);
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::Midlight, brush);
        palette1.setBrush(QPalette::Active, QPalette::AlternateBase, brush);
        QBrush brush2(QColor(227, 227, 227, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Inactive, QPalette::Midlight, brush2);
        QBrush brush3(QColor(245, 245, 245, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Midlight, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush3);
        PanoLabel->setPalette(palette1);
        PanoLabel->setAutoFillBackground(false);
        PanoLabel->setStyleSheet(QString::fromUtf8("QLabel\n"
"{\n"
"    border: 4px solid black;\n"
"    border-radius: 5px;\n"
"}"));
        PanoLabel->setFrameShape(QFrame::StyledPanel);
        PanoLabel->setFrameShadow(QFrame::Plain);
        PanoLabel->setAlignment(Qt::AlignCenter);
        CephLabel = new QLabel(RawGroupBox);
        CephLabel->setObjectName("CephLabel");
        CephLabel->setGeometry(QRect(150, 20, 100, 740));
        sizePolicy.setHeightForWidth(CephLabel->sizePolicy().hasHeightForWidth());
        CephLabel->setSizePolicy(sizePolicy);
        CephLabel->setAutoFillBackground(false);
        CephLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    border: 4px solid black;\n"
"    border-radius: 5px;\n"
"}"));
        CephLabel->setFrameShape(QFrame::StyledPanel);
        PanoProgressBar = new QProgressBar(RawGroupBox);
        PanoProgressBar->setObjectName("PanoProgressBar");
        PanoProgressBar->setGeometry(QRect(10, 20, 20, 740));
        sizePolicy.setHeightForWidth(PanoProgressBar->sizePolicy().hasHeightForWidth());
        PanoProgressBar->setSizePolicy(sizePolicy);
        PanoProgressBar->setLayoutDirection(Qt::LeftToRight);
        PanoProgressBar->setStyleSheet(QString::fromUtf8("\n"
"\n"
"QProgressBar::chunk {\n"
"    background-color:#FF0000;\n"
"	height : 4px;\n"
"    margin: 0.5px;\n"
"}\n"
"\n"
"QProgressBar {\n"
"    border: 2px solid gray;\n"
"    border-radius: 5px;\n"
"}"));
        PanoProgressBar->setMaximum(1750);
        PanoProgressBar->setValue(0);
        PanoProgressBar->setAlignment(Qt::AlignCenter);
        PanoProgressBar->setTextVisible(false);
        PanoProgressBar->setOrientation(Qt::Vertical);
        PanoProgressBar->setInvertedAppearance(false);
        PanoProgressBar->setTextDirection(QProgressBar::TopToBottom);
        CephProgressBar = new QProgressBar(RawGroupBox);
        CephProgressBar->setObjectName("CephProgressBar");
        CephProgressBar->setGeometry(QRect(260, 20, 20, 740));
        sizePolicy.setHeightForWidth(CephProgressBar->sizePolicy().hasHeightForWidth());
        CephProgressBar->setSizePolicy(sizePolicy);
        CephProgressBar->setStyleSheet(QString::fromUtf8("\n"
"QProgressBar::chunk {\n"
"    background-color:#0000FF;\n"
"	height : 4px;\n"
"    margin: 0.5px;\n"
"}\n"
"QProgressBar {\n"
"    border: 2px solid gray;\n"
"    border-radius: 5px;\n"
"}\n"
""));
        CephProgressBar->setMaximum(1250);
        CephProgressBar->setValue(0);
        CephProgressBar->setAlignment(Qt::AlignCenter);
        CephProgressBar->setTextVisible(false);
        CephProgressBar->setOrientation(Qt::Vertical);

        RAWHorizontalLayout->addWidget(RawGroupBox);

        gridLayoutWidget = new QWidget(centralwidget);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(600, 0, 651, 771));
        VTKLayout = new QGridLayout(gridLayoutWidget);
        VTKLayout->setObjectName("VTKLayout");
        VTKLayout->setContentsMargins(0, 0, 0, 0);
        VTKGroupBox = new QGroupBox(gridLayoutWidget);
        VTKGroupBox->setObjectName("VTKGroupBox");
        sizePolicy.setHeightForWidth(VTKGroupBox->sizePolicy().hasHeightForWidth());
        VTKGroupBox->setSizePolicy(sizePolicy);
        openGLWidget_All = new QVTKOpenGLStereoWidget(VTKGroupBox);
        openGLWidget_All->setObjectName("openGLWidget_All");
        openGLWidget_All->setGeometry(QRect(10, 10, 630, 400));
        sizePolicy.setHeightForWidth(openGLWidget_All->sizePolicy().hasHeightForWidth());
        openGLWidget_All->setSizePolicy(sizePolicy);
        openGLWidget_Main = new QVTKOpenGLStereoWidget(VTKGroupBox);
        openGLWidget_Main->setObjectName("openGLWidget_Main");
        openGLWidget_Main->setGeometry(QRect(10, 420, 310, 340));
        sizePolicy.setHeightForWidth(openGLWidget_Main->sizePolicy().hasHeightForWidth());
        openGLWidget_Main->setSizePolicy(sizePolicy);
        openGLWidget_Sub = new QVTKOpenGLStereoWidget(VTKGroupBox);
        openGLWidget_Sub->setObjectName("openGLWidget_Sub");
        openGLWidget_Sub->setGeometry(QRect(330, 420, 310, 340));
        sizePolicy.setHeightForWidth(openGLWidget_Sub->sizePolicy().hasHeightForWidth());
        openGLWidget_Sub->setSizePolicy(sizePolicy);

        VTKLayout->addWidget(VTKGroupBox, 0, 0, 1, 1);

        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(10, 140, 301, 631));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        tableWidget = new QTableWidget(widget);
        if (tableWidget->columnCount() < 3)
            tableWidget->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        if (tableWidget->rowCount() < 10)
            tableWidget->setRowCount(10);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(1, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(2, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(3, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(4, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(5, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(6, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(7, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(8, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(9, __qtablewidgetitem12);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setFrameShape(QFrame::Box);
        tableWidget->setFrameShadow(QFrame::Plain);
        tableWidget->setLineWidth(3);

        gridLayout->addWidget(tableWidget, 1, 0, 1, 1);

        tableWidget_2 = new QTableWidget(widget);
        if (tableWidget_2->columnCount() < 3)
            tableWidget_2->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(0, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(1, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(2, __qtablewidgetitem15);
        if (tableWidget_2->rowCount() < 10)
            tableWidget_2->setRowCount(10);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        tableWidget_2->setVerticalHeaderItem(0, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        tableWidget_2->setVerticalHeaderItem(1, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        tableWidget_2->setVerticalHeaderItem(2, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        tableWidget_2->setVerticalHeaderItem(3, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        tableWidget_2->setVerticalHeaderItem(4, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        tableWidget_2->setVerticalHeaderItem(5, __qtablewidgetitem21);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        tableWidget_2->setVerticalHeaderItem(6, __qtablewidgetitem22);
        QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
        tableWidget_2->setVerticalHeaderItem(7, __qtablewidgetitem23);
        QTableWidgetItem *__qtablewidgetitem24 = new QTableWidgetItem();
        tableWidget_2->setVerticalHeaderItem(8, __qtablewidgetitem24);
        QTableWidgetItem *__qtablewidgetitem25 = new QTableWidgetItem();
        tableWidget_2->setVerticalHeaderItem(9, __qtablewidgetitem25);
        tableWidget_2->setObjectName("tableWidget_2");
        tableWidget_2->setFrameShape(QFrame::Box);
        tableWidget_2->setFrameShadow(QFrame::Plain);
        tableWidget_2->setLineWidth(3);

        gridLayout->addWidget(tableWidget_2, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1248, 22));
        menuCBCT_Simulator = new QMenu(menubar);
        menuCBCT_Simulator->setObjectName("menuCBCT_Simulator");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::Dark, brush);
        QBrush brush4(QColor(160, 160, 160, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::Dark, brush);
        statusbar->setPalette(palette2);
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuCBCT_Simulator->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        PanoCheckBox->setText(QCoreApplication::translate("MainWindow", "Pano Ready", nullptr));
        CephCheckBox->setText(QCoreApplication::translate("MainWindow", "Ceph Ready", nullptr));
        ControlGroupBox->setTitle(QString());
        CaptureResetPushButton->setText(QCoreApplication::translate("MainWindow", "RESET", nullptr));
        CaptureReadyPushButton->setText(QCoreApplication::translate("MainWindow", "READY", nullptr));
        CaptureStartPushButton->setText(QCoreApplication::translate("MainWindow", "START", nullptr));
        CaptureStopPushButton->setText(QCoreApplication::translate("MainWindow", "STOP", nullptr));
        CBCTGroupBox->setTitle(QString());
        SubPushButton->setText(QCoreApplication::translate("MainWindow", "SUB", nullptr));
        MainPushButton->setText(QCoreApplication::translate("MainWindow", "MAIN", nullptr));
        AscendingPushButton->setText(QCoreApplication::translate("MainWindow", "ASCENT", nullptr));
        DescendingPushButton->setText(QCoreApplication::translate("MainWindow", "DESCENT", nullptr));
        RawGroupBox->setTitle(QString());
        CephTextLabel->setText(QCoreApplication::translate("MainWindow", "Ceph RAW", nullptr));
        PanoTextLabel->setText(QCoreApplication::translate("MainWindow", "Pano RAW(90\302\260)", nullptr));
        PanoLabel->setText(QString());
        CephLabel->setText(QString());
        VTKGroupBox->setTitle(QString());
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "\354\235\264\353\257\270\354\247\200 \353\262\210\355\230\270", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "\353\202\240\354\247\234", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "\354\213\234\352\260\204", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->verticalHeaderItem(0);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->verticalHeaderItem(1);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "2", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->verticalHeaderItem(2);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "3", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->verticalHeaderItem(3);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "4", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget->verticalHeaderItem(4);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "5", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget->verticalHeaderItem(5);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "6", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget->verticalHeaderItem(6);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("MainWindow", "7", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget->verticalHeaderItem(7);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("MainWindow", "8", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tableWidget->verticalHeaderItem(8);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("MainWindow", "9", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = tableWidget->verticalHeaderItem(9);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("MainWindow", "10", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = tableWidget_2->horizontalHeaderItem(0);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("MainWindow", "\355\206\265\354\213\240 \353\241\234\352\267\270", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = tableWidget_2->horizontalHeaderItem(1);
        ___qtablewidgetitem14->setText(QCoreApplication::translate("MainWindow", "\353\202\240\354\247\234", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = tableWidget_2->horizontalHeaderItem(2);
        ___qtablewidgetitem15->setText(QCoreApplication::translate("MainWindow", "\354\213\234\352\260\204", nullptr));
        QTableWidgetItem *___qtablewidgetitem16 = tableWidget_2->verticalHeaderItem(0);
        ___qtablewidgetitem16->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        QTableWidgetItem *___qtablewidgetitem17 = tableWidget_2->verticalHeaderItem(1);
        ___qtablewidgetitem17->setText(QCoreApplication::translate("MainWindow", "2", nullptr));
        QTableWidgetItem *___qtablewidgetitem18 = tableWidget_2->verticalHeaderItem(2);
        ___qtablewidgetitem18->setText(QCoreApplication::translate("MainWindow", "3", nullptr));
        QTableWidgetItem *___qtablewidgetitem19 = tableWidget_2->verticalHeaderItem(3);
        ___qtablewidgetitem19->setText(QCoreApplication::translate("MainWindow", "4", nullptr));
        QTableWidgetItem *___qtablewidgetitem20 = tableWidget_2->verticalHeaderItem(4);
        ___qtablewidgetitem20->setText(QCoreApplication::translate("MainWindow", "5", nullptr));
        QTableWidgetItem *___qtablewidgetitem21 = tableWidget_2->verticalHeaderItem(5);
        ___qtablewidgetitem21->setText(QCoreApplication::translate("MainWindow", "6", nullptr));
        QTableWidgetItem *___qtablewidgetitem22 = tableWidget_2->verticalHeaderItem(6);
        ___qtablewidgetitem22->setText(QCoreApplication::translate("MainWindow", "7", nullptr));
        QTableWidgetItem *___qtablewidgetitem23 = tableWidget_2->verticalHeaderItem(7);
        ___qtablewidgetitem23->setText(QCoreApplication::translate("MainWindow", "8", nullptr));
        QTableWidgetItem *___qtablewidgetitem24 = tableWidget_2->verticalHeaderItem(8);
        ___qtablewidgetitem24->setText(QCoreApplication::translate("MainWindow", "9", nullptr));
        QTableWidgetItem *___qtablewidgetitem25 = tableWidget_2->verticalHeaderItem(9);
        ___qtablewidgetitem25->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        menuCBCT_Simulator->setTitle(QCoreApplication::translate("MainWindow", "CBCT Simulator", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
