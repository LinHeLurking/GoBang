//
// Created by Nine_yota on 2018-09-29.
//
#include "icld.h"
#include "mathFunction.h"

#ifndef GOBANG_BOARDEVALUATE_H
#define GOBANG_BOARDEVALUATE_H

int winner_check();

int get_status(int player_side, int *array_to_querry, int array_len, int index);

void build_trie();

void trie_test();

int grade_estimate(int player_side);

void generate_possible_pos(int pos_i[], int pos_j[], int *num);


int alpha_beta_dfs(int my_player_side, int search_player_side, int search_depth, int alpha, int beta);

#endif //GOBANG_BOARDEVALUATE_H
