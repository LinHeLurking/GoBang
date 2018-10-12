//
// Created by Nine_yota on 2018-09-29.
//
#include "icld.h"
#include "statusBoard.h"
#include "mathFunc.h"
#include "drawBoard.h"
#ifndef GOBANG_BOARDEVALUATE_H
#define GOBANG_BOARDEVALUATE_H


int pos_estimate(int i, int j, int player_side);

int winner_check();

long long grade_estimate(int player_side);



#endif //GOBANG_BOARDEVALUATE_H
