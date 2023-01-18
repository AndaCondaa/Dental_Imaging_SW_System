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
    MainWindow.show();
    return CTApp.exec();
}
