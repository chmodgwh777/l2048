#include "block.h"
#include <QMap>
#include <QPropertyAnimation>
#include <QPushButton>

QMap<int, QString> Block::getStyle = {
    {2, QStringLiteral("color:#000000;background-color:#eee4da;")},
    {4, QStringLiteral("color:#000000;background-color:#ede0c8;")},
    {8, QStringLiteral("color:#f9f6f2;background-color:#f2b179;")},
    {16, QStringLiteral("color:#f9f6f2;background-color:#f59563;")},
    {32, QStringLiteral("color:#f9f6f2;background-color:#f67c5f;")},
    {64, QStringLiteral("color:#f9f6f2;background-color:#f65e3b;")},
    {128, QStringLiteral("color:#f9f6f2;background-color:#edcf72;")},
    {256, QStringLiteral("color:#f9f6f2;background-color:#edcc61;")},
    {512, QStringLiteral("color:#f9f6f2;background-color:#edc850;")},
    {1024, QStringLiteral("color:#f9f6f2;background-color:#edc53f;")},
    {2048, QStringLiteral("color:#f9f6f2;background-color:#edc22e;")},
};

QFont Block::font(QStringLiteral("Courier New"), 30);

Block::Block(int num, QRect rect, QWidget *parent)
    : QPushButton(parent),
      moveAnimation(new QPropertyAnimation(this, "pos")),
      num(num),
      step(0),
      doDouble(false) {
  this->setGeometry(rect);
  moveAnimation->setDuration(100);
  moveAnimation->setEasingCurve(QEasingCurve::InOutQuad);

  this->setFont(font);
  reDraw();
  this->show();
}

Block::~Block() {}

void Block::setStep(int s) { step = s; }

void Block::updateState() {
  step = 0;
  if (doDouble) {
    num *= 2;
    doDouble = false;
    reDraw();
  }
}

void Block::reDraw() {
  this->setText(QString("%1").arg(num));
  this->setStyleSheet(getStyle[num]);
}

int Block::getNum() const { return num; }

int Block::getStep() const { return step; }

void Block::setDouble() { doDouble = true; }
