//
// Created by Nine_yota on 2018-09-29.
//


#ifndef GOBANG_STATUSBOARD_H
#define GOBANG_STATUSBOARD_H
#include "icld.h"
#include "AC_Automachine.h"
#include "Zoribrist_hash.h"
#include "mathFunc.h"
typedef struct {
    //these are used to record the board status
    int board[BOARD_SIZE][BOARD_SIZE];
    int oblique_line_sum[2 * BOARD_SIZE][BOARD_SIZE];
    int oblique_line_delta[2 * BOARD_SIZE][BOARD_SIZE];

    //followings are arrays for DIRECT grade estimate
    //0 for black and 1 for white orz
    long long row_grade[2][BOARD_SIZE];
    long long col_grade[2][BOARD_SIZE];
    long long oblique_sum_grade[2][2 * BOARD_SIZE + 5];
    long long oblique_delta_grade[2][2 * BOARD_SIZE + 5];
    long long total_grade[2];

    //these are used to record the last place and the total steps
    int steps;

} boardStatus;

typedef struct {
    int i, j, player;
} drop_record;

int add_piece(int i, int j, int piece_color);

int dfs_add_piece(int i, int j, int piece_color);

void update_line_grade_row(int row_index, int player_side);

void update_line_grade_col(int col_index, int player_side);

void update_line_grade_oblique_sum(int oblique_sum_index, int player_side);

void update_line_grade_oblique_delta(int oblique_delta_index, int player_side);

void update_grade(int i, int j);

void status_init();

void grade_unique(long long grade);


#endif //GOBANG_STATUSBOARD_H
