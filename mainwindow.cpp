#include "mainwindow.h"
#include "block.h"
//#include "ui_mainwindow.h"

#include <QAbstractAnimation>
//#include <QGridLayout>
#include <QKeyEvent>
#include <QLayout>
#include <QMutex>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

#include <ctime>

#include <QDebug>
#include <QTime>

/***
 * margin: #BBADA0
 * bottom: #CDC1B4
 * */

void MainWindow::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_W:
      loop(Up);
      break;
    case Qt::Key_A:
      loop(Left);
      break;
    case Qt::Key_S:
      loop(Down);
      break;
    case Qt::Key_D:
      loop(Right);
      break;
    case Qt::Key_Up:
      loop(Up);
      break;
    case Qt::Key_Left:
      loop(Left);
      break;
    case Qt::Key_Down:
      loop(Down);
      break;
    case Qt::Key_Right:
      loop(Right);
      break;
    default:;
  }
}

MainWindow::MainWindow(int size, QWidget *parent)
    : QWidget(parent),
      group(new QParallelAnimationGroup(this)),
      size(size),
      changed(false),
      //      m_field(this),
      square(new MainWindow::PBlock_opt[size * size]) {
  auto main_layout = new QVBoxLayout;

  /*********
   * field *
   *********/
  auto width = size * (length + gap) + gap;
  //  m_field->resize(width, width);
  this->setStyleSheet(QStringLiteral("background-color:#BBADA0;"));

  auto grid = new QGridLayout;
  grid->setSpacing(gap);
  grid->setContentsMargins(gap, gap, gap, gap);

  for (auto i = 0; i < size; i++) {
    for (auto j = 0; j < size; j++) {
      auto w = new QWidget();
      w->setStyleSheet(
          QStringLiteral("background-color:#CDC1B4;border-radius:3px;"));
      w->resize(length, length);
      grid->addWidget(w, i, j);
    }
  }

  /*********
   * score *
   *********/
  this->m_score = new QLabel(this);
  m_score->setText(QString("Score:%1").arg(this->score, 6));
  auto font = QFont(QStringLiteral("Courier New"), 40, QFont::Bold);
  m_score->setFont(font);
  m_score->setAlignment(Qt::AlignCenter);
  m_score->resize(width, length);

  main_layout->setSpacing(0);
  main_layout->setMargin(0);
  main_layout->addWidget(m_score);
  main_layout->addLayout(grid);

  main_layout->setStretch(0, length);
  main_layout->setStretch(1, width);

  this->resize(width, length + width);
  this->setLayout(main_layout);

  qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

  group->setDirection(QAbstractAnimation::Forward);
  group->setLoopCount(1);

  gen();
  gen();

  QObject::connect(group, &QParallelAnimationGroup::finished, [&]() {
    for (auto block : block_list) {
      block->updateState();
    }
    remove();
    if (changed) {
      gen();
    }
    changed = false;
    m_score->setText(QString("Score:%1").arg(this->score, 6));
    _mutex.unlock();
  });
}

MainWindow::~MainWindow() {
  //  delete ui;
  //  delete m_field;
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

void MainWindow::gen() {
  if (block_list.count() == size * size) {
    return;
  }
  int r = qrand() % (size * size);
  auto init = r % 2 == 0 ? 2 : 4;
  while (square[r]) {
    r = (r + 1) % (size * size);
  }
  int column = r % size;
  int row = r / size;
  int x = gap + column * (gap + length);
  int y = gap + row * (gap + length);

  auto p = new Block(init, QRect(x, y + length, length, length), this);

  block_list.push_front(p);

  (square + row * size)[column] = block_list.begin();

  group->addAnimation(p->moveAnimation);
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
      // 为动画设置 step;
      (**current)->setStep(i);

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
      if ((**current)->getNum() == (**previous)->getNum() && can_merge) {
        // 合并，删除前一个块
        this->score += (**current)->getNum() * 2;
        remove_list.push_back(*previous);
        previous.reset();

        // 将 current 移动到 previous_postion，注册移动动画与翻倍动画.
        (**current)->setStep(i - previous_postion);
        (**current)->setDouble();

        (**current)->stackUnder(**previous);

        std::swap(current, previous);

        can_merge = false;
      } else {
        // 移动
        previous_postion++;
        (**current)->setStep(i - previous_postion);

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
    if (block->getStep() != 0) {
      changed = true;
    }
    block->moveAnimation->setStartValue(block->pos());
    block->moveAnimation->setEndValue(block->pos() +
                                      block->getStep() * (gap + length) * coff);
  }
  group->start();
}

void MainWindow::loop(Direction d) {
  if (_mutex.tryLock()) {
    this->set_dir(d);
    move();
    play();
  }
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
