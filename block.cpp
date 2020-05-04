#include "block.h"
#include <QPropertyAnimation>
#include <QPushButton>

Block::Block(int num, QRect rect, QWidget *parent)
    : QPushButton(parent), moveAnimation(new QPropertyAnimation(this, "pos")),
      num(num) {
  this->setGeometry(rect);
  moveAnimation->setDuration(100);
  moveAnimation->setEasingCurve(QEasingCurve::InOutQuad);
  this->setText(QString("%1").arg(num));
  this->setStyleSheet(QLatin1String("background-color: rgb(164, 211, 255);\n"
                                    "border:none;"));
  this->show();
}

Block::~Block() {}

void Block::setDirection(const QPoint &point) {
  moveAnimation->setStartValue(this->pos());
  moveAnimation->setEndValue(this->pos() + point);
}
