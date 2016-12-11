#include "ui_MyForm.h"
#include <iostream>
class MyForm : public QWidget
{
  Q_OBJECT

  public:
    MyForm (QWidget *parent=0);
  

private slots:

    void on_horizontalSlider_valueChanged(int value);

    void on_widget_zoomMod(int val);

private:
    Ui::MyForm ui;
};
