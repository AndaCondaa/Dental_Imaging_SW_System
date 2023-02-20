#include "editimage.h"
#include "ui_editimage.h"


EditImage::EditImage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditImage)
{

}

EditImage::~EditImage()
{
    delete ui;
}

