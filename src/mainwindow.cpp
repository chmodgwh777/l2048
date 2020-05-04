#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "block.h"

#include <QAbstractAnimation>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

#include <ctime>

#include <QDebug>
#include <QTime>

MainWindow::MainWindow(int size, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      group(new QParallelAnimationGroup(this)),
      size(size),
      square(new MainWindow::PBlock_opt[size * size]) {
  ui->setupUi(this);
  qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

  QObject::connect(ui->upBotton, &QPushButton::clicked, [=]() { loop(Up); });
  QObject::connect(ui->downButton, &QPushButton::clicked,
                   [=]() { loop(Down); });
  QObject::connect(ui->leftButton, &QPushButton::clicked,
                   [=]() { loop(Left); });
  QObject::connect(ui->rightButton, &QPushButton::clicked,
                   [=]() { loop(Right); });

  group->setDirection(QAbstractAnimation::Forward);
  group->setLoopCount(1);

  gen();
  gen();

  QObject::connect(group, &QParallelAnimationGroup::finished, [&]() {
    for (auto block : block_list) {
      block->step = 0;
      if (block->times) {
        block->num <<= 1;
        block->times = false;
        block->setText(QString("%1").arg(block->num));
      }
    }
    remove();
    gen();
  });
}

MainWindow::~MainWindow() {
  delete ui;
  delete[] square;
}

void MainWindow::display() {
  QDebug deb = qDebug();
  deb << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if ((square + (4 * i))[j]) {
        //        printf(" *");
        deb << " *";
      } else {
        //        printf(" =");
        deb << " =";
      }
    }
    deb << "\n";
  }
}
bool MainWindow::gen() {
  int r = qrand() % (size * size);
  while (square[r]) {
    r = (r + 1) % (size * size);
  }
  int column = r % size;
  int row = r / size;
  int x = gap + column * (gap + length);
  int y = gap + row * (gap + length);

  auto p = new Block(2, QRect(x, y, length, length), this);

  block_list.push_front(p);

  (square + row * size)[column] = block_list.begin();

  group->addAnimation(p->moveAnimation);

  //  display();
  return true;
}

void MainWindow::remove() {
  //
  for (auto it : remove_list) {
    group->removeAnimation((*it)->moveAnimation);
    delete *it;
    block_list.erase(it);
  }
  remove_list.clear();
}

void MainWindow::move() {
  for (int num = 0; num < size; num++) {
    int i = 0;
    while (!(this->*get)(num, i) && i < size)
      i++;
    if (i == size) // 这列全是0
      continue;

    //    auto &previous = (this->*get)(num, i);
    auto &current = (this->*get)(num, i);

    if (i > 0) {
      // (num, i) 是本列第一个非空格子，将其移动到 (num, 0);
      // (this->*get)(num, 0) == std::nullopt;
      assert(!(this->*get)(num, 0));

      // 为动画设置 step;
      (**current)->step = i;

      //      (this->*get)(num, 0) = current;
      //      current.reset();
      std::swap(current, (this->*get)(num, 0));
    }

    bool can_merge = true;

    int previous_postion = 0;  //前一个块移动后的位置.

    while (++i < size) {
      // (num, i) 没有块
      if (!(this->*get)(num, i)) continue;

      // (num, i) 有块
      auto &current = (this->*get)(num, i);
      auto &previous = (this->*get)(num, previous_postion);
      if ((**current)->num == (**previous)->num && can_merge) {
        // 合并，删除前一个块
        (**previous)->disappear = true;
        remove_list.push_back(*previous);
        previous = {};

        // 将 current 移动到 previous_postion，注册移动动画与翻倍动画.
        (**current)->step = i - previous_postion;
        (**current)->times = true;

        std::swap(current, previous);

        can_merge = false;
      } else {
        // 移动
        previous_postion++;
        (**current)->step = i - previous_postion;

        std::swap(current, (this->*get)(num, previous_postion));

        can_merge = true;
      }
    }
  }
}

void MainWindow::play() {
  /***
   * Play the animation
   ***/

  QPoint coff;
  switch (dir) {
    case Up:
      coff = QPoint(0, -1);
      break;
    case Down:
      coff = QPoint(0, 1);
      break;
    case Left:
      coff = QPoint(-1, 0);
      break;
    case Right:
      coff = QPoint(1, 0);
      break;
    default:
      coff = QPoint(0, 0);
  }

  for (auto block : block_list) {
    block->moveAnimation->setStartValue(block->pos());
    block->moveAnimation->setEndValue(block->pos() +
                                      block->step * (gap + length) * coff);
  }
  group->start();
}

void MainWindow::loop(Direction d) {
  this->set_dir(d);
  move();
  play();
}

void MainWindow::set_dir(Direction dir) {
  this->dir = dir;
  switch (dir) {
    case Left:
      this->get = &MainWindow::_left;
      break;
    case Right:
      this->get = &MainWindow::_right;
      break;
    case Up:
      this->get = &MainWindow::_up;
      break;
    case Down:
      this->get = &MainWindow::_down;
      break;
  }
}

MainWindow::PBlock_opt &MainWindow::_up(int num, int i) {
  return (square + size * i)[num];
}

MainWindow::PBlock_opt &MainWindow::_down(int num, int i) {
  return (square + size * (size - i - 1))[num];
}

MainWindow::PBlock_opt &MainWindow::_left(int num, int i) {
  return (square + size * num)[i];
}

MainWindow::PBlock_opt &MainWindow::_right(int num, int i) {
  return (square + size * num)[size - i - 1];
}
