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

int64_t pos_estimate(int i, int j, int player_side);

int winner_check();

int64_t grade_estimate(int32_t player_side);

void update_line_type_row(int row_index, int player_side);

void update_line_type_col(int col_index, int player_side);

void update_line_type_oblique_sum(int oblique_sum_index, int player_side);

void update_line_type_oblique_delta(int oblique_delta_index, int player_side);

void update_grade(int i, int j);

uint8_t is_ban();

void evaluate_init();

//this function is used for 011101 <-> 01110
void update_type(uint8_t type_array[], uint8_t type);

#endif //GOBANG_BOARDEVALUATE_H
