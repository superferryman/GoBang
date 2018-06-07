#include <QPainter>
#include <QTimer>
#include <QSound>
#include <QMouseEvent>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QColor>
#include <QPalette>
#include <math.h>
#include <QPushButton>
#include <QDesktopWidget>
#include <QApplication>
#include "GameModel.h"
#include "mainwindow.h"

// -------全局遍历-------//
#define CHESS_ONE_SOUND ":/res/sound/chessone.wav"
#define WIN_SOUND ":/res/sound/win.wav"
#define LOSE_SOUND ":/res/sound/lose.wav"

const int kBoardMargin = 30; // 棋盘边缘空隙
const int kRadius = 15; // 棋子半径
const int kMarkSize = 6; // 落子标记边长
const int kBlockSize = 40; // 格子的大小
const int kPosDelta = 20; // 鼠标点击的模糊距离上限
const int kAIDelay = 550; // AI下棋的思考时间

// -------------------- //

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->move((QApplication::desktop()->width()-this->width())/2, (QApplication::desktop()->height()-this->height())/2);
    this->setWindowTitle("五子棋小游戏");
    // 设置棋盘大小
    this->setFixedSize(kBoardMargin * 2 + kBlockSize * kBoardSizeNum, kBoardMargin * 2 + kBlockSize * kBoardSizeNum);
//    setStyleSheet("background-color:yellow;");

    // 开启鼠标hover功能，这两句一般要设置window的
    setMouseTracking(true);
//    centralWidget()->setMouseTracking(true);

    QPalette palette;
    palette.setColor(QPalette::Background, QColor("#B1723C"));
    this->setPalette(palette);

    // 添加菜单
    QMenuBar *mbar = this->menuBar();
    QMenu *gameMenu = new QMenu("Game");
    QAction *actionPVP = new QAction("Person VS Person", this);
    connect(actionPVP, SIGNAL(triggered()), this, SLOT(on_PVP_clicked()));
    gameMenu->addAction(actionPVP);

    QAction *Method_1 = new QAction("Computer 1", this);
    QAction *Method_2 = new QAction("Computer 2", this);
    QMenu *secondMenu = new QMenu("Person VS Computer");
    gameMenu->addMenu(secondMenu);
    connect(Method_1, SIGNAL(triggered()), this, SLOT(choose_method_1()));
    connect(Method_2, SIGNAL(triggered()), this, SLOT(choose_method_2()));
    secondMenu->addAction(Method_1);
    secondMenu->addAction(Method_2);

    QMenu *help = new QMenu("Help");
    QAction *about = new QAction("About Author");
    connect(about, SIGNAL(triggered()), this, SLOT(on_About_clicked()));
    help->addAction(about);

    mbar->addMenu(gameMenu);
    mbar->addMenu(help);

    // 添加按钮
//    exit = new QPushButton(this);
//    exit->setGeometry(QRect(0,0,100,20));
//    exit->setText("Back");
//    connect(exit, SIGNAL(clicked()), this, SLOT(close()));

    // 添加菜单
//    QMenuBar *mainBar = this->menuBar();
//    QMenu *gameMenu = new QMenu("Game"); // menuBar默认是存在的，直接加菜单就可以了
//    QAction *actionPVP = new QAction("Person VS Person", this);
//    connect(actionPVP, SIGNAL(triggered()), this, SLOT(initPVPGame()));
//    gameMenu->addAction(actionPVP);
//    mainBar->addMenu(gameMenu);

//    QAction *actionPVC = new QAction("Person VS Computer", this);
//    connect(actionPVC, SIGNAL(triggered()), this, SLOT(initPVCGame()));
//    gameMenu->addAction(actionPVC);

    // 开始游戏
    initGame();
}

MainWindow::~MainWindow()
{
    if (game)
    {
        delete game;
        game = nullptr;
    }
}

void MainWindow::initGame()
{   
    // 初始化游戏模型
    game = new GameModel;
    initPVPGame();
}

//void MainWindow::initFUNGame()
//{
//    game_type = FUN;
//    game->gameStatus = PLAYING;
//    game->startGame(game_type);
//    update();
//}

void MainWindow::initPVPGame()
{
    game_type = PERSON;
    game->gameStatus = PLAYING;
    game->startGame(game_type);
    update();
}

void MainWindow::initPVCGame(int method)
{
    game_type = BOT;
    game->gameStatus = PLAYING;
    game->method = method;
    game->startGame(game_type);
    update();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    event = event;
    QPainter painter(this);
    // 绘制棋盘
    painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿

    QPen pen; // 调整线条宽度
    pen.setColor(QColor("#8D5822"));
    pen.setWidth(7);
    painter.setPen(pen);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor("#EEC085"));
    painter.setBrush(brush);
    painter.drawRect(kBoardMargin-15,kBoardMargin-15,kBoardMargin * 2 + kBlockSize * kBoardSizeNum-30,kBoardMargin * 2 + kBlockSize * kBoardSizeNum-30);

    pen.setColor(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);
    for(int i = 0; i < kBoardSizeNum + 1; i++)
    {
        painter.drawLine(kBoardMargin + kBlockSize * i, kBoardMargin, kBoardMargin + kBlockSize * i, size().height() - kBoardMargin);
        painter.drawLine(kBoardMargin, kBoardMargin + kBlockSize * i, size().width() - kBoardMargin, kBoardMargin + kBlockSize * i);
    }

//    brush.setColor(Qt::black);
//    painter.setBrush(brush);
//    painter.drawRect(165, 165, 10, 10);
 //   painter.drawRect(485, 165, 10, 10);
 //   painter.drawRect(165, 485, 10, 10);
//    painter.drawRect(485, 485, 10, 10);
//    painter.drawRect(325, 325, 10, 10);

    // 绘制落子标记(防止鼠标出框越界)
    if(clickPosRow > 0 && clickPosRow < kBoardSizeNum &&
        clickPosCol > 0 && clickPosCol < kBoardSizeNum &&
        game->gameMapVec[clickPosRow][clickPosCol] == 0)
    {
        if (game->playerFlag)
            brush.setColor(Qt::white);
        else
            brush.setColor(Qt::black);
        painter.setBrush(brush);
        painter.drawRect(kBoardMargin + kBlockSize * clickPosCol - kMarkSize / 2, kBoardMargin + kBlockSize * clickPosRow - kMarkSize / 2, kMarkSize, kMarkSize);
    }

    // 绘制棋子 
    for (int i = 0; i < kBoardSizeNum; i++)
    {
        for (int j = 0; j < kBoardSizeNum; j++)
        {
            if (game->gameMapVec[i][j] == 1)
            {
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(kBoardMargin + kBlockSize * j - kRadius, kBoardMargin + kBlockSize * i - kRadius, kRadius * 2, kRadius * 2);
//                 painter.setPen(QPen(Qt::red,1,Qt::DashDotLine,Qt::SquareCap));
//                 painter.drawLine(QPoint(50,100),QPoint(300,100));
            }
            else if (game->gameMapVec[i][j] == -1)
            {
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(kBoardMargin + kBlockSize * j - kRadius, kBoardMargin + kBlockSize * i - kRadius, kRadius * 2, kRadius * 2);
            }
        }
    }

    pen.setColor(Qt::red);
    pen.setWidth(1);
    painter.setPen(pen);
    if (clickPosRow > 0 && clickPosRow < kBoardSizeNum &&
        clickPosCol > 0 && clickPosCol < kBoardSizeNum)
    {
        painter.drawLine((clickPosCol+1)*40-30, (clickPosRow+1)*40+10, (clickPosCol+1)*40-30, (clickPosRow+1)*40);
        painter.drawLine((clickPosCol+1)*40-30, (clickPosRow+1)*40+10, (clickPosCol+1)*40-20, (clickPosRow+1)*40+10);
        painter.drawLine((clickPosCol+1)*40+10, (clickPosRow+1)*40, (clickPosCol+1)*40+10, (clickPosRow+1)*40+10);
        painter.drawLine((clickPosCol+1)*40+10, (clickPosRow+1)*40+10, (clickPosCol+1)*40, (clickPosRow+1)*40+10);
        painter.drawLine((clickPosCol+1)*40+10, (clickPosRow+1)*40-30, (clickPosCol+1)*40+10, (clickPosRow+1)*40-20);
        painter.drawLine((clickPosCol+1)*40+10, (clickPosRow+1)*40-30, (clickPosCol+1)*40, (clickPosRow+1)*40-30);
        painter.drawLine((clickPosCol+1)*40-30, (clickPosRow+1)*40-20, (clickPosCol+1)*40-30, (clickPosRow+1)*40-30);
        painter.drawLine((clickPosCol+1)*40-30, (clickPosRow+1)*40-30, (clickPosCol+1)*40-20, (clickPosRow+1)*40-30);
   }

    // 判断输赢
    if (clickPosRow > 0 && clickPosRow < kBoardSizeNum &&
        clickPosCol > 0 && clickPosCol < kBoardSizeNum &&
        (game->gameMapVec[clickPosRow][clickPosCol] == 1 ||
            game->gameMapVec[clickPosRow][clickPosCol] == -1))
    {
        if (game->isWin(clickPosRow, clickPosCol) && game->gameStatus == PLAYING)
        {
            qDebug() << "胜利";
            game->gameStatus = WIN;
            QSound::play(WIN_SOUND);
            QString str;
            if (game->gameMapVec[clickPosRow][clickPosCol] == 1)
                str = "白棋";
            else if (game->gameMapVec[clickPosRow][clickPosCol] == -1)
                str = "黑棋";
            QMessageBox::StandardButton btnValue = QMessageBox::information(this, "恭喜", str + "获胜！");

            // 重置游戏状态，否则容易死循环
            if (btnValue == QMessageBox::Ok)
            {
                game->startGame(game_type);
                game->gameStatus = PLAYING;
            }
        }
    }

    // 判断死局
    if (game->isDeadGame())
    {
        QSound::play(LOSE_SOUND);
        QMessageBox::StandardButton btnValue = QMessageBox::information(this, "oops", "dead game!");
        if (btnValue == QMessageBox::Ok)
        {
            game->startGame(game_type);
            game->gameStatus = PLAYING;
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{   
    // 通过鼠标的hover确定落子的标记
    int x = event->x();
    int y = event->y();

    QPen pen;
    QPainter painter;
    pen.setColor(Qt::red);
    painter.setPen(pen);
    painter.drawLine(20,20,200,200);

    // 棋盘边缘不能落子
    if (x >= kBoardMargin + kBlockSize / 2 &&
            x < size().width() - kBoardMargin &&
            y >= kBoardMargin + kBlockSize / 2 &&
            y < size().height()- kBoardMargin)
    {
        // 获取最近的左上角的点
        int col = x / kBlockSize;
        int row = y / kBlockSize;

        int leftTopPosX = kBoardMargin + kBlockSize * col;
        int leftTopPosY = kBoardMargin + kBlockSize * row;

        // 根据距离算出合适的点击位置,一共四个点，根据半径距离选最近的
        clickPosRow = -1; // 初始化最终的值
        clickPosCol = -1;
        int len = 0; // 计算完后取整就可以了

        // 确定一个误差在范围内的点，且只可能确定一个出来
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < kPosDelta)
        {
            clickPosRow = row;
            clickPosCol = col;
        }
        len = sqrt((x - leftTopPosX - kBlockSize) * (x - leftTopPosX - kBlockSize) + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < kPosDelta)
        {
            clickPosRow = row;
            clickPosCol = col + 1;
        }
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - kBlockSize) * (y - leftTopPosY - kBlockSize));
        if (len < kPosDelta)
        {
            clickPosRow = row + 1;
            clickPosCol = col;
        }
        len = sqrt((x - leftTopPosX - kBlockSize) * (x - leftTopPosX - kBlockSize) + (y - leftTopPosY - kBlockSize) * (y - leftTopPosY - kBlockSize));
        if (len < kPosDelta)
        {
            clickPosRow = row + 1;
            clickPosCol = col + 1;
        }
    }

    // 存了坐标后也要重绘
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    event = event;
    // 人下棋，并且不能抢机器的棋
    if (!(game_type == BOT && !game->playerFlag))
    {
        chessOneByPerson();
        // 如果是人机模式，需要调用AI下棋
        if (game->gameType == BOT && !game->playerFlag)
        {
            // 用定时器做一个延迟
            QTimer::singleShot(kAIDelay, this, SLOT(chessOneByAI()));
        }
    }

}

void MainWindow::chessOneByPerson()
{
    // 根据当前存储的坐标下子
    // 只有有效点击才下子，并且该处没有子
    if (clickPosRow != -1 && clickPosCol != -1 && game->gameMapVec[clickPosRow][clickPosCol] == 0)
    {
        game->actionByPerson(clickPosRow, clickPosCol);
        QSound::play(CHESS_ONE_SOUND);
        // 重绘
        update();
    }
}

void MainWindow::chessOneByAI()
{
    game->actionByAI(clickPosRow, clickPosCol);
    QSound::play(CHESS_ONE_SOUND);
    update();
}

void MainWindow::on_PVP_clicked()
{
    initPVPGame();
}

void MainWindow::on_exit_clicked()
{
    this->close();
}

void MainWindow::on_About_clicked()
{
    about->show();
}

void MainWindow::choose_method_1()
{
    initPVCGame(1);
}

void MainWindow::choose_method_2()
{
    initPVCGame(2);
}
