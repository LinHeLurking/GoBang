//
// Created by Nine_yota on 2018-10-07.
//


#ifndef GOBANG_STATUSSEARCH_H
#define GOBANG_STATUSSEARCH_H
#include "icld.h"
#include "boardEvaluate.h"
#include "mathFunc.h"
#include "Zoribrist_hash.h"

void generate_possible_pos(drop_choice *drop_choice1, int *num, int search_player_side);

drop_choice alpha_beta_dfs(int search_player_side, int search_depth, long long alpha, long long beta);

int has_neighbor(int i, int j, int wid);

#endif //GOBANG_STATUSSEARCH_H
