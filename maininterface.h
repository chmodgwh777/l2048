#ifndef MAININTERFACE_H
#define MAININTERFACE_H

#include <QWidget>

namespace Ui {
class maininterface;
}

class maininterface : public QWidget
{
  Q_OBJECT

 public:
  explicit maininterface(QWidget *parent = nullptr);
  ~maininterface();

 private:
  Ui::maininterface *ui;
};

#endif // MAININTERFACE_H
