//
// Created by Nine_yota on 2018-09-29.
//
#include "icld.h"
#include "mathFunction.h"

#ifndef GOBANG_BOARDEVALUATE_H
#define GOBANG_BOARDEVALUATE_H

int winner_check();

int get_status(int cur_color, int *array_to_querry, int array_len, int index);

void build_trie();

int grade_estimate();

#endif //GOBANG_BOARDEVALUATE_H
