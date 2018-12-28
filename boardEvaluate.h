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

int64_t pos_estimate(int i, int j, int8_t player_side);

int winner_check();

int64_t grade_estimate(int32_t player_side);

void update_line_type_row(int row_index);

void update_line_type_col(int col_index);

void update_line_type_oblique_sum(int oblique_sum_index);

void update_line_type_oblique_delta(int oblique_delta_index);

void update_grade(int i, int j);

bool is_ban();

void evaluate_init();

void update_type(uint8_t type_array[], uint8_t type);

void ban_clear();

void ban_detect();

void increment_void(int64_t v[][PATTERN_TYPES + 5], __AC_node *p, int32_t index);

int64_t type_shift(int8_t type, int8_t player_side);

#endif //GOBANG_BOARDEVALUATE_H