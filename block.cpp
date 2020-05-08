#include "block.h"
#include <QMap>
#include <QPropertyAnimation>
#include <QPushButton>

QMap<int, QString> Block::getStyle = {
    {2, QStringLiteral("color:#776e65;background-color:#eee4da;border-radius:"
                       "3px;text-align:center;font-size:40px;")},
    {4, QStringLiteral("color:#776e65;background-color:#ede0c8;border-radius:"
                       "3px;text-align:center;font-size:40px;")},
    {8, QStringLiteral("color:#f9f6f2;background-color:#f2b179;border-radius:"
                       "3px;text-align:center;font-size:40px;")},
    {16, QStringLiteral("color:#f9f6f2;background-color:#f59563;border-radius:"
                        "3px;text-align:center;font-size:40px;")},
    {32, QStringLiteral("color:#f9f6f2;background-color:#f67c5f;border-radius:"
                        "3px;text-align:center;font-size:40px;")},
    {64, QStringLiteral("color:#f9f6f2;background-color:#f65e3b;border-radius:"
                        "3px;text-align:center;font-size:40px;")},
    {128, QStringLiteral("color:#f9f6f2;background-color:#edcf72;border-radius:"
                         "3px;text-align:center;font-size:30px;")},
    {256, QStringLiteral("color:#f9f6f2;background-color:#edcc61;border-radius:"
                         "3px;text-align:center;font-size:30px;")},
    {512, QStringLiteral("color:#f9f6f2;background-color:#edc850;border-radius:"
                         "3px;text-align:center;font-size:30px;")},
    {1024, QStringLiteral("color:#f9f6f2;background-color:#edc53f;border-"
                          "radius:3px;text-align:center;font-size:25px;")},
    {2048, QStringLiteral("color:#f9f6f2;background-color:#edc22e;border-"
                          "radius:3px;text-align:center;font-size:25px;")},
};

QFont Block::font(QStringLiteral("Courier New"), -1, QFont::Bold);

Block::Block(int num, QRect rect, QWidget *parent)
    : QLabel(parent),
      moveAnimation(new QPropertyAnimation(this, "pos")),
      num(num),
      step(0),
      doDouble(false) {
  this->setGeometry(rect);
  moveAnimation->setDuration(70);
  moveAnimation->setEasingCurve(QEasingCurve::Linear);

  this->setAlignment(Qt::AlignCenter);
  this->setFont(font);
  this->show();
  reDraw();
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
