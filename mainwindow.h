#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "GameModel.h"
#include "about.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initPVPGame();
    void initPVCGame(int method);
    void initFUNGame();

protected:
    // 绘制
    void paintEvent(QPaintEvent *event);
    // 监听鼠标移动情况，方便落子
    void mouseMoveEvent(QMouseEvent *event);
    // 实际落子
    void mouseReleaseEvent(QMouseEvent *event);

private:
    ABout *about = new ABout;
    GameModel *game; // 游戏指针
    GameType game_type; // 存储游戏类型
    int clickPosRow, clickPosCol; // 存储将点击的位置
    void initGame();
    void checkGame(int y, int x);
    int curx,cury;
//    QPushButton *exit;

private slots:
    void chessOneByPerson(); // 人执行
    void chessOneByAI(); // AI下棋

private slots:
    void on_PVP_clicked();

    void on_exit_clicked();

    void on_About_clicked();

    void choose_method_1();

    void choose_method_2();


};

#endif // MAINWINDOW_H
