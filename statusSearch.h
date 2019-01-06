//
// Created by Nine_yota on 2018-10-07.
//


#ifndef GOBANG_STATUSSEARCH_H
#define GOBANG_STATUSSEARCH_H
#include "icld.h"
#include "boardEvaluate.h"
#include "mathFunc.h"
#include "Zoribrist_hash.h"
#include "customTypes.h"

void generate_possible_pos(drop_choice *drop_choice1, int *num, int search_player_side);

drop_choice alpha_beta_dfs(int search_player_side, uint32_t search_depth, int64_t alpha, int64_t beta);

drop_choice deepening_search(int search_player_side, int depth_bound);

int has_neighbor(int i, int j, int wid, int cnt);

//call these initialise function sright after program starts
void search_init();
void time_init();

//standarise the grade to fit with the scale of 8^remaining_depth
int64_t grade_standardise(int64_t grade);

void time_display(uint32_t depth, long long int dfs_grade);

int duration_check();

#endif //GOBANG_STATUSSEARCH_H
