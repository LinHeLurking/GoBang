//
// Created by Nine_yota on 2018-10-07.
//

#include "statusSearch.h"

extern boardStatus status;
extern boardStatus dfs_status;
extern long long cache_total_grade[CACHE_SIZE];
extern int subtree_height[CACHE_SIZE];
extern int found_at_step[CACHE_SIZE];
extern unsigned long long real_hash[CACHE_SIZE];
extern unsigned long long hash;


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
                if (dfs_status.steps < 2) {
                    if (!has_neighbor(i, j, 1, 1)) continue;
                } else if (dfs_status.steps < 3) {
                    if (!has_neighbor(i, j, 1, 2)) continue;
                } else {
                    if (!has_neighbor(i, j, 2, 1)) continue;
                }

                dfs_add_piece(i, j, search_player_side);
                drop_choice1[*num].i = i;
                drop_choice1[*num].j = j;

                //if (pos_estimate(i, j, search_player_side) >= FIVE_GRADE)
                //break;
                //above is a bad example: if you need break, drop a void piece first!!!
                //todo: more conjecture prune needed.

                if (pos_estimate(i, j, search_player_side) >= FIVE_GRADE) {
                    dfs_add_piece(i, j, VOID);
                    break;
                }

                long long my_new_grade_estimate = grade_estimate(search_player_side);
                long long opponent_new_grade_estimate = grade_estimate(0 - search_player_side);
                drop_choice1[*num].grade_estimate = my_new_grade_estimate + opponent_new_grade_estimate;
                dfs_add_piece(i, j, VOID);
                ++(*num);
            }
        }
    }
    choice_sort(drop_choice1, *num, search_player_side);
    *num = int_min(*num, 50);
}

#ifdef PRUNE_DEBUG
int prune_cnt = 0;
#endif

//remember that the larger the grade is, the better the status is for WHITE. and vice versa.
//alpha is the upper bound and beta is the lower bound.
drop_choice alpha_beta_dfs(int search_player_side, int search_depth, long long alpha, long long beta) {
    drop_choice result;
    //result.i = result.j = 0;
    //result.grade = 0;
    result.player = search_player_side;

    if (search_depth == 0) {
        // it doesn't matter what the position is in the very bottom.
        result.grade = grade_estimate(search_player_side) + grade_estimate(-search_player_side);
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
        drop_choice tmp;
        for (int k = 0; k < pos_num; ++k) {
            dfs_add_piece(drop_choice1[k].i, drop_choice1[k].j, WHITE);

            if (real_hash[HASH] == hash && subtree_height[HASH] >= search_depth) {
                tmp.grade = cache_total_grade[HASH];
            } else {
                tmp = alpha_beta_dfs(0 - search_player_side, search_depth - 1, alpha, beta);
            }

            tmp.i = drop_choice1[k].i, tmp.j = drop_choice1[k].j;
            if (tmp.grade > result.grade) {
                result = tmp;
            }
            dfs_add_piece(drop_choice1[k].i, drop_choice1[k].j, VOID);
            alpha = long_long_max(alpha, result.grade);
            if (beta < alpha) {
#ifdef PRUNE_DEBUG
                ++prune_cnt;
                if (!(prune_cnt % 1000)) {
                    printf("prune_cnt: %d\n", prune_cnt);
                    printf("dfs depth: %d\n", DFS_MAX_DEPTH - search_depth);
                }
#endif
                //return result;
                break;
            }
        }
        //return result;
    } else {
#ifdef DFS_BOARD_DEBUG
        printf("dfs board:\n");
        dfs_output_board();
        GRADE_DEBUG
#endif
        result.grade = INF;
        drop_choice tmp;
        for (int k = 0; k < pos_num; ++k) {
            dfs_add_piece(drop_choice1[k].i, drop_choice1[k].j, BLACK);

            if (is_ban()) {
                //tmp.grade = INF;
                dfs_add_piece(drop_choice1[k].i, drop_choice1[k].j, VOID);
                continue;
            }

            if (real_hash[HASH] == hash && subtree_height[HASH] >= search_depth) {
                tmp.grade = cache_total_grade[HASH];
            } else {
                tmp = alpha_beta_dfs(0 - search_player_side, search_depth - 1, alpha, beta);
            }
            tmp.i = drop_choice1[k].i, tmp.j = drop_choice1[k].j;


            if (tmp.grade < result.grade) {
                result = tmp;
            }
            dfs_add_piece(drop_choice1[k].i, drop_choice1[k].j, VOID);
            beta = long_long_min(beta, result.grade);
            if (beta < alpha) {
#ifdef PRUNE_DEBUG
                ++prune_cnt;
                if (!(prune_cnt % 1000)) {
                    printf("prune_cnt: %d\n", prune_cnt);
                    printf("dfs depth: %d\n", DFS_MAX_DEPTH - search_depth);
                }
#endif
                //return result;
                break;
            }
        }
        //return result;
    }
    if (!real_hash[HASH] || (real_hash[HASH] == hash &&
                             (subtree_height[HASH] < search_depth ||
                              dfs_status.steps + search_depth > DFS_MAX_DEPTH + found_at_step[HASH]))) {
        real_hash[HASH] = hash;
        cache_total_grade[HASH] = result.grade;
        subtree_height[HASH] = search_depth;
        found_at_step[HASH] = dfs_status.steps;
    }
    return result;
}

inline int has_neighbor(int i, int j, int wid, int cnt) {
    int _cnt = 0;
    for (int _i = i - wid; _i <= i + wid; ++_i) {
        if (_i < 0 || _i >= BOARD_SIZE)continue;
        for (int _j = j - wid; _j <= j + wid; ++_j) {
            if (_j < 0 || _j >= BOARD_SIZE)continue;
            if (dfs_status.board[_i][_j] != VOID) {
                if (++_cnt >= cnt)return 1;
            }
        }
    }
    return 0;
}


