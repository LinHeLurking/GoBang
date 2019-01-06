//
// Created by Nine_yota on 2018-09-29.
//

#ifndef GOBANG_BOARDEVALUATE_H
#define GOBANG_BOARDEVALUATE_H

#include "icld.h"
#include "statusBoard.h"
#include "mathFunc.h"
#include "drawBoard.h"
#include "Zoribrist_hash.h"

enum {
    row, col, oblique_sum, oblique_delta
};

long long int pos_estimate(int i, int j, int player_side);

int winner_check();

long long int grade_estimate(int player_side);

void update_line_type_row(int row_index);

void update_line_type_col(int col_index);

void update_line_type_oblique_sum(int oblique_sum_index);

void update_line_type_oblique_delta(int oblique_delta_index);

void update_grade(int i, int j);

bool is_ban();

void evaluate_init();

void update_type(int *type_array, int type);

void ban_clear();

void ban_detect();

void increment_void(long long v[][PATTERN_TYPES+5], __AC_node *p, int index);

long long int type_shift(int type, int player_side);

long long int last_bias_generator(int player_side);

#endif //GOBANG_BOARDEVALUATE_H