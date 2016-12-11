#include "MyForm.h"

MyForm::MyForm (QWidget* parent) : QWidget(parent)
{
  ui.setupUi(this);
}

void MyForm::on_horizontalSlider_valueChanged(int value)
{
    float nextZoom = (float(value)/20);
    if (nextZoom < 0.5) nextZoom = 0.5;
    else if (nextZoom > 5) nextZoom = 5;
    this->ui.widget->zoom = nextZoom;
    this->ui.widget->paintGL();
}

void MyForm::on_widget_zoomMod(int val) //Np
{
    int zoom = this->ui.widget->zoom*20;
    this->ui.horizontalSlider->setValue(zoom);
}
