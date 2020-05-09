#include "maininterface.h"
#include "ui_maininterface.h"

maininterface::maininterface(QWidget *parent)
    : QWidget(parent), ui(new Ui::maininterface) {
  ui->setupUi(this);
}

maininterface::~maininterface()
{
  delete ui;
}
