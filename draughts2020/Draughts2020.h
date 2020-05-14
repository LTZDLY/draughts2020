#pragma once
#ifndef DRAUGHTS2020_H
#define DRAUGHTS2020_H

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
//独立函数部分

/*调试用，将棋盘可视化输出*/
//static void draughts_board_print(int board_printed[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]/*被输出棋盘*/)
/*检查是否位于界内*/
static int draughts_approved_bound(int x/*横坐标*/, int y/*纵坐标*/);
/*检查是否符合dfs方法的要求*/
static int draughts_approved_dfs(struct draughts_dfs_reference *reference, int mid_x, int mid_y, int jump_x, int jump_y);
/*将己方走子指令输出*/
static void draughts_print_command();
/*旋转棋盘和执子方*/
static void draughts_board_spin(int board_spined[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]/*被旋转棋盘*/);
/*拷贝棋盘副本*/
static void draughts_board_copy(int board_sub[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]/*参考棋盘*/, int board_copy[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]/*需被拷贝棋盘*/);
/*遍历棋盘，返回棋盘中双方棋子的个数以及我方棋子的坐标，顺便进行升王判断*/
static void draughts_board_view(int board_viewed[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]/*参考棋盘*/, struct draughts_info *board_info/*棋盘信息*/);

//估值函数部分

/*估值模块一：列队阵型V3*/
static int draughts_score_a(int board_a[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]/*参考棋盘*/, int times/*第几次执行*/, int num_chess/*棋子个数*/, int dir_chess[DRAUGHTS_CHESS_NUM][3]/*棋子坐标*/);
/*估值模块二：基本分数V0、高度和区位分数V1、三角阵型V2*/
static void draughts_score_b(int board_b[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]/*参考棋盘*/, int num_chess/*棋子个数*/, int dir_chess[DRAUGHTS_CHESS_NUM][3]/*棋子坐标*/, int *value/*估值缓存*/);
/*估值模块三：边线棋子V4*/
static int draughts_score_c(int board_c[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]/*参考棋盘*/);
/*估值函数主体*/
static double draughts_score(int board_scored[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]/*被估值棋盘*/);

//START命令部分

/*对接受的START命令进行响应*/
void draughts_start();

//PLACE命令部分

/*PLACE函数主体部分*/
static void draughts_place_main(int board_placed[DRAUGHTS_BOARD_SIZE][DRAUGHTS_BOARD_SIZE]/*被移动棋盘*/, int step/*移动路径长度*/, int dir_input[10][2]/*移动坐标*/);
/*对接受的PLACE命令进行响应*/
void draughts_place();

//TURN命令部分

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
/*对接受的TURN命令进行响应*/
void draughts_turn();

#endif // DRAUGHTS2020_H