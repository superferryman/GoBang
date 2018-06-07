#ifndef GAMEMODEL_H
#define GAMEMODEL_H

// ---- 五子棋游戏模型类 ---- //
#include <vector>
#include <QVector>
#include <QPoint>

// 游戏类型，双人还是AI（目前固定让AI下黑子）
enum GameType
{
    PERSON,
    BOT,
    FUN
};

// 游戏状态
enum GameStatus
{
    PLAYING,
    WIN,
    DEAD
};

//
struct node{
    QPoint point;
    int sscore;
};

//typedef struct
//{
//  int x,y,l,r;
//}Value;

// 棋盘尺寸
const int kBoardSizeNum = 15;
const int INF = 99999999;

class GameModel
{
public:
    GameModel();

public:
    std::vector<std::vector<int>> gameMapVec; // 存储当前游戏棋盘和棋子的情况,空白为0，白子1，黑子-1
    std::vector<std::vector<int>> scoreMapVec; // 存储各个点位的评分情况，作为AI下棋依据
    bool playerFlag; // 标示下棋方
    int who;
    int method;
    GameType gameType; // 游戏模式
    GameStatus gameStatus; // 游戏状态
    QPoint resultbestpoint;
    QVector<QPoint> bestpoints;

    void startGame(GameType type); // 开始游戏
    void calculateScore(); // 计算评分
    void actionByPerson(int row, int col); // 人执行下棋
    void actionByAI(int &clickRow, int &clickCol); // 机器执行下棋
    void updateGameMap(int row, int col); // 每次落子后更新游戏棋盘
    bool isWin(int row, int col); // 判断游戏是否胜利
    bool isDeadGame(); // 判断是否和棋
    // 加入极大极小的算法
    int NegaMax(int depth,int alpha,int beta,QPoint point,int who);
    bool hasNeighbor(int row,int col);
    QVector<QPoint> GenerateMove();
    QPoint MaxMin(int depth);
    int Min(int depth,int alpha, int beta,QPoint point);
    int Max(int depth,int alpha, int beta,QPoint point);
    int evaluate(int row,int col,int who);
    int evaluateScore(QPoint point,int who);
//    int chart(int cnt);
//    void setValue();
//    void getValue();
//    int siteValue[kBoardSizeNum][kBoardSizeNum];
//    Value value[kBoardSizeNum][kBoardSizeNum];
};

#endif // GAMEMODEL_H
