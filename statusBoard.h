//
// Created by Nine_yota on 2018-09-29.
//
#include "icld.h"

#ifndef GOBANG_STATUSBOARD_H
#define GOBANG_STATUSBOARD_H

typedef struct {
    int board[BOARD_SIZE][BOARD_SIZE];
    int oblique_lines_1[2 * BOARD_SIZE][BOARD_SIZE];
    int oblique_lines_2[2 * BOARD_SIZE][BOARD_SIZE];
    int last_i, last_j;
    int white_steps,black_steps;
} boardStatus;

int add_piece(int i, int j, int pc);

int dfs_add_piece(int i, int j, int piece_color);

void status_init();


#endif //GOBANG_STATUSBOARD_H
