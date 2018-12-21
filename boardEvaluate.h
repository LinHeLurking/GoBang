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

long long pos_estimate(int i, int j, int player_side);

int winner_check();

long long grade_estimate(int player_side);


void update_line_grade_row(int row_index, int player_side);

void update_line_grade_col(int col_index, int player_side);

void update_line_grade_oblique_sum(int oblique_sum_index, int player_side);

void update_line_grade_oblique_delta(int oblique_delta_index, int player_side);

void update_grade(int i, int j);

void pattern_parse(long long grade);

int8_t is_ban();

#endif //GOBANG_BOARDEVALUATE_H
