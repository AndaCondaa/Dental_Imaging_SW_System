#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "editimage.h"
#include "imagealbum.h"

#include <QFileDialog>
#include <QColorSpace>
#include <QMessageBox>
#include <QImageReader>
#include <QMdiSubWindow>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_memoAlbum = new ImageAlbum(this);

    QMdiSubWindow *cw = ui->mdiArea->addSubWindow(m_memoAlbum);
    ui->mdiArea->setActiveSubWindow(cw);
    m_memoAlbum->showMaximized();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ButtonClicked()
{
//    QFileDialog dlg;
//    QString filePath = dlg.getOpenFileName(this, "Load Image", "", "Image Files (*.png *.jpg *.bmp)");
//    QString fileName = filePath.section("/", -1);
//    QImage *img = new QImage(filePath);

//    QSize size = img->size();
//    img->scaled(size);

//    QPixmap buf = QPixmap::fromImage(*img);
//    buf = buf.scaled(ui->graphicsView->width(), ui->graphicsView->height());

//    QGraphicsScene* scene = new QGraphicsScene;

//    ui->graphicsView->setScene(scene);
//    scene->addPixmap(buf);
}

