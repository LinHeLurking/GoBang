//
// Created by Nine_yota on 2018-10-07.
//

#include "statusSearch.h"

extern boardStatus status;
extern boardStatus dfs_status;
extern unsigned long long hash_key[BOARD_SIZE][BOARD_SIZE];


/*
 *  DFS starts here
 * */
#define MAX_POS 225

inline void generate_possible_pos(drop_choice *drop_choice1, int *num, int search_player_side) {
    *num = 0;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (dfs_status.board[i][j] == VOID) {
                //skip useless points as many as possible
                if (dfs_status.steps < 6) {
                    if (!has_neighbor(i, j, 1))continue;
                } else if (!has_neighbor(i, j, 2)) continue;


                //long long my_original_grade_estimate = pos_estimate(i, j, search_player_side);
                //long long opponent_original_grade_estimate = pos_estimate(i, j, 0 - search_player_side);
                drop_choice1[*num].i = i;
                drop_choice1[*num].j = j;

                dfs_add_piece(i, j, search_player_side);

                long long my_new_grade_estimate = grade_estimate(search_player_side);
                long long opponent_new_grade_estimate = grade_estimate(0 - search_player_side);
                //long long my_delta = (my_new_grade_estimate - my_original_grade_estimate) * search_player_side;
                //long long opponent_delta =
                //        (opponent_new_grade_estimate - opponent_original_grade_estimate) * (0 - search_player_side);
                //drop_choice1[*num].grade_estimate = my_delta - 1.1 * opponent_delta;
                drop_choice1[*num].grade_estimate = my_new_grade_estimate + opponent_new_grade_estimate;

                dfs_add_piece(i, j, VOID);

                ++(*num);
            }
        }
    }
    choice_sort(drop_choice1, *num, search_player_side);
    switch (dfs_status.steps) {
        case 2:
            *num = 9;
            break;
        case 3:
            *num = 16;
            break;
        default:
            *num = int_min(*num, 50);
            break;
    }
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
                if (!(prune_cnt % 1000)) {
                    printf("prune_cnt: %d\n", prune_cnt);
                    printf("dfs depth: %d\n", DFS_DEPTH - search_depth);
                }
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
                result = tmp;
            }
            dfs_add_piece(tmp.i, tmp.j, VOID);
            beta = min(beta, result.grade);
            if (beta < alpha) {
#ifdef PRUNE_DEBUG
                ++prune_cnt;
                if (!(prune_cnt % 1000)) {
                    printf("prune_cnt: %d\n", prune_cnt);
                    printf("dfs depth: %d\n", DFS_DEPTH - search_depth);
                }
#endif
                return result;
            }
        }
        return result;
    }
}

inline int has_neighbor(int i, int j, int wid) {
    for (int _i = i - wid; _i <= i + wid; ++_i) {
        if (_i < 0 || _i >= BOARD_SIZE)continue;
        for (int _j = j - wid; _j <= j + wid; ++_j) {
            if (_j < 0 || _j >= BOARD_SIZE)continue;
            if (dfs_status.board[_i][_j] != VOID && _i != i && _j != j) {
                return 1;
            }
        }
    }
    return 0;
}

