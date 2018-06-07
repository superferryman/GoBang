#include <utility>
#include <stdlib.h>
#include <time.h>
#include <QDebug>
#include <algorithm>
#include "GameModel.h"

GameModel::GameModel()
{

}

void GameModel::startGame(GameType type)
{
    gameType = type;
    // 初始棋盘
    gameMapVec.clear();
    for (int i = 0; i < kBoardSizeNum; i++)
    {
        std::vector<int> lineBoard;
        for (int j = 0; j < kBoardSizeNum; j++)
            lineBoard.push_back(0);
        gameMapVec.push_back(lineBoard);
    }

    // 如果是AI模式，需要初始化评分数组
    if (gameType == BOT)
    {
        scoreMapVec.clear();
        for (int i = 0; i < kBoardSizeNum; i++)
        {
            std::vector<int> lineScores;
            for (int j = 0; j < kBoardSizeNum; j++)
                lineScores.push_back(0);
            scoreMapVec.push_back(lineScores);
        }
    }

    // 己方下为true,对方下位false
    playerFlag = true;
    who = 1;
}

void GameModel::updateGameMap(int row, int col)
{
    if (playerFlag)
        gameMapVec[row][col] = 1;
    else
        gameMapVec[row][col] = -1;

    // 换手
    playerFlag = !playerFlag;
    who = -who;
}

void GameModel::actionByPerson(int row, int col)
{
    updateGameMap(row, col);
}

void GameModel::actionByAI(int &clickRow, int &clickCol)
{
    qDebug() << method << endl;
    if(method == 1)
    {
        calculateScore();
        QPoint point;
        point = MaxMin(7);
        clickCol = point.y();
        clickRow = point.x();
    }
    if(method == 2)
    {
        // 计算评分
        calculateScore();

        // 从评分中找出最大分数的位置
        int maxScore = 0;
        std::vector<std::pair<int, int>> maxPoints;

        for (int row = 1; row < kBoardSizeNum; row++)
            for (int col = 1; col < kBoardSizeNum; col++)
            {
                // 前提是这个坐标是空的
                if (gameMapVec[row][col] == 0)
                {
                    if (scoreMapVec[row][col] > maxScore)          // 找最大的数和坐标
                    {
                        maxPoints.clear();
                        maxScore = scoreMapVec[row][col];
                        maxPoints.push_back(std::make_pair(row, col));
                    }
                    else if (scoreMapVec[row][col] == maxScore)     // 如果有多个最大的数，都存起来
                        maxPoints.push_back(std::make_pair(row, col));
                }
            }

        // 随机落子，如果有多个点的话
        srand((unsigned)time(0));
        int index = rand() % maxPoints.size();

        std::pair<int, int> pointPair = maxPoints.at(index);
        clickRow = pointPair.first; // 记录落子点
        clickCol = pointPair.second;
    }
    updateGameMap(clickRow, clickCol);
}

// 最关键的计算评分函数
void GameModel::calculateScore()
{
    // 统计玩家或者电脑连成的子
    int personNum = 0; // 玩家连成子的个数
    int botNum = 0; // AI连成子的个数
    int emptyNum = 0; // 各方向空白位的个数

    // 清空评分数组
    scoreMapVec.clear();
    for (int i = 0; i < kBoardSizeNum; i++)
    {
        std::vector<int> lineScores;
        for (int j = 0; j < kBoardSizeNum; j++)
            lineScores.push_back(0);
        scoreMapVec.push_back(lineScores);
    }

    // 计分（此处是完全遍历，其实可以用bfs或者dfs加减枝降低复杂度，通过调整权重值，调整AI智能程度以及攻守风格）
    for (int row = 0; row < kBoardSizeNum; row++)
        for (int col = 0; col < kBoardSizeNum; col++)
        {
            // 空白点就算
            if (row > 0 && col > 0 &&
                gameMapVec[row][col] == 0)
            {
                // 遍历周围八个方向
                for (int y = -1; y <= 1; y++)
                    for (int x = -1; x <= 1; x++)
                    {
                        // 重置
                        personNum = 0;
                        botNum = 0;
                        emptyNum = 0;

                        // 原坐标不算
                        if (!(y == 0 && x == 0))
                        {
                            // 每个方向延伸4个子

                            // 对玩家白子评分（正反两个方向）
                            for (int i = 1; i <= 4; i++)
                            {
                                if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                    col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                    gameMapVec[row + i * y][col + i * x] == 1) // 玩家的子
                                {
                                    personNum++;
                                }
                                else if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                         col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                         gameMapVec[row + i * y][col + i * x] == 0) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            for (int i = 1; i <= 4; i++)
                            {
                                if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                    col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                    gameMapVec[row - i * y][col - i * x] == 1) // 玩家的子
                                {
                                    personNum++;
                                }
                                else if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                         col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                         gameMapVec[row - i * y][col - i * x] == 0) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            if (personNum == 1)                      // 杀二
                                scoreMapVec[row][col] += 10;
                            else if (personNum == 2)                 // 杀三
                            {
                                if (emptyNum == 1)
                                    scoreMapVec[row][col] += 30;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 40;
                            }
                            else if (personNum == 3)                 // 杀四
                            {
                                // 量变空位不一样，优先级不一样
                                if (emptyNum == 1)
                                    scoreMapVec[row][col] += 60;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 110;
                            }
                            else if (personNum == 4)                 // 杀五
                                scoreMapVec[row][col] += 10000;

                            // 进行一次清空
                            emptyNum = 0;

                            // 对AI黑子评分
                            for (int i = 1; i <= 4; i++)
                            {
                                if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                    col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                    gameMapVec[row + i * y][col + i * x] == -1) // AI的子
                                {
                                    botNum++;
                                }
                                else if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                         col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                         gameMapVec[row +i * y][col + i * x] == 0) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            for (int i = 1; i <= 4; i++)
                            {
                                if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                    col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                    gameMapVec[row - i * y][col - i * x] == -1) // AI的子
                                {
                                    botNum++;
                                }
                                else if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                         col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                         gameMapVec[row - i * y][col - i * x] == 0) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            if (botNum == 0)                      // 普通下子
                                scoreMapVec[row][col] += 5;
                            else if (botNum == 1)                 // 活二
                                scoreMapVec[row][col] += 10;
                            else if (botNum == 2)
                            {
                                if (emptyNum == 1)                // 死三
                                    scoreMapVec[row][col] += 25;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 50;  // 活三
                            }
                            else if (botNum == 3)
                            {
                                if (emptyNum == 1)                // 死四
                                    scoreMapVec[row][col] += 55;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 120; // 活四
                            }
                            else if (botNum >= 4)
                                scoreMapVec[row][col] += 10500;   // 活五

                        }
                    }

            }
        }
}

bool GameModel::isWin(int row, int col)
{
    // 横竖斜四种大情况，每种情况都根据当前落子往后遍历5个棋子，有一种符合就算赢
    // 水平方向
    for (int i = 0; i < 5; i++)
    {
        // 往左5个，往右匹配4个子，20种情况
        if (col - i > 0 &&
            col - i + 4 < kBoardSizeNum &&
            gameMapVec[row][col - i] == gameMapVec[row][col - i + 1] &&
            gameMapVec[row][col - i] == gameMapVec[row][col - i + 2] &&
            gameMapVec[row][col - i] == gameMapVec[row][col - i + 3] &&
            gameMapVec[row][col - i] == gameMapVec[row][col - i + 4])
            return true;
    }

    // 竖直方向(上下延伸4个)
    for (int i = 0; i < 5; i++)
    {
        if (row - i > 0 &&
            row - i + 4 < kBoardSizeNum &&
            gameMapVec[row - i][col] == gameMapVec[row - i + 1][col] &&
            gameMapVec[row - i][col] == gameMapVec[row - i + 2][col] &&
            gameMapVec[row - i][col] == gameMapVec[row - i + 3][col] &&
            gameMapVec[row - i][col] == gameMapVec[row - i + 4][col])
            return true;
    }

    // 左斜方向
    for (int i = 0; i < 5; i++)
    {
        if (row + i < kBoardSizeNum &&
            row + i - 4 > 0 &&
            col - i > 0 &&
            col - i + 4 < kBoardSizeNum &&
            gameMapVec[row + i][col - i] == gameMapVec[row + i - 1][col - i + 1] &&
            gameMapVec[row + i][col - i] == gameMapVec[row + i - 2][col - i + 2] &&
            gameMapVec[row + i][col - i] == gameMapVec[row + i - 3][col - i + 3] &&
            gameMapVec[row + i][col - i] == gameMapVec[row + i - 4][col - i + 4])
            return true;
    }

    // 右斜方向
    for (int i = 0; i < 5; i++)
    {
        if (row - i > 0 &&
            row - i + 4 < kBoardSizeNum &&
            col - i > 0 &&
            col - i + 4 < kBoardSizeNum &&
            gameMapVec[row - i][col - i] == gameMapVec[row - i + 1][col - i + 1] &&
            gameMapVec[row - i][col - i] == gameMapVec[row - i + 2][col - i + 2] &&
            gameMapVec[row - i][col - i] == gameMapVec[row - i + 3][col - i + 3] &&
            gameMapVec[row - i][col - i] == gameMapVec[row - i + 4][col - i + 4])
            return true;
    }

    return false;
}

bool GameModel::isDeadGame()
{
    // 所有空格全部填满
    for (int i = 1; i < kBoardSizeNum; i++)
        for (int j = 1; j < kBoardSizeNum; j++)
        {
            if (!(gameMapVec[i][j] == 1 || gameMapVec[i][j] == -1))
                return false;
        }
    return true;
}



bool GameModel::hasNeighbor(int row, int col)
{
    for(int i = -1; i < 2; ++i)
        {
            for(int j = -1; j < 2; ++j)
            {
                if(!i && !j)
                {
                    continue;
                }
                if(row+i > 0 && row+i < kBoardSizeNum && col+j > 0 && col+j < kBoardSizeNum && gameMapVec[row+i][col+j] != 0)
                {
                    return true;
                    break;
                }
            }
        }
        return false;
}

//bool GameModel::hasNeighbor(int row,int col)  // 距离为2之内返回true
//{
//    bool flag = false;
//    int redirect[8][2] = {{1,0},{1,1},{1,-1},{0,1},{0,-1},{-1,0},{-1,1},{-1,-1}};
//    for(int j = 1; j <= 2; ++j)
//    {
//        for(int i = 0; i < 8; ++i)
//        {
//            int m = j * redirect[i][0] + row;
//            int n = j * redirect[i][1] + col;
//            if(m > 0 && m < kBoardSizeNum && n > 0 && n < kBoardSizeNum && gameMapVec[m][n])
//            {
//                flag = true;
//                break;
//            }
//        }
//    }
//    return flag;
//}

//QVector<QPoint> GameModel::GenerateMove()
//{
//    calculateScore();

//    QVector<node *> result;
//    int cnt = 1;

//    for (int row = 1; row < kBoardSizeNum; row++)
//    {
//        for (int col = 1; col < kBoardSizeNum; col++)
//        {
//            // 前提是这个坐标是空的
//            if (gameMapVec[row][col] == 0 && hasNeighbor(row,col))
//            {
//                node * snode = new node;
//                snode->point = QPoint(row,col);
//                snode->sscore = gameMapVec[row][col];
//                result[cnt++] = snode;
//            }
//        }
//    }
//    QVector<QPoint> temp;
//    for(int i = 1; i < cnt; ++i)
//    {
//        temp.push_back(result[i]->point);
//    }
//    for(int i = 0; i < cnt; ++i)// can not ignore this or cause a memory problem
//    {
//        free(result[i]);
//    }
//    return temp;
//}

QVector<QPoint> GameModel::GenerateMove()// 生成可能的位置
{
    calculateScore();
    int top = -1;
    bool first = true;
    QVector<node * > result(12);
    for(int x = 1; x < kBoardSizeNum; ++x)
    {
        for(int y = 1; y < kBoardSizeNum; ++y)
        {
           if(gameMapVec[x][y] == 0)// 有邻居且为空
           {
               if(hasNeighbor(x,y))
               {
                    bool flag = true;
                    for(int i = 0; i <= top; ++i)
                    {
                        if(result[i]->point.x() == x && result[i]->point.y() == y)
                        {
                            flag = !flag;
                            break;
                        }
                    }
                    if(!flag)
                    {
                        continue;
                    }
                    node *snode = new node;
                    snode->point = QPoint(x,y);
                    int tmpscore = snode->sscore = evaluateScore(QPoint(x,y),who);// ~~calculate(x,y) --> gameScoreVec[x][y]~~
                    if(first)
                    {
                        result[0] = snode;
                        first = !first;
                        top = 0;
                    }
                    else // 排序
                    {
                        if(top == 9 && tmpscore < result[9]->sscore)
                        {
                            continue;
                        }
                        int tmp = top;
                        while(tmp >= 0 && result[tmp]->sscore < tmpscore)
                        {
                            result[tmp+1] = result[tmp];
                            --tmp;
                        }
                        result[tmp+1] = snode;
                        ++top;
                        if(top > 9)
                        {
                            top = 9;
                        }
                    }
               }
            }
        }
    }
//    qDebug()<<"emmmmm:"<< result[top]->sscore << result[top]->point.x() << result[top]->point.y() <<endl;
    QVector<QPoint> temp;
    for(int i = 0 ; i < 10; ++i)
    {
        if(i <= top)
        temp.push_back(result[i]->point);
    }
    for(int i = 0 ; i <= top; ++i)
    {
        free(result[i]);
    }
    return temp;
}

//QPoint GameModel::MaxMin(int depth)
//{
//    int best = INT_MIN;
//    QVector<QPoint> points = GenerateMove();
//    for(int i = 0; i < points.count(); ++i)
//    {
//        gameMapVec[points[i].x()][points[i].y()] = who;
//        int val = Min(depth-1,best > INT_MIN ? best : INT_MIN,INT_MAX,points[i]);
//        if(val == best)
//        {
//            bestpoints.push_back(points[i]);
//        }
//        if(val > best)
//        {
//            best = val;
//            bestpoints.clear();
//            bestpoints.push_back(points[i]);
//        }
//        gameMapVec[points[i].x()][points[i].y()] = 0;// 撤销
//    }
//    srand((unsigned)time(NULL));
//    points.clear();
//    return bestpoints[rand()%bestpoints.count()];
//}

//int GameModel::Max(int depth,int alpha,int beta,QPoint point)
//{
//    if(depth <= 0 || isWin(point.x(),point.y()))
//    {
//        int tmp = evaluate(point.x(),point.y(),who);
//        return tmp;
//    }
//    int best = INT_MIN;
//    QVector<QPoint> points = GenerateMove();
//    for(int i = 0; i < points.count(); ++i)
//    {
//        gameMapVec[points[i].x()][points[i].y()] = who;
//        int val = Min(depth-1,best > alpha ? best : alpha, beta,points[i]);
//        if(val > best)
//        {
//            best = val;
//        }
//        gameMapVec[points[i].x()][points[i].y()] = 0;
//        if(val > beta)
//        {
//            break;
//        }
//    }
//    points.clear();
//    return best;
//}

//int GameModel::Min(int depth,int alpha,int beta,QPoint point)
//{
//    if(depth <= 0 || isWin(point.x(),point.y()))
//    {
//        int tmp = evaluate(point.x(),point.y(),who);
//        return tmp;
//    }
//    int best = INT_MAX;
//    QVector<QPoint> points = GenerateMove();
//    for(int i = 0; i < points.count(); ++i)
//    {
//        gameMapVec[points[i].x()][points[i].y()] = -who;
//        int val = Max(depth-1,alpha,best < beta ? best : beta,points[i]);
//        if(val < best)
//        {
//            best = val;
//        }
//        gameMapVec[points[i].x()][points[i].y()] = 0;
//        if(val < alpha)
//        {
//            break;
//        }
//    }
//    points.clear();
//    return best;
//}

//int AlphaBeta(int depth, int alpha, int beta) {
// BOOL fFoundPv = FALSE;
// if (depth == 0) {
//  return Evaluate();
// }
// GenerateLegalMoves();
// while (MovesLeft()) {
//  MakeNextMove();
//  if (fFoundPv) {
//   val = -AlphaBeta(depth - 1, -alpha - 1, -alpha);
//   if ((val > alpha) && (val < beta)) { // 检查失败
//    val = -AlphaBeta(depth - 1, -beta, -alpha);
//   }
//  } else
//   val = -AlphaBeta(depth - 1, -beta, -alpha);
//  }
//  UnmakeMove();
//  if (val >= beta) {
//   return beta;
//  }
//  if (val > alpha) {
//   alpha = val;
//   fFoundPv = TRUE;
//  }
// }
// return alpha;
//}

int GameModel::NegaMax(int depth,int alpha,int beta,QPoint point,int who)
{
    if(depth <= 0 || isWin(point.x(),point.y()))
    {
        return evaluate(point.x(),point.y(),who);
    }

    QVector<QPoint> points = GenerateMove();

    for(int i = 0; i < points.count(); ++i)
    {
        gameMapVec[points[i].x()][points[i].y()] = who;
        int val = -NegaMax(depth-1,-beta,-alpha,points[i],-who);
        gameMapVec[points[i].x()][points[i].y()] = 0;
        if(val >= beta)
        {
            return beta;
        }
        if(val > alpha)
        {
            alpha = val;
        }
    }
    QVector<QPoint>().swap(points);// 听说这样就可以防止内存泄漏？？？QT的内存泄漏有点奇怪  有空要好好研究
    Q_ASSERT(points.capacity() == 0);
    return alpha;
}

QPoint GameModel::MaxMin(int depth)
{
    int alpha = -INT_MAX;
    int beta = INT_MAX;
    QVector<QPoint> points = GenerateMove();
    for(int i = 0; i < points.count(); ++i)
    {
        gameMapVec[points[i].x()][points[i].y()] = who;
        int val = -NegaMax(depth-1,-beta,-alpha,points[i],who);
        if(val > alpha)
        {
            alpha = val;
            resultbestpoint = points[i];
        }
        gameMapVec[points[i].x()][points[i].y()] = 0;
    }
    QVector<QPoint>().swap(points);
    Q_ASSERT(points.capacity() == 0);
//    qDebug()<<"resultbestpoint"<<resultbestpoint.x()<<" "<<resultbestpoint.y()<<endl;
    return resultbestpoint;
}

int GameModel::evaluate(int row,int col,int who)
{
    if(isDeadGame())
        return 0;
    else if(isWin(row, col) && who == 1)
    {
        if(gameMapVec[row][col] == 1)
        {
            return INF;
        }
        if(gameMapVec[row][col] == -1)
        {
            return -INF;
        }
    }
    else if(isWin(row, col) && who == -1)
    {
        if(gameMapVec[row][col] == -1)
        {
            return INF;
        }
        if(gameMapVec[row][col] == 1)
        {
            return -INF;
        }
    }

    return 0;
}

int GameModel::evaluateScore(QPoint point, int who)
{
    int row = point.x();
    int col = point.y();
    int score = 0;

    calculateScore();
    score = scoreMapVec[row][col];
    return score;
}
