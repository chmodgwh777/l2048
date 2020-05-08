#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QKeyEvent>
#include <QLinkedList>
#include <QMainWindow>
#include <QMutex>
#include <QParallelAnimationGroup>
#include <QQueue>
//#include <list>
#include <optional>
#include "block.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


/***
 * MainWindow 将Block的 `构造、销毁与动画' `游戏的逻辑' 分开.
 * 所有的 Block 的动画由 block_list (QLinkedlist<Block*>) 管理，
 * 而 square 这个棋盘每个格子是一个 optional，非空的情况下保存一个指向
 * block_list 的迭代器.
 * 每步的操作流程如下：
 *  1. 读取用户输入的方向键，决定 dir
 *  2. 绑定函数指针 get，get可以看做是对索引的抽象，在使用上与 dir 无关
 *  3. 依次遍历每行每列，并为各个格子更改迭代器，为每个 block 设置动画信息
 *  4. 生成新格子并注册动画信息，将其地址 push 到 block_list 中
 *  5. 执行所有动画 （并行）
 *  6. 销毁必要的块并从 block_list 中 erase 掉.
 **/

class MainWindow : public QWidget {
  Q_OBJECT

 public:
  explicit MainWindow(int, QWidget *parent = nullptr);
  ~MainWindow();
  void keyPressEvent(QKeyEvent *) override;

 private:
  enum Direction { Left, Right, Up, Down };
  using PBlock = typename QLinkedList<Block *>::iterator;
  using PBlock_opt = std::optional<PBlock>;

  const int gap = 10;
  const int length = 70;

  //  Ui::MainWindow *ui;

  QParallelAnimationGroup *group;

  const int size;
  Direction dir;

  QMutex _mutex;

  bool changed = false;

  // square 是一维数组
  PBlock_opt *square;
  QLinkedList<Block *> block_list;
  QLinkedList<Block *> new_list;
  QLinkedList<PBlock> remove_list;

  //  QKeyEvent event;

  void loop(Direction);
  void set_dir(Direction);
  void move();
  void display();
  void play();
  void gen();
  void remove();

  PBlock_opt &(MainWindow::*get)(int, int);
  PBlock_opt &_up(int, int);
  PBlock_opt &_down(int, int);
  PBlock_opt &_left(int, int);
  PBlock_opt &_right(int, int);
};
#endif // MAINWINDOW_H
