//
// Created by Nine_yota on 2018-09-29.
//
#include "icld.h"
#include "statusBoard.h"

#include "drawBoard.h"
#ifndef GOBANG_BOARDEVALUATE_H
#define GOBANG_BOARDEVALUATE_H


typedef struct {
    int trans[3];
    int grade;
    int fail;
} trie;

int winner_check();

int grade_estimate(int player_side);

void AC_build();





#endif //GOBANG_BOARDEVALUATE_H
