#include <iostream>
#include <ctime> 

using namespace std;

struct directs {
	//directX表示每个方向有几个相连，liveX表示每个方向是死0、眠1、活2
	int direct1;
	int direct2;
	int direct3;
	int direct4;
	int live1;
	int live2;
	int live3;
	int live4;
};

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
	s.live1 = 0;
	s.live2 = 0;
	s.live3 = 0;
	s.live4 = 0;

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
	if (board[x][y - i] == 0) {
		s.live1++;
	}

	for (i = 1; i <= 5; i++) {
		if (board[x][y + i] == side) {
			side2++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x][y + i] == 0) {
		s.live1++;
	}
	s.direct1 = 1 + side1 + side2;


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
	if (board[x - i][y] == 0) {
		s.live2++;
	}

	for (i = 1; i <= 5; i++) {
		if (board[x + i][y] == side) {
			side2++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x + i][y] == 0) {
		s.live2++;
	}
	s.direct2 = 1 + side1 + side2;

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
	if (board[x - i][y - i] == 0) {
		s.live3++;
	}

	for (i = 1; i <= 5; i++) {
		if (board[x + i][y + i] == side) {
			side2++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x + i][y + i] == 0) {
		s.live3++;
	}
	s.direct3 = 1 + side1 + side2;

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
	if (board[x + i][y - i] == 0) {
		s.live4++;
	}

	for (i = 1; i <= 5; i++) {
		if (board[x - i][y + i] == side) {
			side2++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x - i][y + i] == 0) {
		s.live4++;
	}
	s.direct4 = 1 + side1 + side2;
	
	return s;
}

void draw(int board[29][29]) {
	int i, j;
	for (i = 5; i < 24; i++) {
		for (j = 5; j < 24; j++) {
			if (board[i][j] == 0) {
				cout << "_ ";
			}
			else if (board[i][j] == 1) { //黑棋
				cout << "x ";
			}
			else { //白棋
				cout << "o ";
			}
		}
		cout << endl;
	}
	cout << endl;
}

void move(int x, int y, int side, int board[29][29]) {
	if (board[x][y] == 0) {
		board[x][y] = side;
	}
}

bool victory(int board[29][29], int x, int y, int side) {
	directs s;

	s = compute(board, x, y, side);
	if (s.direct1 >= 6)return true;
	if (s.direct2 >= 6)return true;
	if (s.direct3 >= 6)return true;
	if (s.direct4 >= 6)return true;

	return false;
}

int main()
{
	int chessboard[29][29] = { 0 };
	int next_x;
	int next_y;
	int steps = 1;
	int side = 1;
	bool win = 0;
	directs sums;
	srand(time(0));

	initialize(chessboard);

	while (steps < 361) {
		//走棋
		do {
			next_x = rand() % 19 + 5;
			next_y = rand() % 19 + 5;
			//cin >> next_x;
			//cin >> next_y;
			//next_x += 5;
			//next_y += 5;
		} while (next_x >= 5 && next_x <= 23 && next_y >= 5 && next_y <= 23 && chessboard[next_x][next_y] != 0);
		move(next_x, next_y, side, chessboard);

		//画图
		cout << steps << "(" << next_x - 5 << ", " << next_y - 5 << ")" << endl;
		draw(chessboard);
		sums = compute(chessboard, next_x, next_y, side);
		cout << "横向：" << sums.direct1 << " " << sums.live1 << " ";
		cout << "竖向：" << sums.direct2 << " " << sums.live2 << " ";
		cout << " 左对角线：" << sums.direct3 << " " << sums.live3 << " ";
		cout << " 右对角线：" << sums.direct4 << " " << sums.live4 << endl;

		//判断胜负
		win = victory(chessboard, next_x, next_y, side);
		if (win) {
			cout << "胜利" << endl;
			break;
		}

		//黑换白或白换黑
		if (steps % 4 == 1) {
			side = 2;
		}
		else if (steps % 4 == 3) {
			side = 1;
		}
		steps++;
	}
}

