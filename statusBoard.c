//
// Created by Nine_yota on 2018-09-29.
//

#include "statusBoard.h"

//status_board[i][j] -> oblique_line_sum[i+j][j]

//status_board[i][j] -> oblique_line_delta[i-j+BOARD_SIZE][j]

boardStatus status;
boardStatus dfs_status;
drop_record record[BOARD_SIZE * BOARD_SIZE + 5];
int ban_cnt[7];

extern trie tr[TRIE_SIZE];
extern unsigned long long hash_key[2][BOARD_SIZE][BOARD_SIZE];
extern long long cache_total_grade[2][CACHE_SIZE];
extern long long cache_col_grade[2][CACHE_SIZE];
extern long long cache_row_grade[2][CACHE_SIZE];
extern long long cache_oblique_sum_grade[2][CACHE_SIZE];
extern long long cache_oblique_delta_grade[2][CACHE_SIZE];
extern int cache_record_step[CACHE_SIZE];
extern unsigned long long real_hash[CACHE_SIZE];
extern unsigned long long hash;

void __status_init(boardStatus *boardStatus1) {
    SET0(boardStatus1->board);
    SET0(boardStatus1->oblique_line_sum);
    SET0(boardStatus1->oblique_line_delta);
    SET0(boardStatus1->row_grade);
    SET0(boardStatus1->col_grade);
    SET0(boardStatus1->oblique_sum_grade);
    SET0(boardStatus1->oblique_delta_grade);
    SET0(boardStatus1->total_grade);
    boardStatus1->steps = 0;
}

void status_init() {
    __status_init(&status);
    __status_init(&dfs_status);
    SET0(ban_cnt);
    record[0].i = record[0].j = -1;
}

//todo: more tests needed
inline void grade_unique(long long grade) {
    switch (grade) {
        case -SPLIT_ALIVE_FOUR_WITH3:
            --ban_cnt[3];
            ++ban_cnt[4];
            break;
        case -SPLIT_ALIVE_FOUR_WITHOUT3:
            ++ban_cnt[4];
            break;
        case -SPLIT_ALIVE_THREE:
            ++ban_cnt[3];
            break;
        case -CONTINUOUS_THREE:
            ++ban_cnt[3];
            break;
        case -LONG_CONTINUOUS:
            --ban_cnt[4];
            ++ban_cnt[6];
            break;
        default:
            break;
    }
}


inline void update_line_grade_row(int row_index, int player_side) {
    long long _grade = 0;
    int cur = 0;
    for (int k = -1; k <= BOARD_SIZE; ++k) {
        int ch = k == -1 || k == BOARD_SIZE ? 0 - player_side : dfs_status.board[row_index][k];
        while (tr[cur].trans[ch + COLOR_OFFSET] == -1 && cur != 0) {
            cur = tr[cur].fail;
        }
        cur = tr[cur].trans[ch + COLOR_OFFSET];
        cur = cur == -1 ? 0 : cur;
        int tmp = cur;
        while (tmp != 0 && tr[tmp].grade != 0) {
            if (player_side == WHITE && tr[tmp].grade > 0) {
                _grade += tr[tmp].grade;
            } else if (player_side == BLACK && tr[tmp].grade < 0) {
                _grade += tr[tmp].grade;
#ifdef BAN_DEBUG
                grade_unique(tr[tmp].grade);
#endif
            }
            tmp = tr[tmp].fail;
        }
    }
    dfs_status.total_grade[PLAYER_IN_LINE] -= dfs_status.row_grade[PLAYER_IN_LINE][row_index];
    dfs_status.total_grade[PLAYER_IN_LINE] += _grade;

    dfs_status.row_grade[PLAYER_IN_LINE][row_index] = _grade;
}

inline void update_line_grade_col(int col_index, int player_side) {
    int cur = 0;
    long long _grade = 0;
    for (int k = -1; k <= BOARD_SIZE; ++k) {
        int ch = k == -1 || k == BOARD_SIZE ? 0 - player_side : dfs_status.board[k][col_index];
        while (tr[cur].trans[ch + COLOR_OFFSET] == -1 && cur != 0) {
            cur = tr[cur].fail;
        }
        cur = tr[cur].trans[ch + COLOR_OFFSET];
        cur = cur == -1 ? 0 : cur;
        int tmp = cur;
        while (tmp != 0 && tr[tmp].grade != 0) {
            if (player_side == WHITE && tr[tmp].grade > 0) {
                _grade += tr[tmp].grade;
            } else if (player_side == BLACK && tr[tmp].grade < 0) {
                _grade += tr[tmp].grade;
#ifdef BAN_DEBUG
                grade_unique(tr[tmp].grade);
#endif
            }
            tmp = tr[tmp].fail;
        }
    }
    dfs_status.total_grade[PLAYER_IN_LINE] -= dfs_status.col_grade[PLAYER_IN_LINE][col_index];
    dfs_status.total_grade[PLAYER_IN_LINE] += _grade;

    dfs_status.col_grade[PLAYER_IN_LINE][col_index] = _grade;
}

inline void update_line_grade_oblique_sum(int oblique_sum_index, int player_side) {
    int cur = 0;
    long long _grade = 0;
    for (int k = -1; k <= BOARD_SIZE; ++k) {
        int ch = k == int_max(-1, oblique_sum_index - BOARD_SIZE) || k == int_min(oblique_sum_index + 1, BOARD_SIZE) ?
                 0 - player_side : dfs_status.oblique_line_sum[oblique_sum_index][k];
        while (tr[cur].trans[ch + COLOR_OFFSET] == -1 && cur != 0) {
            cur = tr[cur].fail;
        }
        cur = tr[cur].trans[ch + COLOR_OFFSET];
        cur = cur == -1 ? 0 : cur;
        int tmp = cur;
        while (tmp != 0 && tr[tmp].grade != 0) {
            if (player_side == WHITE && tr[tmp].grade > 0) {
                _grade += tr[tmp].grade;
            } else if (player_side == BLACK && tr[tmp].grade < 0) {
                _grade += tr[tmp].grade;
#ifdef BAN_DEBUG
                grade_unique(tr[tmp].grade);
#endif
            }
            tmp = tr[tmp].fail;
        }
    }
    dfs_status.total_grade[PLAYER_IN_LINE] -= dfs_status.oblique_sum_grade[PLAYER_IN_LINE][oblique_sum_index];
    dfs_status.total_grade[PLAYER_IN_LINE] += _grade;

    dfs_status.oblique_sum_grade[PLAYER_IN_LINE][oblique_sum_index] = _grade;
}

inline void update_line_grade_oblique_delta(int oblique_delta_index, int player_side) {
    int cur = 0;
    long long _grade = 0;
    for (int k = -1; k <= BOARD_SIZE; ++k) {
        int ch =
                k == int_max(-1, -1 - oblique_delta_index) || k == int_min(BOARD_SIZE - oblique_delta_index, BOARD_SIZE)
                ? 0 - player_side : dfs_status.oblique_line_delta[oblique_delta_index + BOARD_SIZE][k];
        while (tr[cur].trans[ch + COLOR_OFFSET] == -1 && cur != 0) {
            cur = tr[cur].fail;
        }
        cur = tr[cur].trans[ch + COLOR_OFFSET];
        cur = cur == -1 ? 0 : cur;
        int tmp = cur;
        while (tmp != 0 && tr[tmp].grade != 0) {
            if (player_side == WHITE && tr[tmp].grade > 0) {
                _grade += tr[tmp].grade;
            } else if (player_side == BLACK && tr[tmp].grade < 0) {
                _grade += tr[tmp].grade;
#ifdef BAN_DEBUG
                grade_unique(tr[tmp].grade);
#endif
            }
            tmp = tr[tmp].fail;
        }
    }
    dfs_status.total_grade[PLAYER_IN_LINE] -= dfs_status.oblique_delta_grade[PLAYER_IN_LINE][oblique_delta_index +
                                                                                             BOARD_SIZE];
    dfs_status.total_grade[PLAYER_IN_LINE] += _grade;

    dfs_status.oblique_delta_grade[PLAYER_IN_LINE][oblique_delta_index + BOARD_SIZE] = _grade;

}

inline void update_grade(int i, int j) {

    SET0(ban_cnt);

    update_line_grade_row(i, WHITE);
    update_line_grade_col(j, WHITE);
    update_line_grade_oblique_sum(i + j, WHITE);
    update_line_grade_oblique_delta(i - j, WHITE);

    update_line_grade_row(i, BLACK);
    update_line_grade_col(j, BLACK);
    update_line_grade_oblique_sum(i + j, BLACK);
    update_line_grade_oblique_delta(i - j, BLACK);
}

/*  return -1 if that position is occupied
 *  return 0 if pc==0, which means there's nothing to do
 *  return 1 -> successful
 * */

int add_piece(int i, int j, int player_side) {
    if (status.board[i][j] != VOID)
        return -1;
    status.board[i][j] = player_side;
    status.oblique_line_sum[i + j][j] = player_side;
    status.oblique_line_delta[i - j + BOARD_SIZE][j] = player_side;

    int check_code = dfs_add_piece(i, j, player_side);

    if (check_code != -1) {
        status.steps++;
        record[status.steps].i = i;
        record[status.steps].j = j;
        record[status.steps].player = player_side;
    }

    //todo: more tests needed for ban checking
    if (ban_cnt[3] >= 2) {
        printf("!!\nban found: 3\n!!\n");
    } else if (ban_cnt[4] >= 2) {
        printf("!!\nban found: 4\n!!\n");
    } else if (ban_cnt[6]) {
        printf("!!\nban found: 6\n!!\n");
    } else {
        SET0(ban_cnt);
    }

#ifdef RECORD_DEBUG
    printf("steps: %d\n", status.steps);
    printf("i: %d\tj: %d\n", record[status.steps].i, record[status.steps].j);
#endif
    return check_code;
}

/*  return -1 if that position is occupied
 *  return 1 -> successful
 * */
int dfs_add_piece(int i, int j, int player_side) {
    if (dfs_status.board[i][j] != VOID && player_side != VOID)
        return -1;
    dfs_status.board[i][j] = player_side;
    dfs_status.oblique_line_sum[i + j][j] = player_side;
    dfs_status.oblique_line_delta[i - j + BOARD_SIZE][j] = player_side;
    if (player_side == VOID) {
        dfs_status.steps--;
    } else {
        dfs_status.steps++;
    }

    //if this status has been evaluated and exists in hash table, then take the grade from the hash table
    //otherwise update_grade


    //todo: hash needs more tests!
#ifdef USE_HASH
    hash ^= hash_key[PLAYER_IN_LINE][i][j];

    if (cache_total_grade[PLAYER_IN_LINE][HASH] && real_hash[HASH] == hash &&
        cache_record_step[HASH] == dfs_status.steps) {

        //but it seems that after adding hash features, the level of this program decrease....QAQ
        //sometimes the program drops weirdly????

#ifdef HASH_DEBUG
        printf("current HASH: %llu\n", HASH);
        //printf("input any char to continue:\n");
        //char tmp;
        //scanf("%c", &tmp);
#endif
        for (int player = 0; player <= 1; ++player) {
            dfs_status.total_grade[player] = cache_total_grade[player][HASH];
            dfs_status.row_grade[player][i] = cache_row_grade[player][HASH];
            dfs_status.col_grade[player][j] = cache_col_grade[player][HASH];
            dfs_status.oblique_sum_grade[player][i + j] = cache_oblique_sum_grade[player][HASH];
            dfs_status.oblique_delta_grade[player][i - j + BOARD_SIZE] = cache_oblique_delta_grade[player][HASH];
        }
    } else {
        update_grade(i, j);
        for (int player = 0; player <= 1; ++player) {
            cache_total_grade[player][HASH] = dfs_status.total_grade[player];
            cache_row_grade[player][HASH] = dfs_status.row_grade[player][i];
            cache_col_grade[player][HASH] = dfs_status.col_grade[player][j];
            cache_oblique_sum_grade[player][HASH] = dfs_status.oblique_sum_grade[player][i + j];
            cache_oblique_delta_grade[player][HASH] = dfs_status.oblique_delta_grade[player][i - j + BOARD_SIZE];
        }
        cache_record_step[HASH] = dfs_status.steps;
    }
#endif

#ifndef USE_HASH
    update_grade(i, j);
#endif
    return 1;
}
