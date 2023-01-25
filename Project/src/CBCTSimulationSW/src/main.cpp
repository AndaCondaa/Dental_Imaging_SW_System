#include "mainwindow.h"

#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication CTApp(argc, argv);

   

    //vtkSmartPointer<QVTKOpenGLStereoWidget> StereoAll = vtkSmartPointer<
    //QVTKOpenGLStereoWidget<vtkRenderWindow>->AddRenderer(rendererAll);
    //
    //widget.show();
    
    MainWindow MainWindow;
    MainWindow.setMinimumSize(100, 100);
    MainWindow.resize(1920, 1200);
    MainWindow.showMaximized();
    MainWindow.show();

    return CTApp.exec();
}
