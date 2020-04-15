#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>

#define BLACK 0
#define WHITE 1
#define EMPTY 2

struct Point { //点结构
    int x, y;
};
struct Step { //步结构
    Point first, second;
    int value;
};
int Board[19][19];//存储棋盘信息，其元素值为 BLACK, WHITE, EMPTY 之一

/*以下为本人写的函数*/

struct directs {
    //directX表示每个方向有几个相连，liveX表示每个方向是死0、眠1、活2
    int direct[4];
    int live[4];
};

struct point {
    int x;
    int y;
    int mark;
};

//函数声明
void copy(int board1[19][19], int board2[19][19]);
int markslef(int* result);
int markenemy(int *result);

void analyse(int thearray[19], int side, int *result);
int* analyse1(int board[19][19], int side,int g,int h);//x,y为当前棋子位置
void analyse_horizontal(int board[19][19], int*result, int side,int g,int h);//"  —  "
void analyse_vertical(int board[19][19], int*result, int side,int g,int h);//"  丨  "
void analyse_oblique1(int board[19][19], int*result, int side,int g,int h);//"  \  "
void analyse_oblique2(int board[19][19], int*result, int side,int g,int h);//"  /  "
bool surround(int board[19][19], int x, int y, int side);//用来判断该子周围两格内是不是空

bool surround(int board[19][19], int x, int y, int side)//周围有棋子返回ture，全是空白返回false
{
    for (int i = -3; i < 4; i++)
    {
        if (x + i > 18)
            break;
        for (int j = -3; j < 4; j++)
        {
            while (x + i < 0)
                i += 1;
            while (y + j < 0)
                j += 1;
            if (i == 0 & j == 0)
                j += 1;
            if (y + j > 18)
                break;
            if (board[x + i][y + j] == WHITE || board[x + i][y + j] == BLACK)
                return true;
        }
    }
    return false;
}

void analyse_horizontal(int board[19][19], int*result, int side,int g,int h)
{
    int thearray[19];
    for (int l = 0; l < 19; l++)
        thearray[l] = board[g][l];
    analyse(thearray, side, result);
}
void analyse_vertical(int board[19][19], int*result, int side,int g,int h)
{
    int thearray[19];
    for (int l = 0; l < 19; l++)
        thearray[l] = board[l][h];
    analyse(thearray, side, result);
}
void analyse_oblique1(int board[19][19], int*result, int side,int g,int h)//"  \  "
{
    int thearray[19];
    for (int x = 5; x < 19; x++)
    {
        int judge = 0;
        for (int k = 0; k < 19; k++)
            thearray[k] = -1;
        for (int y = 0; y <= x; y++)
        {
            if (g==y&&h==18-x+y)
            {
                judge = 1;
                break;
            }
        }
        if (judge == 1)
        {
            for (int y = 0; y <= x; y++)
                thearray[y] = board[y][18 - x + y];
            analyse(thearray, side, result);
            return;
        }
    }
    for (int x = 1; x < 14; x++)
    {
        int judge = 0;
        for (int k = 0; k < 19; k++)
            thearray[k] = -1;
        for (int y = 0; y <= 18 - x; y++)
        {
            if (g==x+y&&h==y)
            {
                judge = 1;
                break;
            }
        }
        if (judge == 1)
        {
            for (int y = 0; y <= 18 - x; y++)
                thearray[y] = board[x + y][y];
            analyse(thearray, side, result);
            return;
        }
    }
}
void analyse_oblique2(int board[19][19], int*result, int side,int g,int h)//"  /  "
{
    int thearray[19];
    for (int x = 5; x < 19; x++)
    {
        int judge = 0;
        for (int k = 0; k < 19; k++)
            thearray[k] = -1;
        for (int y = 0; y <= x; y++)
        {
            if (g==y&&h==x-y)
            {
                judge = 1;
                break;
            }
        }
        if (judge == 1)
        {
            for (int y = 0; y <= x; y++)
                thearray[y] = board[y][x - y];
            analyse(thearray, side, result);
            return;
        }
    }
    for (int x = 1; x < 14; x++)
    {
        int judge = 0;
        for (int k = 0; k < 19; k++)
            thearray[k] = -1;
        for (int y = 0; y <= 18 - x; y++)
        {
            if (g==x+y&&h==18-y)
            {
                judge = 1;
                break;
            }
        }
        if (judge == 1)
        {
            for (int y = 0; y <= 18 - x; y++)
                thearray[y] = board[x + y][18 - y];
            analyse(thearray, side, result);
            return;
        }
    }
}

void analyse(int thearray[19], int side, int *result)
{
    //1&&2：六连，长连
    int judge_continuous = 0;//用于判断多少个同类棋子连续
    for (int i = 0; i < 19; i++)
    {
        if (thearray[i] == side)
        {
            judge_continuous += 1;
            if (judge_continuous == 6)
            {
                result[0] += 1;
                for (int j = 0; j < 6; j++)
                    thearray[i - j] = 1 - side;
            }
            if (judge_continuous > 6 && ((i < 18 && thearray[i + 1] != side) || i == 18))
            {
                result[1] += 1;
                for (int j = 0; j < judge_continuous; j++)
                {
                    if (thearray[i - j] == side)
                        thearray[i - j] = -1;
                }
            }
        }
        else
            judge_continuous = 0;
    }

    //3：活5
    judge_continuous = 0;//判断值归零
    for (int i = 0; i < 19; i++)
    {
        if (i > 5 && judge_continuous == 5)
        {
            if (thearray[i] == EMPTY && thearray[i - 6] == EMPTY)//两头为空
            {
                result[2] += 1;
                for (int j = 1; j < 6; j++)
                {
                    if (thearray[i - j] == side)
                        thearray[i - j] = -1;
                }
            }
        }
        if (thearray[i] == side)
            judge_continuous += 1;
        else
            judge_continuous = 0;
    }
    //4：眠5②：__OO__OOO__
    judge_continuous = 0;//判断值归零
    for (int i = 0; i < 19; i++)
    {
        int recover_i = i;
        int judgeempty = 0;
        if (thearray[i] == side || thearray[i] == EMPTY)
        {
            if (thearray[i] == EMPTY)
                judgeempty += 1;
            i += 1;
            judge_continuous += 1;
            if (i > 14)
                break;
            int tempi = i + 5;
            for (i; i < tempi; i++)
            {
                if (thearray[i] == side || thearray[i] == EMPTY)
                {
                    judge_continuous += 1;
                    if (thearray[i] == EMPTY)
                        judgeempty += 1;
                }
                else if (thearray[i] == -1)
                {
                    while (i < 18 && thearray[i + 1] != -1)
                        i += 1;
                    judge_continuous = 0;
                    goto here4;
                }
                else
                {
                    i = recover_i;
                    break;
                }
            }
        }
        if (judge_continuous == 6 && judgeempty == 1)
        {
            result[3] += 1;
            for (int j = 1; j < 7; j++)
            {
                if (thearray[i - j] == side)
                    thearray[i - j] = -1;
            }
        }
        else
        {
            judge_continuous = 0;
            i = recover_i;
        }
    here4:      i = i;
    }
    //5：死5
    judge_continuous = 0;//判断值归零
    for (int i = 0; i < 19; i++)
    {
        int recover_i = i;
        int judgeempty = 0;
        if (thearray[i] == 1 - side)
        {
            i += 1;
            if (i > 14)
                break;
            int tempi = i + 5;
            for (i; i < tempi; i++)
            {
                if (thearray[i] == side || thearray[i] == EMPTY)
                    judge_continuous += 1;
                else if (thearray[i] == -1)
                {
                    while (i < 18 && thearray[i + 1] != -1)
                        i += 1;
                    judge_continuous = 0;
                    goto here5;
                }
                else
                {
                    i = recover_i;
                    break;
                }
            }
        }
        if (i < 19 && judge_continuous == 5 && thearray[i] == 1 - side)
        {
            result[4] += 1;
            for (int j = 1; j < 6; j++)
            {
                if (thearray[i - j] == side)
                    thearray[i - j] = -1;
            }
        }
        else
        {
            judge_continuous = 0;
            i = recover_i;
        }
    here5:i = i;
    }
    //6：活4
    judge_continuous = 0;//判断值归零
    for (int i = 0; i < 19; i++)
    {
        if (i > 4 && judge_continuous == 4)
        {
            if (thearray[i] == EMPTY && thearray[i - 5] == EMPTY)//两头为空
            {
                result[5] += 1;//不需要跳出，因为可以同时存在两个活4，接下来继续判断
                for (int j = 1; j < 5; j++)
                {
                    if (thearray[i - j] == side)
                        thearray[i - j] = -1;
                }
            }
        }
        if (thearray[i] == side)
            judge_continuous += 1;
        else
            judge_continuous = 0;
    }
    //7：眠4：__OO____OO__类似
    judge_continuous = 0;//判断值归零
    for (int i = 0; i < 19; i++)
    {

        int recover_i = i;
        int judgeempty = 0;
        if (thearray[i] == side || thearray[i] == EMPTY)
        {
            if (thearray[i] == EMPTY)
                judgeempty += 1;
            i += 1;
            judge_continuous += 1;
            if (i > 14)
                break;
            int tempi = i + 5;
            for (i; i < tempi; i++)
            {
                if (thearray[i] == side || thearray[i] == EMPTY)
                {
                    judge_continuous += 1;
                    if (thearray[i] == EMPTY)
                        judgeempty += 1;
                }
                else if (thearray[i] == -1)
                {
                    while (i < 18 && thearray[i + 1] != -1)
                        i += 1;
                    judge_continuous = 0;
                    goto here7;
                }
                else
                {
                    i = recover_i;
                    break;
                }
            }
        }
        if (judge_continuous == 6 && judgeempty == 2)
        {
            result[6] += 1;
            for (int j = 1; j < 7; j++)
            {
                if (thearray[i - j] == side)
                    thearray[i - j] = -1;
            }
        }
        else
        {
            judge_continuous = 0;
            i = recover_i;
        }
    here7:i = i;
    }
    //8：死4
    judge_continuous = 0;//判断值归零
    for (int i = 0; i < 19; i++)
    {
        int recover_i = i;
        int judgeempty = 0;
        if (thearray[i] == 1 - side)
        {
            i += 1;
            if (i > 14)
                break;
            int tempi = i + 4;
            for (i; i < tempi; i++)
            {
                if (thearray[i] == side || thearray[i] == EMPTY)
                    judge_continuous += 1;
                else if (thearray[i] == -1)
                {
                    while (i < 18 && thearray[i + 1] != -1)
                        i += 1;
                    judge_continuous = 0;
                    goto here8;
                }
                else
                {
                    i = recover_i;
                    break;
                }
            }
        }
        if (i < 19 && judge_continuous == 4 && thearray[i] == 1 - side)
        {
            result[7] += 1;
            for (int j = 1; j < 5; j++)
            {
                if (thearray[i - j] == side)
                    thearray[i - j] = -1;
            }
        }
        else
        {
            judge_continuous = 0;
            i = recover_i;
        }
    here8:i = i;
    }
    //9：活3
    judge_continuous = 0;//判断值归零
    for (int i = 0; i < 19; i++)
    {
        if (i > 3 && judge_continuous == 3)
        {
            if (thearray[i] == EMPTY && thearray[i - 4] == EMPTY)//两头为空
            {
                result[8] += 1;//不需要跳出，因为可以同时存在两个活3，接下来继续判断
                for (int j = 1; j < 4; j++)
                {
                    if (thearray[i - j] == side)
                        thearray[i - j] = -1;
                }
            }
        }
        if (thearray[i] == side)
            judge_continuous += 1;
        else
            judge_continuous = 0;
    }
    //10：朦胧3：__O__O__O__     __OO____O__
    judge_continuous = 0;//判断值归零
    for (int i = 0; i < 19; i++)
    {
        int recover_i = i;
        int judgeempty = 0;
        if (thearray[i] == EMPTY)
        {
            i += 1;
            if (i > 13)
                break;
            int tempi = i + 5;
            for (i; i < tempi; i++)
            {
                if (thearray[i] == side || thearray[i] == EMPTY)
                {
                    judge_continuous += 1;
                    if (thearray[i] == EMPTY)
                        judgeempty += 1;
                }
                else if (thearray[i] == -1)
                {
                    while (i < 18 && thearray[i + 1] != -1)
                        i += 1;
                    judge_continuous = 0;
                    goto here10;
                }
                else
                {
                    i = recover_i;
                    break;
                }
            }
        }
        if (judge_continuous == 5 && judgeempty == 2 && thearray[i] == EMPTY)
        {
            result[9] += 1;
            for (int j = 1; j < 6; j++)
            {
                if (thearray[i - j] == side)
                    thearray[i - j] = -1;
            }
        }
        else
        {
            judge_continuous = 0;
            i = recover_i;
        }
    here10:i = i;
    }
    //11：眠3：__O__OO__
    judge_continuous = 0;//判断值归零
    for (int i = 0; i < 19; i++)
    {
        int recover_i = i;
        int judgeempty = 0;
        if (thearray[i] == side || thearray[i] == EMPTY)
        {
            if (thearray[i] == EMPTY)
                judgeempty += 1;
            i += 1;
            judge_continuous += 1;
            if (i > 14)
                break;
            int tempi = i + 5;
            for (i; i < tempi; i++)
            {
                if (thearray[i] == side || thearray[i] == EMPTY)
                {
                    judge_continuous += 1;
                    if (thearray[i] == EMPTY)
                        judgeempty += 1;
                }
                else if (thearray[i] == -1)
                {
                    while (i < 18 && thearray[i + 1] != -1)
                        i += 1;
                    judge_continuous = 0;
                    goto here11;
                }
                else
                {
                    i = recover_i;
                    break;
                }
            }
        }
        if (judge_continuous == 6 && judgeempty == 3)
        {
            result[10] += 1;
            for (int j = 1; j < 7; j++)
            {
                if (thearray[i - j] == side)
                    thearray[i - j] = -1;
            }
        }
        else
        {
            judge_continuous = 0;
            i = recover_i;
        }
    here11:i = i;
    }
    //12：死3
    judge_continuous = 0;//判断值归零
    for (int i = 0; i < 19; i++)
    {
        int recover_i = i;
        int judgeempty = 0;
        if (thearray[i] == 1 - side)
        {
            i += 1;
            if (i > 15)
                break;
            int tempi = i + 3;
            for (i; i < tempi; i++)
            {
                if (thearray[i] == side || thearray[i] == EMPTY)
                    judge_continuous += 1;
                else if (thearray[i] == -1)
                {
                    while (i < 18 && thearray[i + 1] != -1)
                        i += 1;
                    judge_continuous = 0;
                    goto here12;
                }
                else
                {
                    i = recover_i;
                    break;
                }
            }
        }
        if (i < 19 && judge_continuous == 3 && thearray[i] == 1 - side)
        {
            result[11] += 1;
            for (int j = 1; j < 4; j++)
            {
                if (thearray[i - j] == side)
                    thearray[i - j] = -1;
            }
        }
        else
        {
            judge_continuous = 0;
            i = recover_i;
        }
    here12:i = i;
    }
    //13：活2：__OO__
    judge_continuous = 0;//判断值归零
    for (int i = 0; i < 19; i++)
    {
        if (i > 2 && judge_continuous == 2)
        {
            if (thearray[i] == EMPTY && thearray[i - 3] == EMPTY)//两头为空
            {
                result[12] += 1;//不需要跳出，因为可以同时存在两个活3，接下来继续判断
                for (int j = 1; j < 3; j++)
                {
                    if (thearray[i - j] == side)
                        thearray[i - j] = -1;
                }
            }
        }
        if (thearray[i] == side)
            judge_continuous += 1;
        else
            judge_continuous = 0;
    }
    //14：眠2：六个非X其中两个O
    judge_continuous = 0;//判断值归零
    for (int i = 0; i < 19; i++)
    {
        int recover_i = i;
        int judgeempty = 0;
        if (thearray[i] == side || thearray[i] == EMPTY)
        {
            if (thearray[i] == EMPTY)
                judgeempty += 1;
            i += 1;
            judge_continuous += 1;
            if (i > 14)
                break;
            int tempi = i + 5;
            for (i; i < tempi; i++)
            {
                if (thearray[i] == side || thearray[i] == EMPTY)
                {
                    judge_continuous += 1;
                    if (thearray[i] == EMPTY)
                        judgeempty += 1;
                }
                else if (thearray[i] == -1)
                {
                    while (i < 18 && thearray[i + 1] != -1)
                        i += 1;
                    judge_continuous = 0;
                    goto here14;
                }
                else
                {
                    i = recover_i;
                    break;
                }
            }
        }
        if (judge_continuous == 6 && judgeempty == 4)
        {
            result[13] += 1;
            for (int j = 1; j < 7; j++)
            {
                if (thearray[i - j] == side)
                    thearray[i - j] = -1;
            }
        }
        else
        {
            judge_continuous = 0;
            i = recover_i;
        }
    here14:i = i;
    }
    //15：死2
    judge_continuous = 0;//判断值归零
    for (int i = 0; i < 19; i++)
    {
        int recover_i = i;
        int judgeempty = 0;
        if (thearray[i] == 1 - side)
        {
            i += 1;
            if (i > 16)
                break;
            int tempi = i + 2;
            for (i; i < tempi; i++)
            {
                if (thearray[i] == side || thearray[i] == EMPTY)
                    judge_continuous += 1;
                else if (thearray[i] == -1)
                {
                    while (i < 18 && thearray[i + 1] != -1)
                        i += 1;
                    judge_continuous = 0;
                    goto here15;
                }
                else
                {
                    i = recover_i;
                    break;
                }
            }
        }
        if (i < 19 && judge_continuous == 2 && thearray[i] == 1 - side)
        {
            result[14] += 1;
            for (int j = 1; j < 3; j++)
            {
                if (thearray[i - j] == side)
                    thearray[i - j] = -1;
            }
        }
        else
        {
            judge_continuous = 0;
            i = recover_i;
        }
    here15:i = i;
    }
}




int* analyse1(int board[19][19], int side,int g,int h)//x,y为当前棋子位置
{
    int *result = new int[15];//15种棋型
    for (int i = 0; i < 15; i++)
        result[i] = 0;
    analyse_horizontal(board, result, side,g,h);
    analyse_vertical(board, result, side,g,h);
    analyse_oblique1(board, result, side,g,h);
    analyse_oblique2(board, result, side,g,h);
    return result;
}




point* AI(int board[19][19], int side) {
    int i, j, k;
    int marks[361] = { 0 };//i*19+j         所有点的分值
    int temp_max_mark = 0;
    int temp_max_j = 0;
    point *max = new point[360];//分值最高的15个点
    int enemy_side;
    int temp_board[19][19];

    if (side == BLACK)enemy_side = WHITE;
    else enemy_side = BLACK;

    for (i = 0; i < 360; i++) {
        max[i].x = 0;
        max[i].y = 0;
        max[i].mark = -1;
    }
    int maxcount = 0;
    for (i = 0; i < 19; i++) {
        for (j = 0; j < 19; j++) {
            if (board[i][j] == EMPTY && surround(board, i, j, side)) { //只考虑棋盘的空位
                {
                    int boardtemp[19][19];
                    copy(board, boardtemp);
                    int mark_enemy1 = markenemy(analyse1(boardtemp, enemy_side,i,j));
                    boardtemp[i][j] = side;
                    int mark_self = markslef(analyse1(boardtemp, side,i,j));
                    boardtemp[i][j] = enemy_side;
                    int mark_enemy2 = markenemy(analyse1(boardtemp, enemy_side,i,j));
                    max[maxcount].mark = mark_self + mark_enemy2 - mark_enemy1;
                    max[maxcount].x = i;
                    max[maxcount].y = j;
                    maxcount += 1;
                }
            }
        }
    }


    return max;
}

//决策树：
int* AItree(int board[19][19], int side, point* max)
{
    int *result = new int[4];
    int enemy_side = 1 - side;
    point *maxtree = new point[360];//找到分数和最大的15个点
    for (int i = 0; i < 360; i++)
        maxtree[i].mark = maxtree[i].x = maxtree[i].y = -1;//赋初值0
    for (int i = 0; i < 360; i++)
    {
        int chessboard_tree[19][19] = { 0 };
        copy(board, chessboard_tree);
        if (max[i].mark >= 0)
            chessboard_tree[max[i].x][max[i].y] = side;
        else
            break;
        int marks[361] = { 0 };
        for (int z = 0; z < 361; z++)
            marks[z] = -1;
        for (int k = 0; k < 19; k++) {
            for (int l = 0; l < 19; l++) {
                if (chessboard_tree[k][l] == EMPTY && surround(chessboard_tree, k, l, side)) { //只考虑棋盘的空位
                    int boardtemp[19][19];
                    copy(chessboard_tree, boardtemp);
                    int mark_enemy1 = markenemy(analyse1(boardtemp, enemy_side,k,l));
                    boardtemp[k][l] = side;
                    int mark_self = markslef(analyse1(boardtemp, side,k,l));
                    boardtemp[k][l] = enemy_side;
                    int mark_enemy2 = markenemy(analyse1(boardtemp, enemy_side,k,l));
                    marks[k * 19 + l] = mark_self + mark_enemy2 - mark_enemy1;
                }
            }
        }
        point tempmax;
        tempmax.mark = tempmax.x = tempmax.y = 0;
        int aitree_temp_max = -1;
        for (int k = 0; k < 361; k++)//找到分数最大的点
        {
            if (marks[k]>=0&&marks[k] > aitree_temp_max)
            {
                aitree_temp_max = marks[k];
                tempmax.x = k / 19;
                tempmax.y = k % 19;
                tempmax.mark = marks[k];
            }
        }
        maxtree[i].x = tempmax.x;
        maxtree[i].y = tempmax.y;
        maxtree[i].mark = tempmax.mark;
    }
    int aitree_temp_max = -1;
    for (int i = 0; i < 360; i++)//找到最大的分数和的两步棋的坐标
    {
        if (max[i].mark >= 0&&maxtree[i].mark>=0)
        {
            if (max[i].mark + maxtree[i].mark > aitree_temp_max)
            {
                result[0] = max[i].x;
                result[1] = max[i].y;
                result[2] = maxtree[i].x;
                result[3] = maxtree[i].y;
                aitree_temp_max = max[i].mark + maxtree[i].mark;
            }
        }
        else
            break;
    }
    return result;
}

int markslef(int* result) { //根据result内的各属性打分(自己)
/*1.六连:100000
2.长连:100000
3.活5：128
4.眠5：31
5.死5:0
6.活4:1024
7:眠4:512
8死4:0
9.活3:256
10.朦胧3：32
11.眠3：32
12.死3：0
13.活2：16
14.眠2：3
15.死2: 0
*/
    int sum = 0;
    sum += result[0] * 100000;
    sum += result[1] * 100000;
    sum += result[2] * 128;
    sum += result[3] * 31;
    sum += result[4] * 0;
    sum += result[5] * 1024;
    sum += result[6] * 512;
    sum += result[7] * 0;
    sum += result[8] * 256;
    sum += result[9] * 32;
    sum += result[10] * 31;
    sum += result[11] * 0;
    sum += result[12] * 16;
    sum += result[13] * 3;
    sum += result[14] * 0;
    return sum;
}

int markenemy(int *result) { //根据result内的各属性打分（堵人）
/*1.六连:30000
2.长连:30000
3.活5：15000
4.眠5：12000
5.死5:0
6.活4:3000
7:眠4:2000
8死4:0
9.活3:256
10.朦胧3：32
11.眠3：32
12.死3：0
13.活2：16
14.眠2：3
15.死2: 0
*/
    int sum = 0;
    sum += result[0] * 30000;
    sum += result[1] * 30000;
    sum += result[2] * 15000;
    sum += result[3] * 12000;
    sum += result[4] * 0;
    sum += result[5] * 3000;
    sum += result[6] * 2000;
    sum += result[7] * 0;
    sum += result[8] * 256;
    sum += result[9] * 32;
    sum += result[10] * 31;
    sum += result[11] * 0;
    sum += result[12] * 16;
    sum += result[13] * 3;
    sum += result[14] * 0;
    return sum;
}


void copy(int board1[19][19], int board2[19][19]) {
    int i, j;
    for (i = 0; i < 19; i++) {
        for (j = 0; j < 19; j++) {
            board2[i][j] = board1[i][j];
        }
    }
}

int main()
{
    Step step;//临时步结构
    char message[256];//通信消息缓冲
    int computerSide;//己方执棋颜色
    int start = 0;//对局开始标记
    srand(int(time(0)));
    //此处放置初始化代码
    //...

    while (1)   //程序主循环
    {
        fflush(stdout);//不要删除此语句，否则程序会出问题
        scanf("%s", message);//获取平台命令消息
        //分析命令
        if (strcmp(message, "name?") == 0)//向对战平台发送队名
        {
            fflush(stdin);
            /***********将"令狐冲"改为你的队名，不超过6个汉字或12个英文字母，否则无成绩************/
            /*******/       printf("name gkd队\n");      /**只修改令狐冲，不要删除name空格****/
            /***********将"令狐冲"改为你的队名，不超过6个汉字或12个英文字母，否则无成绩************/
        }
        else if (strcmp(message, "new") == 0)//建立新棋局
        {
            int i, j;
            scanf("%s", message);//获取己方执棋颜色
            fflush(stdin);
            if (strcmp(message, "black") == 0)  computerSide = BLACK;  //执黑
            else  computerSide = WHITE;   //执白

            for (i = 0; i < 19; ++i)   //初始化棋局
                for (j = 0; j < 19; ++j)
                    Board[i][j] = EMPTY;
            start = 1;

            if (computerSide == BLACK)
            {
                /**********生成第一手着法，并保存在step结构中，落子坐标为(step.first.x,step.first.y)**********/
                /****************************在下方填充代码，并替换我的示例代码******************************/

                //此处保留老师的代码
                step.first.x = 9;
                step.first.y = 9;


                /******************************在上面填充第一步行棋代码*******************************************/

                Board[step.first.x][step.first.y] = computerSide;//处理己方行棋
                //printf("move %c%c@@\n", step.first.x + 'A', step.first.y + 'A');//输出着法
                printf("move %c%c@@\n", step.first.x + 'A', step.first.y + 'A');//输出着法
            }
        }
        else if (strcmp(message, "move") == 0)//行棋,本程序核心
        {
            scanf("%s", message);//获取对手行棋着法
            fflush(stdin);
            int len = strlen(message);

            step.first.x = message[0] - 'A';        step.first.y = message[1] - 'A';
            if (len == 4)
            {
                step.second.x = message[2] - 'A';       step.second.y = message[3] - 'A';
            }
            //处理对手行棋
            Board[step.first.x][step.first.y] = 1 - computerSide;
            //if(!(step.second.x==-1 && step.second.y==-1)) Board[step.second.x][step.second.y] = 1 - computerSide;
            if (len == 4) Board[step.second.x][step.second.y] = 1 - computerSide;
            /**********************************************************************************************************/
            /***生成落子的坐标，保存在step结构中，第一子下在(step.first.x,step.first.y)，第一子下在(step.first.x,step.first.y)***/
            /**************************************在下方填充代码，并替换我的示例代码*****************************************/

            int next_x;
            int next_y;
            int side = computerSide;
            int * temp;
            temp = new int[4];

            //生成第1子落子位置step.first.x和step.first.y
            temp = AItree(Board, side, AI(Board, side));
            step.first.x = temp[0];
            step.first.y = temp[1];
            step.second.x = temp[2];
            step.second.y = temp[3];

            Board[step.first.x][step.first.y] = computerSide;

            //生成第2子落子位置step.second.x和step.second.y


            Board[step.second.x][step.second.y] = computerSide;


            /*****************************************在上面填充代码******************************************************/
            /**********************************************************************************************************/

            printf("move %c%c%c%c\n", step.first.x + 'A', step.first.y + 'A', step.second.x + 'A', step.second.y + 'A');//输出着法
        }
        else if (strcmp(message, "error") == 0)//着法错误
        {
            fflush(stdin);
        }
        else if (strcmp(message, "end") == 0)//对局结束
        {
            fflush(stdin);
            start = 0;
        }
        else if (strcmp(message, "quit") == 0)//退出引擎
        {
            fflush(stdin);
            printf("Quit!\n");
            break;
        }
    }
    return 0;
}
