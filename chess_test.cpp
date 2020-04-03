
#include <iostream>
#include <ctime> 

using namespace std;

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
	int i, j, k;
	int flag = 0;

	//竖向
	for (i = x - 5; i <= x; i++) {
		j = y;
		if (board[i][j] == side) {
			flag = 1;
			for (k = 1; k <= 5; k++) { //从最上一格往下数
				if (board[i + k][j] != side) {
					flag = 0;
					break;
				}
			}
			if (flag == 1) return true;
		}
	}

	//横向
	for (i = y - 5; i <= y; i++) {
		j = x;
		if (board[j][i] == side) {
			flag = 1;
			for (k = 1; k <= 5; k++) { //从最左一格往右数
				if (board[j][i + k] != side) {
					flag = 0;
					break;
				}
			}
			if (flag == 1)return true;
		}
	}

	//斜向 左上到右下
	for (i = y - 5, j = x - 5; i <= y, j <= x; i++, j++) {
		if (board[j][i] == side) {
			flag = 1;
			for (k = 1; k <= 5; k++) { //从最上一格往下数
				if (board[j + k][i + k] != side) {
					flag = 0;
					break;
				}
			}
			if (flag == 1)return true;
		}
	}

	//斜向 右上到左下
	for (i = y + 5, j = x - 5; i >= y, j <= x; i--, j++) {
		if (board[j][i] == side) {
			flag = 1;
			for (k = 1; k <= 5; k++) { //从最下一格往上数
				if (board[j + k][i - k] != side) {
					flag = 0;
					break;
				}
			}
			if (flag == 1)return true;
		}
	}


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
	srand(time(0));


	while (steps < 361) {
		//走棋
		do {
			next_x = rand() % 19 + 5;
			next_y = rand() % 19 + 5;
		} while (next_x >= 5 && next_x <= 23 && next_y >= 5 && next_y <= 23 && chessboard[next_x][next_y] != 0);
		move(next_x, next_y, side, chessboard);

		//画图
		cout << steps << "(" << next_x - 5 << ", " << next_y - 5 << ")" << endl;
		draw(chessboard);

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

