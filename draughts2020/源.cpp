#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<math.h>
#define DRAUGHTS_CHESS_NUM 12
#define DRAUGHTS_BOARD_SIZE 8
#define DRAUGHTS_EMPTY 0
#define DRAUGHTS_BLACK 1
#define DRAUGHTS_WHITE -1
#define DRAUGHTS_BLACK_KING 2
#define DRAUGHTS_WHITE_KING -2
#define DEPTH 6
#define MAX(a,b) (a)>(b)?(a):(b)
#define MIN(a,b) (a)<(b)?(a):(b)

static int Draughts_my_turn = 0;//记录游戏中己方所行棋
static long Draughts_times = 0;//代表该局面下搜索到达所需的深度时的分支总数
static double Draughts_scorepast;//代表深度为1时检查是否改变的估值
static char Draughts_seem[10] = {};//代表深度为1模拟走棋时缓存的坐标
static char Draughts_dir[100] = {};//代表录入或者需输出的坐标
static int Draughts_move_dir[4][2] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };
static int Draughts_eat_dir[4][2] = { {-2, -2}, {-2, 2}, {2, -2}, {2, 2} };
									/*   ↖        ↗      ↙        ↘  */

struct draughts_dfs_reference {
	int board_reference[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE] = { 0 };//拷贝棋盘
	int flag_eat[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE] = { 0 };//吃子标记
	int way_now[10][2] = { 0 };//当前路径
	int way_max[10][2] = { 0 };//最大路径
	int step_max = 0;//最大路径的长度
	int depth = 0;//当前搜索深度
	double score_before = 0;//当前层分数
};//专门用来存放dfs方法所需传递的参数的结构体
struct draughts_command {
	int step = 0;
	int dir[10][2] = { 0 };
}input_my;//用于记录我方走子的结构体

int Draughts_board_main[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE] = { 0,-1, 0,-1, 0,-1, 0,-1,
																	 -1, 0,-1, 0,-1, 0,-1, 0,
																	  0,-1, 0,-1, 0,-1, 0,-1,
																	  0, 0, 0, 0, 0, 0, 0, 0,
																	  0, 0, 0, 0, 0, 0, 0, 0,
																	  1, 0, 1, 0, 1, 0, 1, 0,
																	  0, 1, 0, 1, 0, 1, 0, 1,
																	  1, 0, 1, 0, 1, 0, 1, 0 };//初始化全局棋盘
static int draughts_is_in_bound(int x, int y) {
	return (x >= 0 && x < DRAUGHTS_BOARD_SIZE && y >= 0 && y < DRAUGHTS_BOARD_SIZE);
}//判断是否位于界内
static void draughts_board_spin(int board_spined[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]) {
	for (int i = 0; i < DRAUGHTS_BOARD_SIZE / 2; i++) {
		for (int j = 0; j < DRAUGHTS_BOARD_SIZE; j++) {
			int temp = board_spined[i][j];
			board_spined[i][j] = 0 - board_spined[DRAUGHTS_BOARD_SIZE - i - 1][DRAUGHTS_BOARD_SIZE - j - 1];
			board_spined[DRAUGHTS_BOARD_SIZE - i - 1][DRAUGHTS_BOARD_SIZE - j - 1] = 0 - temp;
		}
	}//交换两个位置的棋子并变换棋子归属方
}//旋转棋盘
static void draughts_board_print(int board_printed[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]) {
	for (int i = 0; i < DRAUGHTS_BOARD_SIZE; i++) {
		for (int j = 0; j < DRAUGHTS_BOARD_SIZE; j++) {
			switch (board_printed[i][j]) {
			case DRAUGHTS_EMPTY:
				printf("..");
				break;
			case DRAUGHTS_WHITE:
				printf("ww");
				break;
			case DRAUGHTS_WHITE_KING:
				printf("WW");
				break;
			case DRAUGHTS_BLACK:
				printf("bb");
				break;
			case DRAUGHTS_BLACK_KING:
				printf("BB");
				break;
			}
		}
		printf("\n");
	}
}//将当前棋盘输出
static void draughts_board_copy(int board_sub[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE], int board_copy[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]) {
	int i, j;
	for (i = 0; i < DRAUGHTS_BOARD_SIZE; i++)
		for (j = 0; j < DRAUGHTS_BOARD_SIZE; j++)board_copy[i][j] = board_sub[i][j];
}//复制一份旧棋盘到棋盘的拷贝当中

static void draughts_view_board(int board_viewed[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE], int* num_black, int* num_white, int dir_black[DRAUGHTS_CHESS_NUM][3], int dir_white[DRAUGHTS_CHESS_NUM][3]) {
	*num_black = *num_white = 0;
	for (int i = 0; i < DRAUGHTS_BOARD_SIZE; i++) {
		for (int j = 0; j < DRAUGHTS_BOARD_SIZE; j++) {
			if (board_viewed[i][j] < DRAUGHTS_EMPTY) {
				dir_white[*num_white][0] = i;
				dir_white[*num_white][1] = j;
				if (i == 7) {
					board_viewed[i][j] = DRAUGHTS_WHITE_KING;
					dir_white[*num_white][2] = 1;
				}
				(*num_white)++;
			}
			else if (board_viewed[i][j] > DRAUGHTS_EMPTY) {
				dir_black[*num_black][0] = i;
				dir_black[*num_black][1] = j;
				if (i == 0) {
					board_viewed[i][j] = DRAUGHTS_BLACK_KING;
					dir_black[*num_black][2] = 1;
				}
				(*num_black)++;
			}
		}
	}
}//遍历棋盘，返回棋盘中双方棋子的个数以及我方棋子的坐标，顺便进行升王判断
static int draughts_score_a(int board_a[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE], int times, int num_chess, int dir_chess[DRAUGHTS_CHESS_NUM][3]) {
	int board_marked[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE] = { 0 };//记录是否被标记
	int dir_now[2] = { 0 };//存储当前坐标
	int len = 0;//记录长度
	int value_a = 0;//记录该函数内的总分
	for (int i = 0; i < num_chess; i++) {
		dir_now[0] = dir_chess[i][0];//依次读取坐标
		dir_now[1] = dir_chess[i][1];
		if (board_marked[dir_now[0]][dir_now[1]] == 0) {//若未被标记
			do {
				len++;
				board_marked[dir_now[0]][dir_now[1]] = 1;//将其标记
				dir_now[0] += Draughts_move_dir[times][0];//向前方延展
				dir_now[1] += Draughts_move_dir[times][1];
			} while (board_marked[dir_now[0]][dir_now[1]] == 0 && draughts_is_in_bound(dir_now[0], dir_now[1]) && board_a[dir_now[0]][dir_now[1]] > DRAUGHTS_EMPTY);//直到前方无棋子
			dir_now[0] = dir_chess[i][0];//当前坐标返回至初始读取坐标
			dir_now[1] = dir_chess[i][1];
			board_marked[dir_now[0]][dir_now[1]] = 0;//取消标记
			len--;
			do {
				len++;
				board_marked[dir_now[0]][dir_now[1]] = 1;//将其标记
				dir_now[0] -= Draughts_move_dir[times][0];//向后方延展
				dir_now[1] -= Draughts_move_dir[times][1];
			} while (board_marked[dir_now[0]][dir_now[1]] == 0 && draughts_is_in_bound(dir_now[0], dir_now[1]) && board_a[dir_now[0]][dir_now[1]] > DRAUGHTS_EMPTY);//直到后方无棋子
			board_marked[dir_chess[i][0]][dir_chess[i][1]] = 1;//最后保证确实进行了标记
			if (len >= 3) value_a += len - 2;
		}
	}
	return value_a;
}
static void draughts_score_b(int board_b[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE], int num_chess, int dir_chess[DRAUGHTS_CHESS_NUM][3], int *value) {
	int flag_jump = 0;//可移动标记，代表某棋子前方是否可移动或为对方的棋子
	int flag_def = 0;//防守标记，代表某棋子后方是否有自己方的棋子
	int dir_now[2] = { 0 };//记录当前坐标
	for (int i = 0; i < num_chess; i++) {
		flag_jump = 0;
		if (dir_chess[i][2] == 1) value[0] += 70;//升王棋后的价值，应远大于升格前，使得程序更加积极的升格
		else {
			value[0] += (20 + (12 - i));//非王棋棋子基本分数，随棋子的数量增加而减少单个价值
			value[1] += (10 + dir_chess[i][0] * 2);//非王棋子的高度分数，使得棋子更加积极向上移动
			for (int j = 0; j < 2; j++) {
				dir_now[0] = dir_chess[i][0] - Draughts_move_dir[i][0];
				dir_now[1] = dir_chess[i][1] - Draughts_move_dir[i][1];
				if (draughts_is_in_bound(dir_now[0], dir_now[1]) && board_b[dir_now[0]][dir_now[1]] < DRAUGHTS_EMPTY) flag_jump++;
				dir_now[0] = dir_chess[i][0] + Draughts_move_dir[i][0];
				dir_now[1] = dir_chess[i][1] + Draughts_move_dir[i][1];
				if (draughts_is_in_bound(dir_now[0], dir_now[1]) && board_b[dir_now[0]][dir_now[1]] > DRAUGHTS_EMPTY) flag_def++;
			}
			if (flag_def == 2 && flag_jump >= 1) value[2]++;//三角阵型分数，若构成三角阵且位列前方则加分，使得棋子具有一定的阵型意识
		}
		if (dir_chess[i][0] > 1 && dir_chess[i][1] > 1 && dir_chess[i][0] < 6 && dir_chess[i][1] < 6) value[1] += 10;//棋子区位分数，中间的棋子分数高
	}
}
static int draughts_score_c(int board_c[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]) {
	int value_c = 0;
	if (board_c[1][0] == DRAUGHTS_WHITE)value_c--;
	if (board_c[2][7] == DRAUGHTS_WHITE)value_c--;
	if (board_c[6][7] == DRAUGHTS_BLACK)value_c++;
	if (board_c[7][0] == DRAUGHTS_BLACK)value_c++;//边线棋子的优势分数，距对方底线近的边线棋子威胁大，距己方底线近的边线棋子移动受到限制
	return value_c;
}
static double draughts_score(int board_scored[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]) {
	double score = 0;
	double value[5] = { 0 };
	int Weight[5] = { 2,2,15,10,15 };//代表V01234的权重
	int num_white, num_black;
	int dir_white[DRAUGHTS_CHESS_NUM][3] = { 0 };
	int dir_black[DRAUGHTS_CHESS_NUM][3] = { 0 };
	draughts_view_board(board_scored, &num_black, &num_white, dir_black, dir_white);
	for (int i = 0; i < 2; i++) {
		draughts_board_spin(board_scored);
		if (i == 0) {
			int value_b[3] = { 0 };
			draughts_score_b(board_scored, num_white, dir_white, value_b);
			value[0] -= value_b[0] * 0.8;//消减对手的棋子基本分数所占的权重，使程序减少以一换一的次数
			value[1] -= value_b[1];
			value[2] -= value_b[2];
		}
		value[3] -= draughts_score_a(board_scored, i, num_white, dir_white);
		draughts_board_spin(board_scored);
		if (i == 0) {
			int value_b[3] = { 0 };
			draughts_score_b(board_scored, num_black, dir_black, value_b);
			value[0] += value_b[0];
			value[1] += value_b[1];
			value[2] += value_b[2];
		}
		value[3] += draughts_score_a(board_scored, i, num_black, dir_black);
		value[4] += draughts_score_c(board_scored);
	}
}

void draughts_start() {
	scanf("%d", &Draughts_my_turn);
	printf("OK\n");
	fflush(stdout);
	if (Draughts_my_turn == 1)Draughts_my_turn = DRAUGHTS_BLACK;
	else Draughts_my_turn = DRAUGHTS_WHITE;
}

static void draughts_place_main(int board_placed[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE], int step, int input_dir[10][2]) {
	int from_x, from_y, mid_x, mid_y, to_x, to_y;
	for (int i = 0; i < step - 1; i++) {
		from_x = input_dir[i][0];
		from_y = input_dir[i][1];
		to_x = input_dir[i + 1][0];
		to_y = input_dir[i + 1][1];
		board_placed[to_x][to_y] = board_placed[from_x][from_y];
		if (abs(from_x - to_x) == 2) {//判断走还是跳
			mid_x = (from_x + to_x) / 2;
			mid_y = (from_y + to_y) / 2;
			board_placed[mid_x][mid_y] = DRAUGHTS_EMPTY;
		}
		board_placed[from_x][from_y] = DRAUGHTS_EMPTY;
	}
}//对接收到的对方的走子指令进行移动
void draughts_place() {
	int step = 0;//代表录入所经过的格数
	int input_dir[10][2] = {};
	scanf("%d", &step);
	for (int i = 0; i < step; i++) scanf("%d,%d", &input_dir[i][0], &input_dir[i][1]);
	draughts_place_main(Draughts_board_main, step, input_dir);
}

int draughts_check_eat(int board_checked[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE], int dir_eat[DRAUGHTS_CHESS_NUM][2]) {
	int to_x, to_y;//被吃子坐标
	int jump_x, jump_y;//目标格坐标	
	int num = 0;//记录可以发生吃子的棋子数量，若为0则代表无法发生吃子
	int i, j;
	int num_black, num_white;//代表当前黑白棋的个数
	int dir_black[12][3] = { 0 };//代表当前黑白棋的坐标
	int dir_white[12][3] = { 0 };//其中[0][1]为横纵坐标，[3]为是否成王
	draughts_view_board(board_checked, &num_black, &num_white, dir_black, dir_white);//遍历棋盘
	for (i = 0; i < num_black; i++) {
		for (j = 0; j < 4; j++) {
			to_x = dir_black[i][0] + Draughts_move_dir[j][0];
			to_y = dir_black[i][1] + Draughts_move_dir[j][1];
			jump_x = dir_black[i][0] + Draughts_eat_dir[j][0];
			jump_y = dir_black[i][1] + Draughts_eat_dir[j][1];//获得坐标
			if (draughts_is_in_bound(jump_x, jump_y) && board_checked[to_x][to_y] < DRAUGHTS_EMPTY && board_checked[jump_x][jump_y] == DRAUGHTS_EMPTY) {
				dir_eat[num][0] = dir_black[i][0];
				dir_eat[num][1] = dir_black[i][1];//记录可以发生吃子的棋子的坐标
				num++;
				break;
			}
		}
	}
	return num;
}
void draughts_try_eat(int board_sub[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE], int eat_dir[12][2], int num_eat, int depth) {
	void draughts_dfs_eat(struct draughts_dfs_reference *reference, int step);
	void draughts_turn_main(int now_board[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]);
	struct draughts_dfs_reference reference;
	draughts_board_copy(board_sub, reference.board_reference);
	for (int i = 0; i < num_eat; i++) {
		reference.way_now[0][0] = eat_dir[i][0];
		reference.way_now[0][1] = eat_dir[i][1];
		draughts_dfs_eat(&reference, 1);
	}
	if (depth == 1) {//如果深度为1，则当前最大坐标即为走子坐标
		input_my.step = reference.step_max;//记录经过格数
		for (int i = 0; i < input_my.step; i++) {
			input_my.dir[i][0] = reference.way_max[i][0];
			input_my.dir[i][1] = reference.way_max[i][1];
		}
	}/*
	else if (depth == DEPTH) {//若深度到达，进行估值并返回
		if (depth % 2 == 0) SpinTheBoard(board_copy);
		if (depth % 2 == 0)	*Scorenow = MIN(*Scorenow, score(board_copy));
		else if (depth % 2 == 1) *Scorenow = MAX(*Scorenow, score(board_copy));
		if (depth % 2 == 0) SpinTheBoard(board_copy);
		times++;
	}
	else {//若深度未到且不为1，则进行下一层搜索
		draughts_place_main(reference.board_reference, reference.step_max, reference.way_max);//对拷贝棋盘进行走子
		SpinTheBoard(board_copy);
		TURN(board_copy, depth + 1, Scorebe);
	}*/
}
void draughts_try_move(int board_sub[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]) {

}
void draughts_dfs_eat(struct draughts_dfs_reference* reference, int step) {
	int from_x = reference->way_now[step - 1][0];//起始坐标
	int from_y = reference->way_now[step - 1][1];
	int jump_x, jump_y;//目标坐标
	int mid_x, mid_y;//被吃子的坐标
	int flag = 0;//标记是否能够吃子
	double ScoreNow;//当前分数
	if (reference->depth % 2 == 0)ScoreNow = 10000;//对分数赋初值
	else ScoreNow = -10000;
	double PastScore = 0;//分数缓存，用于判断当前分数是否发生改变
	int board_copy[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE] = { 0 };
	for (int j = 0; j < 4; j++)	{//枚举4个方向是否可吃
		jump_x = from_x + Draughts_eat_dir[j][0];
		jump_y = from_y + Draughts_eat_dir[j][1];
		mid_x = (from_x + jump_x) / 2;
		mid_y = (from_y + jump_y) / 2;//若该位置没被越过、在范围内、能发生吃子、可为空可放子
		if (reference->flag_eat[mid_x][mid_y] == 0 && draughts_is_in_bound(jump_x, jump_y) && reference->board_reference[mid_x][mid_y] < DRAUGHTS_EMPTY && reference->board_reference[jump_x][jump_y] == DRAUGHTS_EMPTY) {
			reference->flag_eat[mid_x][mid_y] = 1;//标记该位置已被越过
			flag = 1;//标记当前棋子能够发生吃子
			reference->way_now[step][0] = jump_x;
			reference->way_now[step][1] = jump_y;//记录这一步的坐标
			int temp = reference->board_reference[mid_x][mid_y];
			reference->board_reference[jump_x][jump_y] = reference->board_reference[from_x][from_y];
			reference->board_reference[from_x][from_y] = DRAUGHTS_EMPTY;
			draughts_dfs_eat(reference, step + 1);//递归调用该函数再次搜索
			reference->board_reference[from_x][from_y] = reference->board_reference[jump_x][jump_y];
			reference->board_reference[jump_x][jump_y] = DRAUGHTS_EMPTY;
			reference->board_reference[mid_x][mid_y] = temp;
			reference->flag_eat[mid_x][mid_y] = 0;//取消该位置的标记
		}
	}
	if (flag == 0) {//如果未发生吃子
		if (reference->step_max < step) {//判断是否为最长路径
			reference->step_max = step;//若原路径短，则进行路径替换
			for (int i = 0; i < reference->step_max; i++) {
				reference->way_max[i][0] = reference->way_now[i][0];
				reference->way_max[i][1] = reference->way_now[i][1];
			}
		}/*
		else if (reference->step_max == step)
		{//若原路径与现路径长度相同，通过模拟走棋计算分数判断应取何
			//该段代码应该存在优化空间
			Doit(step, MaxWay, board, depth, &ScoreNow, Scorebefore);
			PastScore = ScoreNow;
			ScoreNow = *Scorebefore;
			*Scorebefore = PastScore;
			Doit(step, NowWay, board, depth, &ScoreNow, Scorebefore);
			PastScore = ScoreNow;
			if (depth % 2 == 1) *Scorebefore = MAX(*Scorebefore, ScoreNow);
			else if (depth % 2 == 0) *Scorebefore = MIN(*Scorebefore, ScoreNow);
			if (PastScore != *Scorebefore)//如果分数发生改变，证明新路径最优，进行替换
			{
				for (i = 0; i < *MaxStep; i++)
				{
					MaxWay[i][0] = NowWay[i][0];
					MaxWay[i][1] = NowWay[i][1];
				}
			}
		}*/
	}
}
static void draughts_print_command() {
	if (Draughts_my_turn == DRAUGHTS_WHITE) {
		for (int i = 0; i < input_my.step; i++) {
			input_my.dir[i][0] = 7 - input_my.dir[i][0];
			input_my.dir[i][1] = 7 - input_my.dir[i][1];
		}
	}
	printf("%d", input_my.step);
	for (int i = 0; i < input_my.step; i++) printf(" %d,%d", input_my.dir[i][0], input_my.dir[i][1]);
	printf("\n");
	fflush(stdout);
}
static void draughts_turn_main(int now_board[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]) {
	//先检查是否存在有吃必吃
		//若存在则寻找有多吃多的路线
			//若有多吃多存在多条则对每一条进行搜索并估值
		//若不存在有多吃多则枚举所有可行棋子
		//对每一个行走后进行翻转并向下传递
	int draughts_check_eat(int board_checked[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE], int dir_eat[DRAUGHTS_CHESS_NUM][2]);
	void draughts_try_eat(int board_sub[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE], int eat_dir[12][2], int num_eat, int depth);
	void draughts_try_move(int board_sub[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]);
	int dir_eat[DRAUGHTS_CHESS_NUM][2] = { 0 };//向内层传递可以发生吃子的棋子坐标
	int num_eat = draughts_check_eat(now_board, dir_eat);//记录可以发送吃子的棋子个数
	if (num_eat != 0) draughts_try_eat(now_board, dir_eat, num_eat, 1);
	else (draughts_try_move(now_board));
}
void draughts_turn() {
	if (Draughts_my_turn == DRAUGHTS_WHITE)draughts_board_spin(Draughts_board_main);
	draughts_turn_main(Draughts_board_main);
	draughts_place_main(Draughts_board_main, input_my.step, input_my.dir);
	draughts_print_command();
	if (Draughts_my_turn == DRAUGHTS_WHITE)draughts_board_spin(Draughts_board_main);
}

int draughts_loop() {
	char command[10] = {};
	scanf("%s", command);
	if (strcmp(command, "START") == 0) {
		draughts_start();
	}
	else if (strcmp(command, "PLACE") == 0) {
		draughts_place();
	}
	else if (strcmp(command, "TURN") == 0) {
		draughts_turn();
	}
	else if (strcmp(command, "END") == 0) {
		return 0;
	}
	return 1;
}
int main() {
	while (draughts_loop())draughts_board_print(Draughts_board_main);
	return 0;
}