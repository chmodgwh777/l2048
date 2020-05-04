#ifndef BLOCK_H
#define BLOCK_H

#include <QPropertyAnimation>
#include <QPushButton>

class Block : public QPushButton {
  Q_OBJECT;

public:
  explicit Block(int, QRect, QWidget *parent = nullptr);
  ~Block();

  void setDirection(const QPoint &);

  QPropertyAnimation *moveAnimation = nullptr;

  int num;
  int step;
  bool disappear;
  bool times;
};

#endif // BLOCK_H
