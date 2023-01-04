#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QColorSpace>
#include <QMessageBox>
#include <QImageReader>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(ButtonClicked()));
    connect(ui->actionzoomIn, SIGNAL(clicked()), this, SLOT(zoomIn()));
    connect(ui->actionzoomOut, SIGNAL(clicked()), this, SLOT(zoomOut()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ButtonClicked()
{
    QFileDialog dlg;
    QString filePath = dlg.getOpenFileName(this, "Load Image", "", "Image Files (*.png *.jpg *.bmp)");
    QString fileName = filePath.section("/", -1);
    QImage *img = new QImage(filePath);

    QSize size = img->size();
    img->scaled(size);

    QPixmap buf = QPixmap::fromImage(*img);
    buf = buf.scaled(ui->graphicsView->width(), ui->graphicsView->height());

    QGraphicsScene* scene = new QGraphicsScene;

    ui->graphicsView->setScene(scene);
    scene->addPixmap(buf);

}

void MainWindow::zoomIn()
{
//    img.scaled();
}

void MainWindow::wheelEvent(QWheelEvent* event)
{
//    setTransformationAnchor(AnchorUnderMouse);
//    setDragMode(ScrollHandDrag);
//    double scalefactor = 1.5;
//    if(event->delta() > 0)
//    {
//        scale(scalefactor, scalefactor);
//    }

//    else {
//        scale(1/scalefactor, 1/scalefactor);
//    }
}



