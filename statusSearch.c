//
// Created by Nine_yota on 2018-10-07.
//

#include "statusSearch.h"

extern boardStatus status;
extern boardStatus dfs_status;

/*
 *  DFS starts here
 * */
#define MAX_POS 225

void generate_possible_pos(drop_choice *drop_choice1, int *num, int search_player_side) {
    *num = 0;

    //TODO: change the generate method
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (dfs_status.board[i][j] == VOID) {
                long long my_original_grade_estimate = pos_estimate(i, j, search_player_side);
                long long opponent_original_grade_estimate = pos_estimate(i, j, 0 - search_player_side);
                drop_choice1[*num].i = i;
                drop_choice1[*num].j = j;
                dfs_add_piece(i, j, search_player_side);
                //TODO: modify the grade estimate function for empty positions
                long long my_new_grade_estimate = pos_estimate(i, j, search_player_side);
                long long opponent_new_grade_estimate = pos_estimate(i, j, 0 - search_player_side);
                long long my_delta = (my_new_grade_estimate - my_original_grade_estimate) * search_player_side;
                long long opponent_delta =
                        (opponent_new_grade_estimate - opponent_original_grade_estimate) * (0 - search_player_side);
                drop_choice1[*num].grade_estimate = my_delta - opponent_delta;
                dfs_add_piece(i, j, VOID);
                ++(*num);
            }
        }
    }
    choice_sort(drop_choice1, *num);
    *num = min(*num, 100);
}

#ifdef PRUNE_DEBUG
int prune_cnt = 0;
#endif

//remember that the larger the grade is, the better the status is for WHITE. and vice versa.
//alpha is the upper bound and beta is the lower bound.
drop_choice alpha_beta_dfs(int search_player_side, int search_depth, long long alpha, long long beta) {
    drop_choice result;
    result.i = result.j = 0;
    result.grade = 0;

    if (search_depth == 0) {
        result.grade = grade_estimate(search_player_side) + grade_estimate(0 - search_player_side);
        return result;
    }
    drop_choice drop_choice1[MAX_POS];
    int pos_num = 0;

    //generate proper places to drop a piece
    generate_possible_pos(drop_choice1, &pos_num, search_player_side);

    //traverse the proper places
    if (search_player_side == WHITE) {
#ifdef DFS_BOARD_DEBUG
        printf("dfs board:\n");
        dfs_output_board();
        GRADE_DEBUG
#endif
        result.grade = 0 - INF;
        for (int k = 0; k < pos_num; ++k) {
            dfs_add_piece(drop_choice1[k].i, drop_choice1[k].j, WHITE);
            drop_choice tmp;
            tmp = alpha_beta_dfs(0 - search_player_side, search_depth - 1, alpha, beta);
            tmp.i = drop_choice1[k].i, tmp.j = drop_choice1[k].j;
            if (tmp.grade > result.grade) {
                result = tmp;
            }
            dfs_add_piece(tmp.i, tmp.j, VOID);
            alpha = max(alpha, result.grade);
            if (beta < alpha) {
#ifdef PRUNE_DEBUG
                ++prune_cnt;
                if (!(prune_cnt % 100))printf("prune_cnt: %d\n", prune_cnt);
#endif
                return result;
            }
        }
        return result;
    } else {
#ifdef DFS_BOARD_DEBUG
        printf("dfs board:\n");
        dfs_output_board();
        GRADE_DEBUG
#endif
        result.grade = INF;
        for (int k = 0; k < pos_num; ++k) {
            dfs_add_piece(drop_choice1[k].i, drop_choice1[k].j, BLACK);
            drop_choice tmp;
            tmp = alpha_beta_dfs(0 - search_player_side, search_depth - 1, alpha, beta);
            tmp.i = drop_choice1[k].i, tmp.j = drop_choice1[k].j;
            if (tmp.grade < result.grade) {
                ++prune_cnt;
                result = tmp;
            }
            dfs_add_piece(tmp.i, tmp.j, VOID);
            beta = min(beta, result.grade);
            if (beta < alpha) {
#ifdef PRUNE_DEBUG
                ++prune_cnt;
                if (!(prune_cnt % 100))printf("prune_cnt: %d\n", prune_cnt);
#endif
                return result;
            }
        }
        return result;
    }
}

