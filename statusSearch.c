//
// Created by Nine_yota on 2018-10-07.
//

#include "statusSearch.h"

#define FORCE_CUT 225

extern boardStatus status;
extern boardStatus dfs_status;


//0 for WHITE and 1 for BLACK
static unsigned long long prune_table[2][BOARD_SIZE][BOARD_SIZE];
/*
 * values are stored in this table by 8^remaining_depth
 * and the scale of different depth are following:
 * */


timer search_duration;

drop_choice best_choice_of_lower_depth;

/*
 *  DFS starts here
 * */
#define MAX_POS 225

inline void generate_possible_pos(drop_choice *drop_choice1, int *num, int search_player_side) {
    *num = 0;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (dfs_status.board[i][j] == VOID) {
                /* skip useless points as many as possible
                 * no piece has been dropped when counting,
                 * so the central piece should not been taken into consideration.
                 */
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
                drop_choice1[*num].grade_estimate = grade_standardise(
                        pos_estimate(i, j, search_player_side) - pos_estimate(i, j, -search_player_side));
                drop_choice1[*num].grade_estimate += prune_table[search_player_side == WHITE ? 0 : 1][i][j];

                ++(*num);
            }
        }
    }
    choice_sort(drop_choice1, *num, search_player_side);
    // in older versions, this is used for force prune.
    // but it turns out that program drops worse so now it doesn't do this
    *num = int_min(*num, FORCE_CUT);
}

//remember that the larger the grade is, the better the status is for WHITE. and vice versa.
//alpha is the upper bound and beta is the lower bound.
drop_choice alpha_beta_dfs(int search_player_side, unsigned int search_depth, long long alpha, long long beta) {
    // as the name suggests, this is a depth first searching with the alpha-beta-prune method.
    // the return type is a struct which stores the place and the grade of that choice.
    drop_choice result;
    result.broken_search_flag = false;

    if (search_depth == 0) {
        // it doesn't matter what the position is in the very bottom.
        result.grade = grade_estimate(search_player_side) + grade_estimate(0 - search_player_side);
        return result;
    }
    /*
     * if you check hash and return result here, it may drop in a occupied place!
     */

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

            //check whether a winner occurs only at this place is useless because it helps nothing.
            if (winner_check() == WHITE) {
                dfs_add_piece(drop_choice1[k].i, drop_choice1[k].j, VOID);
                result.grade = FIVE_GRADE;
                result.i = drop_choice1[k].i;
                result.j = drop_choice1[k].j;
                break;
            }


            tmp = alpha_beta_dfs(0 - search_player_side, search_depth - 1, alpha, beta);
            tmp.i = drop_choice1[k].i, tmp.j = drop_choice1[k].j;

            if (tmp.grade > result.grade) {
                result = tmp;
            }
            dfs_add_piece(drop_choice1[k].i, drop_choice1[k].j, VOID);
            alpha = long_long_max(alpha, result.grade);
            if (result.grade >= FIVE_GRADE) {
                break;
            }

            if (beta < alpha) {
#ifdef PRUNE_DEBUG
                ++prune_cnt;
                if (!(prune_cnt % 1000)) {
                    printf("prune_cnt: %d\n", prune_cnt);
                    printf("dfs depth: %d\n", search_depth);
                }
#endif

                //0 for WHITE
                if (prune_table[0][drop_choice1[k].i][drop_choice1[k].j] < INF)
                    prune_table[0][drop_choice1[k].i][drop_choice1[k].j] += 1U << (search_depth << 2U);
                break;
            }

            if (duration_check() || tmp.broken_search_flag) {
#ifdef DEBUG_DRAW
                printf("search breaks half way\n");
                time_display(search_depth, result.grade);
#endif
                result.broken_search_flag = true;
                break;
            }
        }
    } else {
        result.grade = INF;
        drop_choice tmp;
        for (int k = 0; k < pos_num; ++k) {


            dfs_add_piece(drop_choice1[k].i, drop_choice1[k].j, BLACK);

            if (is_ban()) {
                dfs_add_piece(drop_choice1[k].i, drop_choice1[k].j, VOID);
                continue;
            } else if (winner_check() == BLACK) {
                dfs_add_piece(drop_choice1[k].i, drop_choice1[k].j, VOID);
                result.grade = -FIVE_GRADE;
                result.i = drop_choice1[k].i;
                result.j = drop_choice1[k].j;
                break;
            }


            tmp = alpha_beta_dfs(0 - search_player_side, search_depth - 1, alpha, beta);
            tmp.i = drop_choice1[k].i, tmp.j = drop_choice1[k].j;


            if (tmp.grade < result.grade) {
                result = tmp;
            }
            dfs_add_piece(drop_choice1[k].i, drop_choice1[k].j, VOID);
            beta = long_long_min(beta, result.grade);
            if (result.grade <= -FIVE_GRADE) {
                break;
            }

            if (beta < alpha) {
#ifdef PRUNE_DEBUG
                ++prune_cnt;
                if (!(prune_cnt % 1000)) {
                    printf("prune_cnt: %d\n", prune_cnt);
                    printf("dfs depth: %d\n", search_depth);
                }
#endif

                //1 for BLACK
                if (prune_table[1][drop_choice1[k].i][drop_choice1[k].j] > -INF)
                    prune_table[1][drop_choice1[k].i][drop_choice1[k].j] -= 1U << (search_depth << 2U);
                //return result;
                break;
            }
            if (duration_check() || tmp.broken_search_flag) {
#ifdef DEBUG_DRAW
                printf("search breaks half way\n");
                time_display(search_depth, result.grade);
#endif
                result.broken_search_flag = true;
                break;
            }
        }
    }
    return result;
}


inline drop_choice deepening_search(int search_player_side, int depth_bound) {
    // iterative deepening searching function that tries to use every seconds of the time limit.
    if (depth_bound <= 0) {
        printf("Search error!\n Invalid depth\n");
        exit(1);
    }
    search_duration.start_clock = clock();
    unsigned int d = 2;
    drop_choice best_choice = best_choice_of_lower_depth = alpha_beta_dfs(search_player_side, d, 0 - INF, INF);
#ifdef DEBUG_DRAW
    time_display(d, best_choice.grade);
#endif
    for (d = d + 2; d <= depth_bound; d += 2) {
        best_choice = best_choice_of_lower_depth = alpha_beta_dfs(search_player_side, d, 0 - INF, INF);
#ifdef DEBUG_DRAW
        time_display(d, best_choice.grade);
#endif
        // if time is not enough, then return the choice of the searching of lower depth.
        if (best_choice.broken_search_flag ||
            search_duration.start_clock - clock() >= 0.99 * TIME_LIMIT) {
            return best_choice_of_lower_depth;
        }
        if (best_choice.grade * search_player_side >= FIVE_GRADE) {
            return best_choice;
        }
    }
    return best_choice;
}

inline int has_neighbor(int i, int j, int wid, int cnt) {
    int _cnt = 0;
    for (int _i = i - wid; _i <= i + wid; ++_i) {
        if (_i < 0 || _i >= BOARD_SIZE)continue;
        for (int _j = j - wid; _j <= j + wid; ++_j) {
            if (_j < 0 || _j >= BOARD_SIZE)continue;
            if (dfs_status.board[_i][_j] != VOID) {
                if (++_cnt == cnt)return 1;
            }
        }
    }
    return 0;
}


void search_init() {
    SET0(prune_table);
}

inline long long int grade_standardise(long long int grade) {
    if (grade <= -CONTINUOUS_FOUR) {
        return -INF;
    } else if (grade >= CONTINUOUS_FOUR) {
        return INF;
    } else if (grade < 0) {
        return -(long long) pow(1.001, -grade);
    } else {
        return (long long) pow(1.001, grade);
    }
}

void time_init() {
    search_duration.start_clock = search_duration.end_clock = 0;
}

inline void time_display(unsigned int depth, long long int dfs_grade) {
    clock_t tm = (clock() - search_duration.start_clock);
    printf("time: %2.3lf  depth: %d  ", tm * 1.0 / CLOCKS_PER_SEC, depth);
    printf("grade: %lld\n", dfs_grade);
}

inline int duration_check() {
    return (clock() - search_duration.start_clock) * 1.0 / CLOCKS_PER_SEC > TIME_LIMIT * 0.99;
}