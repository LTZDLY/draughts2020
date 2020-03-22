#include"Draughts2020.h"

//main函数部分

	/*循环主体*/
int draughts_loop() {
	char command[10] = {};//录入命令
	scanf("%s", command);
	if (strcmp(command, "START") == 0) draughts_start();
	else if (strcmp(command, "PLACE") == 0) draughts_place();
	else if (strcmp(command, "TURN") == 0) draughts_turn();
	else if (strcmp(command, "END") == 0) return 1;
	return 0;//返回0代表正常循环，返回1代表结束循环
}
int main() {
	while (1 - draughts_loop())/*draughts_board_print(Draughts_board_main)*/;
	return 0;
}