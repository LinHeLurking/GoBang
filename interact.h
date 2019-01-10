//
// Created by Nine_yota on 2018-09-29.
//


#ifndef GOBANG_INTERACT_H
#define GOBANG_INTERACT_H

#include "icld.h"
#include "drawBoard.h"
#include "statusBoard.h"
#include "boardEvaluate.h"
#include "statusSearch.h"

void play();

void human_vs_human();

void human_vs_computer();

void computer_vs_computer();

int read_pos(int *i, int *j);

void round_announcement(int player_side);

void throw_interact_error(int to_continue);

void print_line(int cnt, int with_a_newline);

#endif //GOBANG_INTERACT_H
