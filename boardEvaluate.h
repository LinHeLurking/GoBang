//
// Created by Nine_yota on 2018-09-29.
//
#include "icld.h"

#ifndef GOBANG_BOARDEVALUATE_H
#define GOBANG_BOARDEVALUATE_H

int winner_check();

void build_trie();

void trie_test();

int grade_estimate(int player_side);

void generate_possible_pos(drop_choice *drop_choice1, int *num);


drop_choice alpha_beta_dfs(int search_player_side, int search_depth);



#endif //GOBANG_BOARDEVALUATE_H
