#pragma once
#define _CRT_SECURE_NO_WARNINGS		//不准警告
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define DRAUGHTS_CHESS_NUM 12		//棋盘上一方最多拥有的棋子数量
#define DRAUGHTS_BOARD_SIZE 8		//棋盘大小
#define DRAUGHTS_EMPTY 0			//无棋子
#define DRAUGHTS_BLACK 1			//黑色棋子
#define DRAUGHTS_WHITE -1			//白色棋子
#define DRAUGHTS_BLACK_KING 2		//黑色王棋
#define DRAUGHTS_WHITE_KING -2		//白色王棋
#define DRAUGHTS_DEPTH 6			//搜索深度
#define MAX(a,b) (a)>(b)?(a):(b)	//求最大值函数
#define MIN(a,b) (a)<(b)?(a):(b)	//求最小值函数
//全局变量声明
static long Draughts_times = 0;		//调试用，代表该局面下搜索到达所需的深度时的分支总数
static int Draughts_turn_my = 0;	//记录游戏中己方所行棋
static int Draughts_dir_move[4][2] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };	//走子坐标
static int Draughts_dir_eat[4][2] = { {-2, -2}, {-2, 2}, {2, -2}, {2, 2} };		//吃子坐标
//结构体声明							/*    ↖       ↗       ↙       ↘  */
struct draughts_dfs_reference {//专门用来存放dfs方法所需传递的参数的结构体
	int board_reference[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE] = { {0} };//拷贝棋盘
	int flag_eat[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE] = { {0} };//吃子标记
	int dir_now[10][2] = { {0} };//当前路径
	int dir_max[10][2] = { {0} };//最大路径
	int step_max = 0;//最大路径的长度
	int depth = 0;//当前搜索深度
	double score_before = 0;//上一层分数
	double score_now = 0;//当前层分数
};
struct draughts_command {//用于记录走子的结构体
	int step = 0;
	int dir[10][2] = { {0} };
};
struct draughts_info {//用于记录当前棋盘上所有棋子个数和坐标
	int num_black = 0;
	int num_white = 0;
	int dir_black[DRAUGHTS_CHESS_NUM][3] = { {0} };
	int dir_white[DRAUGHTS_CHESS_NUM][3] = { {0} };
};
static struct draughts_command Input_my;//用于记录我方走子的全局结构体
int Draughts_board_main[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE] = { { 0,-1, 0,-1, 0,-1, 0,-1 },//全局棋盘
																	  {-1, 0,-1, 0,-1, 0,-1, 0 },
																	  { 0,-1, 0,-1, 0,-1, 0,-1 },
																	  { 0, 0, 0, 0, 0, 0, 0, 0 },
																	  { 0, 0, 0, 0, 0, 0, 0, 0 },
																	  { 1, 0, 1, 0, 1, 0, 1, 0 },
																	  { 0, 1, 0, 1, 0, 1, 0, 1 },
																	  { 1, 0, 1, 0, 1, 0, 1, 0 } };
//TURN命令函数预声明

	/*尝试移动棋子*/
static void draughts_move_try(int board_moved[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]/*被移动棋盘*/, struct draughts_command my_command/*移动命令结构体*/, int depth/*当前搜索深度*/, double *score_now/*当前层分数*/);
	/*搜索可移动棋子*/
static void draughts_move_search(int board_sub[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]/*被搜索棋盘*/, int depth/*当前搜索深度*/, double *score_before/*上一层分数*/, double *score_now/*当前层分数*/);
	/*比较有多吃多最优路径*/
static void draughts_eat_cmp(struct draughts_dfs_reference* reference/*dfs方法所需所有参数*/);
	/*使用dfs方法搜索有多吃多*/
static void draughts_eat_dfs(struct draughts_dfs_reference* reference/*dfs方法所需所有参数*/, int step/*dfs深度*/);
	/*尝试吃子*/
static void draughts_eat_try(int board_sub[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]/*被移动棋盘*/, int eat_dir[DRAUGHTS_CHESS_NUM][2]/*记录可以发生吃子的棋子坐标*/, int num_eat, int depth/*当前搜索深度*/, double *score_before/*上一层分数*/, double *score_now/*当前层分数*/);
	/*搜索吃否存在有吃必吃，返回当前局面能发生吃子的棋子个数*/
static int draughts_eat_check(int board_checked[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]/*被检查棋盘*/, int dir_eat[DRAUGHTS_CHESS_NUM][2]/*记录可以发生吃子的棋子坐标*/);
	/*TURN函数主体部分*/
static void draughts_turn_main(int board_now[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]/*被执行棋盘*/, int depth/*当前搜索深度*/, double *score_before/*上一层分数*/);

//独立函数部分

	/*调试用，将棋盘可视化输出*/
//static void draughts_board_print(int board_printed[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]/*被输出棋盘*/) {
//	for (int i = 0; i < DRAUGHTS_BOARD_SIZE; i++) {
//		for (int j = 0; j < DRAUGHTS_BOARD_SIZE; j++) {
//			switch (board_printed[i][j]) {
//			case DRAUGHTS_EMPTY:
//				printf("..");
//				break;
//			case DRAUGHTS_WHITE:
//				printf("ww");
//				break;
//			case DRAUGHTS_WHITE_KING:
//				printf("WW");
//				break;
//			case DRAUGHTS_BLACK:
//				printf("bb");
//				break;
//			case DRAUGHTS_BLACK_KING:
//				printf("BB");
//				break;
//			}
//		}
//		printf("\n");
//	}
//}
	/*检查是否位于界内*/
static int draughts_approved_bound(int x/*横坐标*/, int y/*纵坐标*/) {
	return (x >= 0 && x < DRAUGHTS_BOARD_SIZE && y >= 0 && y < DRAUGHTS_BOARD_SIZE);
}
/*检查是否符合dfs方法的要求*/
static int draughts_approved_dfs(struct draughts_dfs_reference *reference, int mid_x, int mid_y, int jump_x, int jump_y) {
	return (reference->flag_eat[mid_x][mid_y] == 0 && draughts_approved_bound(jump_x, jump_y) && reference->board_reference[mid_x][mid_y] < DRAUGHTS_EMPTY && (reference->board_reference[jump_x][jump_y] == DRAUGHTS_EMPTY || (jump_x == reference->dir_now[0][0] && jump_y == reference->dir_now[0][1])));
}//是否未被标记、位于界内，发生吃子，目标格为空(目标格为原棋子所在格是目标格为空的一个特例)
	/*将己方走子指令输出*/
static void draughts_print_command() {
	if (Draughts_turn_my == DRAUGHTS_WHITE) {
		for (int i = 0; i < Input_my.step; i++) {
			Input_my.dir[i][0] = 7 - Input_my.dir[i][0];
			Input_my.dir[i][1] = 7 - Input_my.dir[i][1];
		}
	}//若我方执白，需进行坐标旋转
	printf("%d", Input_my.step);
	for (int i = 0; i < Input_my.step; i++) printf(" %d,%d", Input_my.dir[i][0], Input_my.dir[i][1]);
	printf("\n");
	fflush(stdout);
}
/*旋转棋盘和执子方*/
static void draughts_board_spin(int board_spined[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]/*被旋转棋盘*/) {
	for (int i = 0; i < DRAUGHTS_BOARD_SIZE / 2; i++) {
		for (int j = 0; j < DRAUGHTS_BOARD_SIZE; j++) {
			int temp = board_spined[i][j];
			board_spined[i][j] = 0 - board_spined[DRAUGHTS_BOARD_SIZE - i - 1][DRAUGHTS_BOARD_SIZE - j - 1];
			board_spined[DRAUGHTS_BOARD_SIZE - i - 1][DRAUGHTS_BOARD_SIZE - j - 1] = 0 - temp;
		}//交换两个位置的棋子并变换棋子归属方
	}
}
/*拷贝棋盘副本*/
static void draughts_board_copy(int board_sub[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]/*参考棋盘*/, int board_copy[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]/*需被拷贝棋盘*/) {
	for (int i = 0; i < DRAUGHTS_BOARD_SIZE; i++)
		for (int j = 0; j < DRAUGHTS_BOARD_SIZE; j++)board_copy[i][j] = board_sub[i][j];
}//复制一份旧棋盘到棋盘的拷贝当中
	/*遍历棋盘，返回棋盘中双方棋子的个数以及我方棋子的坐标，顺便进行升王判断*/
static void draughts_board_view(int board_viewed[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]/*参考棋盘*/, struct draughts_info *board_info/*棋盘信息*/) {
	for (int i = 0; i < DRAUGHTS_BOARD_SIZE; i++) {
		for (int j = 0; j < DRAUGHTS_BOARD_SIZE; j++) {
			if (board_viewed[i][j] < DRAUGHTS_EMPTY) {//白棋为负值，故直接判断小于空值
				board_info->dir_white[board_info->num_white][0] = i;
				board_info->dir_white[board_info->num_white][1] = j;
				if (i == 7) {//进行升王判断
					board_viewed[i][j] = DRAUGHTS_WHITE_KING;
					board_info->dir_white[board_info->num_white][2] = 1;
				}
				board_info->num_white++;
			}
			else if (board_viewed[i][j] > DRAUGHTS_EMPTY) {//黑棋为正值，故直接判断大于空值
				board_info->dir_black[board_info->num_black][0] = i;
				board_info->dir_black[board_info->num_black][1] = j;
				if (i == 0) {//进行升王判断
					board_viewed[i][j] = DRAUGHTS_BLACK_KING;
					board_info->dir_black[board_info->num_black][2] = 1;
				}
				board_info->num_black++;
			}
		}
	}
}

//估值函数部分

	/*估值模块一：列队阵型V3*/
static int draughts_score_a(int board_a[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]/*参考棋盘*/, int times/*第几次执行*/, int num_chess/*棋子个数*/, int dir_chess[DRAUGHTS_CHESS_NUM][3]/*棋子坐标*/) {
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
				dir_now[0] += Draughts_dir_move[times][0];//向前方延展
				dir_now[1] += Draughts_dir_move[times][1];
			} while (board_marked[dir_now[0]][dir_now[1]] == 0 && draughts_approved_bound(dir_now[0], dir_now[1]) && board_a[dir_now[0]][dir_now[1]] > DRAUGHTS_EMPTY);//直到前方无棋子
			dir_now[0] = dir_chess[i][0];//当前坐标返回至初始读取坐标
			dir_now[1] = dir_chess[i][1];
			board_marked[dir_now[0]][dir_now[1]] = 0;//取消标记
			len--;
			do {
				len++;
				board_marked[dir_now[0]][dir_now[1]] = 1;//将其标记
				dir_now[0] -= Draughts_dir_move[times][0];//向后方延展
				dir_now[1] -= Draughts_dir_move[times][1];
			} while (board_marked[dir_now[0]][dir_now[1]] == 0 && draughts_approved_bound(dir_now[0], dir_now[1]) && board_a[dir_now[0]][dir_now[1]] > DRAUGHTS_EMPTY);//直到后方无棋子
			board_marked[dir_chess[i][0]][dir_chess[i][1]] = 1;//最后保证确实进行了标记
			if (len >= 3) value_a += len - 2;
		}
	}
	return value_a;
}
/*估值模块二：基本分数V0、高度和区位分数V1、三角阵型V2*/
static void draughts_score_b(int board_b[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]/*参考棋盘*/, int num_chess/*棋子个数*/, int dir_chess[DRAUGHTS_CHESS_NUM][3]/*棋子坐标*/, int *value/*估值缓存*/) {
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
				dir_now[0] = dir_chess[i][0] - Draughts_dir_move[i][0];
				dir_now[1] = dir_chess[i][1] - Draughts_dir_move[i][1];
				if (draughts_approved_bound(dir_now[0], dir_now[1]) && board_b[dir_now[0]][dir_now[1]] < DRAUGHTS_EMPTY) flag_jump++;
				dir_now[0] = dir_chess[i][0] + Draughts_dir_move[i][0];
				dir_now[1] = dir_chess[i][1] + Draughts_dir_move[i][1];
				if (draughts_approved_bound(dir_now[0], dir_now[1]) && board_b[dir_now[0]][dir_now[1]] > DRAUGHTS_EMPTY) flag_def++;
			}
			if (flag_def == 2 && flag_jump >= 1) value[2]++;//三角阵型分数，若构成三角阵且位列前方则加分，使得棋子具有一定的阵型意识
		}
		if (dir_chess[i][0] > 1 && dir_chess[i][1] > 1 && dir_chess[i][0] < 6 && dir_chess[i][1] < 6) value[1] += 10;//棋子区位分数，中间的棋子分数高
	}
}
/*估值模块三：边线棋子V4*/
static int draughts_score_c(int board_c[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]/*参考棋盘*/) {
	int value_c = 0;
	if (board_c[1][0] == DRAUGHTS_WHITE)value_c--;
	if (board_c[2][7] == DRAUGHTS_WHITE)value_c--;
	if (board_c[6][7] == DRAUGHTS_BLACK)value_c++;
	if (board_c[7][0] == DRAUGHTS_BLACK)value_c++;//边线棋子的优势分数，距对方底线近的边线棋子威胁大，距己方底线近的边线棋子移动受到限制
	return value_c;
}
/*估值函数主体*/
static double draughts_score(int board_scored[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]/*被估值棋盘*/) {
	double score = 0;
	double value[5] = { 0 };//V[0]代表每颗棋子的基本分数，V[1]为棋子的高度和区位分数，V[2]为三角阵型分数，V[3]为列队阵型分数,V[4]为底线边棋的优势或劣势分数
	int weight[5] = { 2,2,10,15,15 };//代表V01234的权重
	struct draughts_info board_info;//记录棋盘信息
	draughts_board_view(board_scored, &board_info);
	if (board_info.num_black == 0)return -9000;
	if (board_info.num_white == 0)return 9000;//若某方棋子不存在，则直接返回正无穷或负无穷
	for (int i = 0; i < board_info.num_white; i++) {//通过旋转白棋坐标，达到化简代码的效果
		board_info.dir_white[i][0] = 7 - board_info.dir_white[i][0];
		board_info.dir_white[i][1] = 7 - board_info.dir_white[i][1];
	}
	for (int i = 0; i < 2; i++) {
		draughts_board_spin(board_scored);
		if (i == 0) {//保证这一部分只执行一次
			int value_b[3] = { 0 };
			draughts_score_b(board_scored, board_info.num_white, board_info.dir_white, value_b);
			value[0] -= value_b[0] * 0.8;//消减对手的棋子基本分数所占的权重，使程序减少以一换一的次数
			value[1] -= value_b[1];
			value[2] -= value_b[2];
		}
		value[3] -= draughts_score_a(board_scored, i, board_info.num_white, board_info.dir_white);
		draughts_board_spin(board_scored);
		if (i == 0) {//保证这一部分只执行一次
			int value_b[3] = { 0 };
			draughts_score_b(board_scored, board_info.num_black, board_info.dir_black, value_b);
			value[0] += value_b[0];
			value[1] += value_b[1];
			value[2] += value_b[2];
		}
		value[3] += draughts_score_a(board_scored, i, board_info.num_black, board_info.dir_black);
	}
	value[4] += draughts_score_c(board_scored);
	for (int i = 0; i < 5; i++)score += value[i] * weight[i];//按照权重求和
	return score;
}

//START命令部分

	/*对接受的START命令进行响应*/
void draughts_start() {
	scanf("%d", &Draughts_turn_my);
	printf("OK\n");
	fflush(stdout);
	if (Draughts_turn_my == 1)Draughts_turn_my = DRAUGHTS_BLACK;
	else Draughts_turn_my = DRAUGHTS_WHITE;//初始化执子方
}

//PLACE命令部分

	/*PLACE函数主体部分*/
static void draughts_place_main(int board_placed[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]/*被移动棋盘*/, int step/*移动路径长度*/, int dir_input[10][2]/*移动坐标*/) {
	int from_x, from_y, mid_x, mid_y, to_x, to_y;
	for (int i = 0; i < step - 1; i++) {
		from_x = dir_input[i][0];
		from_y = dir_input[i][1];
		to_x = dir_input[i + 1][0];
		to_y = dir_input[i + 1][1];
		board_placed[to_x][to_y] = board_placed[from_x][from_y];
		if (abs(from_x - to_x) == 2) {//判断走还是跳
			mid_x = (from_x + to_x) / 2;
			mid_y = (from_y + to_y) / 2;
			board_placed[mid_x][mid_y] = DRAUGHTS_EMPTY;
		}
		board_placed[from_x][from_y] = DRAUGHTS_EMPTY;
	}
}
/*对接受的PLACE命令进行响应*/
void draughts_place() {
	int step = 0;//所经过的路径长度
	int dir_input[10][2] = {};//所经过的坐标
	scanf("%d", &step);
	for (int i = 0; i < step; i++) scanf("%d,%d", &dir_input[i][0], &dir_input[i][1]);
	draughts_place_main(Draughts_board_main, step, dir_input);
}

//TURN命令部分

static void draughts_move_try(int board_moved[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE], struct draughts_command my_command, int depth, double *score_now) {
	int board_copy[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE] = { 0 };//定义拷贝棋盘
	draughts_board_copy(board_moved, board_copy);//棋盘拷贝
	draughts_place_main(board_copy, my_command.step, my_command.dir);//在拷贝棋盘中移动棋子
	if (depth == DRAUGHTS_DEPTH) {
		if (depth % 2 == 0) draughts_board_spin(board_copy);//防止修改深度导致的被估值主体错误，保证被估值主体永远是己方
		if (depth % 2 == 0)	*score_now = MIN(draughts_score(board_copy), *score_now);
		else if (depth % 2 == 1) *score_now = MAX(draughts_score(board_copy), *score_now);
		if (depth % 2 == 0) draughts_board_spin(board_copy);
		Draughts_times++;
	}
	else {
		draughts_board_spin(board_copy);
		draughts_turn_main(board_copy, depth + 1, score_now);
	}
}
static void draughts_move_search(int board_sub[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE], int depth, double *score_before, double *score_now) {
	int to_x = 0, to_y = 0;//要到达的坐标
	int time = 0;//判断枚举坐标方向
	struct draughts_info board_info;//记录棋盘信息
	struct draughts_command my_command;//定义走子命令结构体
	draughts_board_view(board_sub, &board_info);//遍历棋盘、
	for (int i = 0; i < board_info.num_black; i++) {
		if (board_info.dir_black[i][2] == 1) time = 4;//若被枚举棋子为王棋，则向4个方向枚举
		else time = 2;//否则，仅向前方枚举
		for (int j = 0; j < time; j++) {
			to_x = board_info.dir_black[i][0] + Draughts_dir_move[j][0];
			to_y = board_info.dir_black[i][1] + Draughts_dir_move[j][1];
			if (draughts_approved_bound(to_x, to_y) && board_sub[to_x][to_y] == DRAUGHTS_EMPTY) {//如果坐标合理
				my_command.dir[0][0] = board_info.dir_black[i][0];
				my_command.dir[0][1] = board_info.dir_black[i][1];
				my_command.dir[1][0] = to_x;
				my_command.dir[1][1] = to_y;
				my_command.step = 2;
				double score_temp = *score_now;//分数缓存，用于判断第一层估值是否发生改变
				draughts_move_try(board_sub, my_command, depth, score_now);
				if (depth == 1) {//记录根部移动坐标：如果深度为1，记录坐标 
					if (score_temp != *score_now) {//若改变，则代表新路径更优
						Input_my.step = 2;
						Input_my.dir[0][0] = board_info.dir_black[i][0];
						Input_my.dir[0][1] = board_info.dir_black[i][1];
						Input_my.dir[1][0] = to_x;
						Input_my.dir[1][1] = to_y;
					}
				}
				else if (depth % 2 == 0 && *score_before > *score_now) return;//ab剪枝主体部分，没错就只有这2行
				else if (depth % 2 == 1 && *score_before < *score_now) return;//你问我为什么？我怎么知道！
			}//传入得到的下一个棋盘、坐标组、深度、当前分数和上一层分数并进行模拟走子
		}
	}
}
static void draughts_eat_cmp(struct draughts_dfs_reference* reference) {
	struct draughts_command command_test;//定义走子命令结构体
	command_test.step = reference->step_max;
	for (int i = 0; i < command_test.step; i++) {
		command_test.dir[i][0] = reference->dir_max[i][0];
		command_test.dir[i][1] = reference->dir_max[i][1];
	}//结构体赋原路径
	draughts_move_try(reference->board_reference, command_test, reference->depth, &reference->score_now);//按照原路径尝试走子
	double score_temp = reference->score_now;//分数缓存，用于判断估值是否发生改变
	for (int i = 0; i < command_test.step; i++) {
		command_test.dir[i][0] = reference->dir_now[i][0];
		command_test.dir[i][1] = reference->dir_now[i][1];
	}//将新路径重新赋值给结构体
	draughts_move_try(reference->board_reference, command_test, reference->depth, &reference->score_now);//按照原路径尝试走子
	if (reference->depth % 2 == 1) reference->score_now = MAX(score_temp, reference->score_now);
	else if (reference->depth % 2 == 0) reference->score_now = MIN(score_temp, reference->score_now);
	if (score_temp != reference->score_now) {//如果分数发生改变，证明新路径最优，进行替换
		for (int i = 0; i < reference->step_max; i++) {
			reference->dir_max[i][0] = reference->dir_now[i][0];
			reference->dir_max[i][1] = reference->dir_now[i][1];
		}
	}
}
static void draughts_eat_dfs(struct draughts_dfs_reference* reference, int step) {
	int from_x = reference->dir_now[step - 1][0];//起始坐标
	int from_y = reference->dir_now[step - 1][1];
	int jump_x, jump_y;//目标坐标
	int mid_x, mid_y;//被吃子的坐标
	int flag = 0;//标记是否能够吃子
	for (int j = 0; j < 4; j++) {//枚举4个方向是否可吃
		jump_x = from_x + Draughts_dir_eat[j][0];
		jump_y = from_y + Draughts_dir_eat[j][1];
		mid_x = (from_x + jump_x) / 2;
		mid_y = (from_y + jump_y) / 2;//若该位置没被越过、在范围内、能发生吃子、可为空可放子
		if (draughts_approved_dfs(reference, mid_x, mid_y, jump_x, jump_y)) {
			reference->flag_eat[mid_x][mid_y] = 1;//标记该位置已被越过
			flag = 1;//标记当前棋子能够发生吃子
			reference->dir_now[step][0] = jump_x;
			reference->dir_now[step][1] = jump_y;//记录这一步的坐标
			draughts_eat_dfs(reference, step + 1);//递归调用该函数再次搜索
			reference->flag_eat[mid_x][mid_y] = 0;//取消该位置的标记
		}
	}
	if (flag == 0) {//如果未发生吃子
		if (reference->step_max < step) {//判断是否为最长路径
			reference->step_max = step;//若原路径短，则进行路径替换
			for (int i = 0; i < reference->step_max; i++) {
				reference->dir_max[i][0] = reference->dir_now[i][0];
				reference->dir_max[i][1] = reference->dir_now[i][1];
			}
		}
		else if (reference->step_max == step) draughts_eat_cmp(reference);//若原路径与现路径长度相同，通过模拟走棋计算分数判断应取何
	}
}
static void draughts_eat_try(int board_sub[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE], int eat_dir[DRAUGHTS_CHESS_NUM][2], int num_eat, int depth, double *score_before, double *score_now) {
	struct draughts_dfs_reference reference;//定义dfs专用结构体参考
	draughts_board_copy(board_sub, reference.board_reference);//拷贝棋盘至结构体参考中
	reference.depth = depth;//复制各项参数
	reference.score_before = *score_before;
	reference.score_now = *score_now;
	for (int i = 0; i < num_eat; i++) {
		reference.dir_now[0][0] = eat_dir[i][0];
		reference.dir_now[0][1] = eat_dir[i][1];
		draughts_eat_dfs(&reference, 1);
	}
	*score_now = reference.score_now;//从结构体中取出该层分数
	draughts_place_main(reference.board_reference, reference.step_max, reference.dir_max);//对拷贝棋盘进行走子
	if (depth == 1) {//如果深度为1，则当前最大坐标即为走子坐标
		Input_my.step = reference.step_max;//记录经过格数
		for (int i = 0; i < Input_my.step; i++) {
			Input_my.dir[i][0] = reference.dir_max[i][0];
			Input_my.dir[i][1] = reference.dir_max[i][1];
		}
	}
	else if (depth == DRAUGHTS_DEPTH) {//若深度到达，进行走子并估值返回
		if (depth % 2 == 0) draughts_board_spin(reference.board_reference);//防止修改深度导致的被估值主体错误，保证被估值主体永远是己方(黑方)
		if (depth % 2 == 0)	*score_now = MIN(draughts_score(reference.board_reference), *score_now);
		else if (depth % 2 == 1) *score_now = MAX(draughts_score(reference.board_reference), *score_now);
		if (depth % 2 == 0) draughts_board_spin(reference.board_reference);
		Draughts_times++;
	}
	else {//若深度未到且不为1，则进行下一层搜索
		draughts_board_spin(reference.board_reference);//旋转棋盘，保证永远对黑棋判断
		draughts_turn_main(reference.board_reference, depth + 1, score_now);
	}
}
static int draughts_eat_check(int board_checked[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE], int dir_eat[DRAUGHTS_CHESS_NUM][2]) {
	int to_x, to_y;//被吃子坐标
	int jump_x, jump_y;//目标格坐标	
	int num = 0;//记录可以发生吃子的棋子数量，若为0则代表无法发生吃子
	struct draughts_info board_info;//记录棋盘信息
	draughts_board_view(board_checked, &board_info);//遍历棋盘
	for (int i = 0; i < board_info.num_black; i++) {
		for (int j = 0; j < 4; j++) {//向4个方向枚举
			to_x = board_info.dir_black[i][0] + Draughts_dir_move[j][0];
			to_y = board_info.dir_black[i][1] + Draughts_dir_move[j][1];
			jump_x = board_info.dir_black[i][0] + Draughts_dir_eat[j][0];
			jump_y = board_info.dir_black[i][1] + Draughts_dir_eat[j][1];//获得坐标
			if (draughts_approved_bound(jump_x, jump_y) && board_checked[to_x][to_y] < DRAUGHTS_EMPTY && board_checked[jump_x][jump_y] == DRAUGHTS_EMPTY) {
				dir_eat[num][0] = board_info.dir_black[i][0];
				dir_eat[num][1] = board_info.dir_black[i][1];//记录可以发生吃子的棋子的坐标
				num++;
				break;
			}
		}
	}
	return num;
}
static void draughts_turn_main(int board_now[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE], int depth, double *score_before) {
	double score_now = 0;//新建该层分数
	if (depth % 2 == 0) score_now = 10000;
	else score_now = -10000;//初始化分数
	int dir_eat[DRAUGHTS_CHESS_NUM][2] = { 0 };//用于向内层传递可以发生吃子的棋子坐标
	int num_eat = draughts_eat_check(board_now, dir_eat);//记录可以发送吃子的棋子个数
	if (num_eat != 0) draughts_eat_try(board_now, dir_eat, num_eat, depth, score_before, &score_now);//若可以吃子，执行吃子搜索
	else draughts_move_search(board_now, depth, score_before, &score_now);//无吃子，执行走子搜索
	if (depth % 2 == 0)	*score_before = MAX(*score_before, score_now);//进行minimax取值
	else if (depth % 2 == 1) *score_before = MIN(*score_before, score_now);
}
/*对接受的TURN命令进行响应*/
void draughts_turn() {
	if (Draughts_turn_my == DRAUGHTS_WHITE)draughts_board_spin(Draughts_board_main);//若我方执白，旋转棋盘保证程序认为我方执黑
	double score_temp = 10000;//该值无实际意义，仅用于填充缺少的参数
	Draughts_times = 0;//调试用，初始化分支总数
	draughts_turn_main(Draughts_board_main, 1, &score_temp);//进行搜索并返回坐标至全局结构体中
	draughts_place_main(Draughts_board_main, Input_my.step, Input_my.dir);//从全局结构体中取出坐标进行走子
	draughts_print_command();//输出指令
	//printf("%lf %ld\n", score_temp, Draughts_times);//调试用，输出分支和估值
	if (Draughts_turn_my == DRAUGHTS_WHITE)draughts_board_spin(Draughts_board_main);
}