//
// Created by Nine_yota on 2018-10-07.
//
#include "icld.h"
#include "boardEvaluate.h"
#ifndef GOBANG_STATUSSEARCH_H
#define GOBANG_STATUSSEARCH_H

void generate_possible_pos(drop_choice *drop_choice1, int *num);

drop_choice alpha_beta_dfs(int search_player_side, int search_depth);

#endif //GOBANG_STATUSSEARCH_H
