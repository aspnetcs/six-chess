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
directs compute(int board[29][29], int x, int y, int side);
int markslef(directs s);
int markenemy(directs s);
void copy(int board1[29][29], int board2[29][29]);
void initialize(int board[29][29]);


point* AI(int board[29][29], int side) {
	int i, j, k;
	int marks[361] = { 0 };//i*19+j         所有点的分值
	int temp_max_mark = 0;
	int temp_max_j = 0;
	point *max=new point[15];//分值最高的15个点
	int enemy_side;
	int temp_board[29][29];

	if (side == BLACK)enemy_side = WHITE;
	else enemy_side = BLACK;

	for (i = 0; i < 15; i++) {
		max[i].x = 0;
		max[i].y = 0;
		max[i].mark = 0;
	}

	for (i = 5; i < 24; i++) {
		for (j = 5; j < 24; j++) {
			if (board[i][j] == EMPTY) { //只考虑棋盘的空位
				marks[(i - 5) * 19 + (j - 5)] = markslef(compute(board, i, j, side)) + markenemy(compute(board, i, j, enemy_side));
			}
		}
	}

	for (i = 0; i < 15; i++) { //在361个数中选出15个最大的，并记录坐标
		for (j = i; j < 361; j++) {
			if (marks[j] > temp_max_mark) {
				temp_max_mark = marks[j];
				temp_max_j = j; //保存坐标
			}
		}
		marks[i] = temp_max_mark;
		max[i].mark = temp_max_mark;
		max[i].x = temp_max_j / 19; //向下取整
		max[i].y = temp_max_j % 19;
		temp_max_mark = 0;
		marks[temp_max_j] = 0;//将本轮循环找到的最大值在marks数组中归零，避免下轮循环再次找到这个数
	}
	//以上完成第一步的选择
	//以下决策树未写，求补充

	return max;
}

//决策树：
int* AItree(int board[29][29], int side, point* max)
{
	int *result=new int[4];
	point *maxtree=new point[15];//找到分数和最大的15个点
	for (int i = 0; i < 15; i++)
		maxtree[i].mark = maxtree[i].x = maxtree[i].y = 0;//赋初值0
	int a = board[0][0];
	for (int i = 0; i < 15; i++)
	{
		int chessboard_tree[29][29] = { 0 };
		initialize(chessboard_tree);
		
		for (int p = 0; p < 29; p++)
		{
			for (int q = 0; q < 29; q++)
				chessboard_tree[p][q] = board[p][q];
		}
		chessboard_tree[max[i].x+5][max[i].y+5] = side;

		int marks[361] = { 0 };
		for (int k = 5; k < 24; k++) {
			for (int l = 5; l < 24; l++) {
				if (chessboard_tree[k][l] == EMPTY) { //只考虑棋盘的空位
					marks[(k - 5) * 19 + (l - 5)] = markslef(compute(chessboard_tree, k, l, side)) + markenemy(compute(chessboard_tree, k, l, 1-side));
				}
			}
		}
		point tempmax;
		int aitree_temp_max = 0;
		for (int k = 0; k < 361; k++)//找到分数最大的点
		{
			if (marks[k] > aitree_temp_max)
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
	int aitree_temp_max = 0;
	for (int i = 0; i < 15; i++)//找到最大的分数和的两步棋的坐标
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
	return result;
}

int markslef(directs s) { //根据directs内的各属性打分(自己)
/*死X：0分
  活1：4分 眠1：2分
  活2：16分 眠2：3分
  活3：64分 眠3：32分
  活4：512分 眠4：52分
  活5：512分 眠5：256分
  6：9999分
*/
	int sum = 0;
	int i;
	for (i = 0; i < 4; i++) {
		if (s.live[i] == 0) {//死
			if (s.direct[i] >= 6)sum += 9999;
			else sum += 0;
		}
		else if (s.live[i] == 1) { //眠
			if (s.direct[i] == 1)sum += 2;
			else if (s.direct[i] == 2)sum += 3;
			else if (s.direct[i] == 3)sum += 32;
			else if (s.direct[i] == 4)sum += 52;
			else if (s.direct[i] == 5)sum += 256;
			else if (s.direct[i] >= 6)sum += 9999;
		}
		else { //活
			if (s.direct[i] == 1)sum += 4;
			else if (s.direct[i] == 2)sum += 16;
			else if (s.direct[i] == 3)sum += 64;
			else if (s.direct[i] == 4)sum += 512;
			else if (s.direct[i] == 5)sum += 512;
			else if (s.direct[i] >= 6)sum += 9999;
		}
	}
	return sum;
}

int markenemy(directs s) { //根据directs内的各属性打分（堵人）
/*死X：0分
  活1：4分 眠1：2分
  活2：16分 眠2：3分
  活3：64分 眠3：32分
  活4：256分 眠4：52分
  活5：512分 眠5：1024分
  6：5000分
*/
	int sum = 0;
	int i;
	for (i = 0; i < 4; i++) {
		if (s.live[i] == 0) {//死
			if (s.direct[i] >= 6)sum += 9999;
			else sum += 0;
		}
		else if (s.live[i] == 1) { //眠
			if (s.direct[i] == 1)sum += 2;
			else if (s.direct[i] == 2)sum += 3;
			else if (s.direct[i] == 3)sum += 32;
			else if (s.direct[i] == 4)sum += 52;
			else if (s.direct[i] == 5)sum += 1024;
			else if (s.direct[i] >= 6)sum += 5000;
		}
		else { //活
			if (s.direct[i] == 1)sum += 4;
			else if (s.direct[i] == 2)sum += 16;
			else if (s.direct[i] == 3)sum += 64;
			else if (s.direct[i] == 4)sum += 256;
			else if (s.direct[i] == 5)sum += 512;
			else if (s.direct[i] >= 6)sum += 5000;
		}
	}
	return sum;
}

void initialize(int board[29][29]) { //本函数的作用是把所有边界以外的点的初始值设为3
	int i, j;
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 29; j++) {
			board[i][j] = 3;
		}
	}
	for (i = 24; i < 29; i++) {
		for (j = 0; j < 29; j++) {
			board[i][j] = 3;
		}
	}
	for (i = 5; i < 24; i++) {
		for (j = 0; j < 5; j++) {
			board[i][j] = 3;
		}
	}
	for (i = 5; i < 24; i++) {
		for (j = 24; j < 29; j++) {
			board[i][j] = 3;
		}
	}
}

directs compute(int board[29][29], int x, int y, int side) { //判断落子后有多少个己方棋子相连（分四个方向）
	//以落子点为中心，计算周围
	int i;
	int flag = 0;
	int side1, side2;
	directs s;
	s.live[0] = 0;
	s.live[1] = 0;
	s.live[2] = 0;
	s.live[3] = 0;

	//横向
	side1 = 0;
	side2 = 0;

	for (i = 1; i <= 5; i++) {
		if (board[x][y - i] == side) {
			side1++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x][y - i] == EMPTY) {
		s.live[0]++;
	}

	for (i = 1; i <= 5; i++) {
		if (board[x][y + i] == side) {
			side2++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x][y + i] == EMPTY) {
		s.live[0]++;
	}
	s.direct[0] = 1 + side1 + side2;


	//竖向
	side1 = 0;
	side2 = 0;

	for (i = 1; i <= 5; i++) {
		if (board[x - i][y] == side) {
			side1++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x - i][y] == EMPTY) {
		s.live[1]++;
	}

	for (i = 1; i <= 5; i++) {
		if (board[x + i][y] == side) {
			side2++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x + i][y] == EMPTY) {
		s.live[1]++;
	}
	s.direct[1] = 1 + side1 + side2;

	//斜向 左上到右下
	side1 = 0;
	side2 = 0;

	for (i = 1; i <= 5; i++) {
		if (board[x - i][y - i] == side) {
			side1++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x - i][y - i] == EMPTY) {
		s.live[2]++;
	}

	for (i = 1; i <= 5; i++) {
		if (board[x + i][y + i] == side) {
			side2++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x + i][y + i] == EMPTY) {
		s.live[2]++;
	}
	s.direct[2] = 1 + side1 + side2;

	//斜向 左下到右上
	side1 = 0;
	side2 = 0;

	for (i = 1; i <= 5; i++) {
		if (board[x + i][y - i] == side) {
			side1++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x + i][y - i] == EMPTY) {
		s.live[3]++;
	}

	for (i = 1; i <= 5; i++) {
		if (board[x - i][y + i] == side) {
			side2++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x - i][y + i] == EMPTY) {
		s.live[3]++;
	}
	s.direct[3] = 1 + side1 + side2;

	return s;
}

void copy(int board1[29][29], int board2[29][29]) {
	int i, j;
	for (i = 0; i < 29; i++) {
		for (j = 0; j < 29; j++) {
			board2[i][j] = board1[i][j];
		}
	}
}

void copy(int board1[19][19], int board2[29][29]) {
	int i, j;
	for (i = 0; i < 19; i++) {
		for (j = 0; j < 19; j++) {
			board2[i + 5][j + 5] = board1[i][j];
		}
	}
}


/*以上为本人写的函数*/


int main()
{
	Step step;//临时步结构
	char message[256];//通信消息缓冲
	int computerSide;//己方执棋颜色
	int start = 0;//对局开始标记
	srand(int(time(0)));
	//此处放置初始化代码
	//...

	while (1)	//程序主循环
	{
		fflush(stdout);//不要删除此语句，否则程序会出问题
		scanf("%s", message);//获取平台命令消息
		//分析命令
		if (strcmp(message, "name?") == 0)//向对战平台发送队名
		{
			fflush(stdin);
			/***********将"令狐冲"改为你的队名，不超过6个汉字或12个英文字母，否则无成绩************/
			/*******/		printf("name gkd队\n");		/**只修改令狐冲，不要删除name空格****/
			/***********将"令狐冲"改为你的队名，不超过6个汉字或12个英文字母，否则无成绩************/
		}
		else if (strcmp(message, "new") == 0)//建立新棋局
		{
			int i, j;
			scanf("%s", message);//获取己方执棋颜色
			fflush(stdin);
			if (strcmp(message, "black") == 0)	computerSide = BLACK;  //执黑
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
				printf("move %c%c\n", step.first.x + 'A', step.first.y + 'A');//输出着法
			}
		}
		else if (strcmp(message, "move") == 0)//行棋,本程序核心
		{
			scanf("%s", message);//获取对手行棋着法
			fflush(stdin);
			int len = strlen(message);

			step.first.x = message[0] - 'A';		step.first.y = message[1] - 'A';
			if (len == 4)
			{
				step.second.x = message[2] - 'A';		step.second.y = message[3] - 'A';
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
			int chessboard[29][29] = { EMPTY };//空棋盘
			initialize(chessboard);
			copy(Board, chessboard); //复制系统的棋盘
			temp=AItree( chessboard,side,AI(chessboard, side));
			step.first.x = temp[0];
			step.first.y = temp[1];
			step.second.x = temp[2];
			step.second.y = temp[3];
			/*next_x = temp[0];
			next_y = temp[1];
			chessboard[next_x][next_y] = computerSide;

			step.first.x = next_x - 5;
			step.first.y = next_y - 5;*/
			Board[step.first.x][step.first.y] = computerSide;

			//生成第2子落子位置step.second.x和step.second.y	

			/*initialize(chessboard);
			copy(Board, chessboard); //复制系统的棋盘
			temp = AI(chessboard, side, 3);
			next_x = temp[0];
			next_y = temp[1];

			step.second.x = next_x - 5;
			step.second.y = next_y - 5;*/
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