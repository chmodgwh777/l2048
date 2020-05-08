#ifndef BLOCK_H
#define BLOCK_H

#include <QLabel>
#include <QMap>
#include <QPropertyAnimation>
#include <QPushButton>

class Block : public QLabel {
  Q_OBJECT;

 public:
  explicit Block(int, QRect, QWidget *parent = nullptr);
  ~Block();

  int getNum() const;
  int getStep() const;

  void setStep(int);
  void setDouble();
  void updateState();

  QPropertyAnimation *moveAnimation = nullptr;

 private:
  static QMap<int, QString> getStyle;
  static QFont font;

  int num = 2;
  int step = 0;
  bool doDouble = false;

  void reDraw();
};

#endif // BLOCK_H
