//
// Created by Nine_yota on 2018-10-07.
//

#include "statusSearch.h"

extern boardStatus status;
extern boardStatus dfs_status;
//extern long long cache_total_grade[CACHE_SIZE];
extern drop_choice cache_choice[CACHE_SIZE];
extern int subtree_height[CACHE_SIZE];
extern int found_at_step[CACHE_SIZE];
extern unsigned long long real_hash[CACHE_SIZE];
extern unsigned long long hash;

//0 for WHITE and 1 for BLACK
unsigned long long prune_table[2][BOARD_SIZE][BOARD_SIZE];
/*
 * values are stored in this table by 8^remaining_depth
 * and the scale of different depth are following:
 * */


/*
 *  DFS starts here
 * */
#define MAX_POS 225

inline void generate_possible_pos(drop_choice *drop_choice1, int *num, int8_t search_player_side) {
    *num = 0;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (dfs_status.board[i][j] == VOID) {
                /* skip useless points as many as possible
                 * no piece has been dropped when counting,
                 * so the central piece should not been taken into consideration.
                 */
                //todo: more methods needed to ensure that better places are sorted ahead.
                if (dfs_status.steps <= 2) {
                    if (!has_neighbor(i, j, 1, 1)) continue;
                } else if (dfs_status.steps <= 4) {
                    if (!has_neighbor(i, j, 1, 2)) continue;
                } else {
                    if (!has_neighbor(i, j, 2, 1)) continue;
                }
                drop_choice1[*num].i = i;
                drop_choice1[*num].j = j;
                //CAUTIONS!!! IT SHOULD BE MINUS!!!
                drop_choice1[*num].grade_estimate =
                        grade_standarise(
                                pos_estimate(i, j, search_player_side) - pos_estimate(i, j, -search_player_side));
                //todo: check
                drop_choice1[*num].grade_estimate += prune_table[search_player_side == WHITE ? 0 : 1][i][j];

                //make reached choices sorted ahead
                if (real_hash[HASH] == hash) {
                    drop_choice1[*num].grade_estimate += search_player_side == WHITE ? INF : -INF;
                }
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
drop_choice alpha_beta_dfs(int8_t search_player_side, uint32_t search_depth, int64_t alpha, int64_t beta) {
    drop_choice result;
    //result.i = result.j = 0;
    //result.grade = 0;
    result.player = search_player_side;

    if (search_depth == 0) {
        // it doesn't matter what the position is in the very bottom.
        result.grade = grade_estimate(search_player_side) + grade_estimate(0 - search_player_side);
        return result;
    }
    if (real_hash[HASH] == hash && subtree_height[HASH] >= search_depth) {
        result = cache_choice[HASH];
        return result;
    }

    drop_choice drop_choice1[MAX_POS];
    int pos_num = 0;

    //generate proper places to drop a piece
    generate_possible_pos(drop_choice1, &pos_num, search_player_side);

    //traverse the proper places
    if (search_player_side == WHITE) {
        result.grade = 0 - INF;
        drop_choice tmp;
        for (int k = 0; k < pos_num; ++k) {
            dfs_add_piece(drop_choice1[k].i, drop_choice1[k].j, WHITE);

            tmp = alpha_beta_dfs((int8_t) 0 - search_player_side, search_depth - 1, alpha, beta);

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

                //0 for WHITE
                prune_table[0][drop_choice1[k].i][drop_choice1[k].j] += 1U << (search_depth << 2U);
                //return result;
                break;
            }
        }
        //return result;
    } else {
        result.grade = INF;
        drop_choice tmp;
        for (int k = 0; k < pos_num; ++k) {
            dfs_add_piece(drop_choice1[k].i, drop_choice1[k].j, BLACK);

            if (is_ban()) {
                //tmp.grade = INF;
                dfs_add_piece(drop_choice1[k].i, drop_choice1[k].j, VOID);
                continue;
            }

            tmp = alpha_beta_dfs((int8_t) 0 - search_player_side, search_depth - 1, alpha, beta);
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

                //1 for BLACK
                prune_table[1][drop_choice1[k].i][drop_choice1[k].j] -= 1U << (search_depth << 2U);
                //return result;
                break;
            }
        }
        //return result;
    }
    if (!real_hash[HASH] ||
        (real_hash[HASH] != hash && dfs_status.steps + search_depth > DFS_MAX_DEPTH + found_at_step[HASH])
        || (real_hash[HASH] == hash && subtree_height[HASH] < search_depth)) {
        real_hash[HASH] = hash;
        cache_choice[HASH] = result;
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
                ++_cnt;
                if (_cnt == cnt)return 1;
            }
        }
    }
    return 0;
}


void search_init() {
    SET0(prune_table);
}

int64_t grade_standarise(int64_t grade) {
    if (grade <= -CONTINUOUS_FOUR) {
        return -INF;
    } else if (grade >= CONTINUOUS_FOUR) {
        return INF;
    } else if (grade < 0) {
        return -(int64_t) pow(1.0008, -grade);
    } else {
        return (int64_t) pow(1.0008, grade);
    }
}