//
// Created by Nine_yota on 2018-09-29.
//

#include "boardEvaluate.h"

int32_t type_grade[PATTERN_TYPES];

extern boardStatus status;
extern boardStatus dfs_status;
extern trie tr[TRIE_SIZE];
// values in ban_cnt are only valid right after calling update_grade()
uint8_t ban_cnt[7];

//1==white wins -1== black wins 0==no one wins
int winner_check() {
    int __flag = VOID;
    int continuous5[5] = {};
    int sum = 0;
    //clear these two after each loop
    //loop in row
    for (int i = 0; i < BOARD_SIZE; ++i) {
        sum = 0;
        memset(continuous5, 0, sizeof(continuous5));
        for (int j = 0; j < BOARD_SIZE; ++j) {
            sum -= continuous5[j % 5];
            continuous5[j % 5] = status.board[i][j];
            sum += continuous5[j % 5];
            if (sum == 5)
                __flag = WHITE;
            else if (sum == -5)
                __flag = BLACK;
        }
    }
    //loop in col
    for (int j = 0; j < BOARD_SIZE; ++j) {
        sum = 0;
        memset(continuous5, 0, sizeof(continuous5));
        for (int i = 0; i < BOARD_SIZE; ++i) {
            sum -= continuous5[i % 5];
            continuous5[i % 5] = status.board[i][j];
            sum += continuous5[i % 5];
            if (sum == 5)
                __flag = WHITE;
            else if (sum == -5)
                __flag = BLACK;
        }
    }
    //loop oblique
    //i+j==offset
    for (int offset = 0; offset <= 2 * (BOARD_SIZE - 1); ++offset) {
        sum = 0;
        memset(continuous5, 0, sizeof(continuous5));
        int i = int_min(offset, BOARD_SIZE - 1);
        int j = offset - i;
        for (; i >= 0 && j < BOARD_SIZE; --i, ++j) {
            sum -= continuous5[i % 5];
            continuous5[i % 5] = status.board[i][j];
            sum += continuous5[i % 5];
            if (sum == 5)
                __flag = WHITE;
            else if (sum == -5)
                __flag = BLACK;
        }
    }
    //i-j==offset
    for (int offset = -(BOARD_SIZE - 1); offset < BOARD_SIZE; ++offset) {
        sum = 0;
        memset(continuous5, 0, sizeof(continuous5));
        int i = int_max(0, offset);
        int j = i - offset;
        for (; i < BOARD_SIZE && j < BOARD_SIZE; ++i, ++j) {
            sum -= continuous5[i % 5];
            continuous5[i % 5] = status.board[i][j];
            sum += continuous5[i % 5];
            if (sum == 5)
                __flag = WHITE;
            else if (sum == -5)
                __flag = BLACK;
        }
    }


    int flag;
    if (dfs_status.total_type[a5w] || dfs_status.total_type[d5w]) {
        //return WHITE;
        flag = WHITE;
    } else if (dfs_status.total_type[a5b] || dfs_status.total_type[d5b]) {
        //return BLACK;
        flag = BLACK;
    } else {
        //return VOID;
        flag = VOID;
    }

    if (__flag != flag) {
        printf("ERROR IN WINNER_CHECK!\n");
    }

    return __flag;
}


inline int64_t pos_estimate(int i, int j, int player_side) {
    //todo: add pos estimate here.
}


inline int64_t grade_estimate(int32_t player_side) {
    int64_t __grade = 0;
    if (player_side == WHITE) {
        for (int i = 1; i <= PATTERN_TYPES; ++i) {
            if (type_grade[i] < 0)
                continue;
            __grade += dfs_status.total_type[i] * type_grade[i];
        }
        /*
        if (dfs_status.total_type[a4b]) {
            __grade += type_grade[a4b];
        } else if (dfs_status.total_type[h4b]) {
            __grade += type_grade[h4b];
        } else if (dfs_status.total_type[sa4w3b]) {
            __grade += type_grade[sa4w3b];
        } else if (dfs_status.total_type[sa4n3b]) {
            __grade += type_grade[sa4n3b];
        } else if (dfs_status.total_type[a3b]) {
            __grade += type_grade[a3b];
        }
         */
    } else {
        for (int i = 1; i <= PATTERN_TYPES; ++i) {
            if (type_grade[i] > 0)
                continue;
            __grade += dfs_status.total_type[i] * type_grade[i];
        }
        /*
        if (dfs_status.total_type[a4w]) {
            __grade += type_grade[a4w];
        } else if (dfs_status.total_type[h4w]) {
            __grade += type_grade[h4w];
        } else if (dfs_status.total_type[sa4w3w]) {
            __grade += type_grade[sa4w3w];
        } else if (dfs_status.total_type[sa4n3w]) {
            __grade += type_grade[sa4n3w];
        } else if (dfs_status.total_type[a3w]) {
            __grade += type_grade[a3w];
        }
         */
    }
    return __grade;
}

inline void update_line_type_row(int row_index, int player_side) {
    for (int i = 1; i <= PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] -= dfs_status.row_type[row_index][i];
    }

    for (int j = 1; j <= PATTERN_TYPES; ++j) {
        dfs_status.row_type[row_index][j] = 0;
    }
    int cur = 0;
    for (int k = -1; k <= BOARD_SIZE; ++k) {
        int ch = k == -1 || k == BOARD_SIZE ? 0 - player_side : dfs_status.board[row_index][k];
        while (tr[cur].trans[ch + COLOR_OFFSET] == -1 && cur != 0) {
            cur = tr[cur].fail;
        }
        cur = tr[cur].trans[ch + COLOR_OFFSET];
        cur = cur == -1 ? 0 : cur;
        int tmp = cur;
        while (tmp != 0 && tr[tmp].type != 0) {
            if (tr[tmp].type) {
                update_type(dfs_status.row_type[row_index], tr[tmp].type);
            }
            tmp = tr[tmp].fail;
        }
    }
    for (int i = 1; i < PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] += dfs_status.row_type[row_index][i];
    }
}

inline void update_line_type_col(int col_index, int player_side) {
    for (int i = 1; i <= PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] -= dfs_status.col_type[col_index][i];
    }
    for (int j = 1; j <= PATTERN_TYPES; ++j) {
        dfs_status.col_type[col_index][j] = 0;
    }

    int cur = 0;
    for (int k = -1; k <= BOARD_SIZE; ++k) {
        int ch = k == -1 || k == BOARD_SIZE ? 0 - player_side : dfs_status.board[k][col_index];
        while (tr[cur].trans[ch + COLOR_OFFSET] == -1 && cur != 0) {
            cur = tr[cur].fail;
        }
        cur = tr[cur].trans[ch + COLOR_OFFSET];
        cur = cur == -1 ? 0 : cur;
        int tmp = cur;
        while (tmp != 0 && tr[tmp].type != 0) {
            if (tr[tmp].type) {
                update_type(dfs_status.col_type[col_index], tr[tmp].type);
            }
            tmp = tr[tmp].fail;
        }
    }
    for (int i = 1; i < PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] += dfs_status.col_type[col_index][i];
    }
}

inline void update_line_type_oblique_sum(int oblique_sum_index, int player_side) {
    for (int i = 1; i <= PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] -= dfs_status.oblique_sum_type[oblique_sum_index][i];
    }
    for (int j = 1; j <= PATTERN_TYPES; ++j) {
        dfs_status.oblique_sum_type[oblique_sum_index][j] = 0;
    }

    int cur = 0;

    int start, end;

    if (oblique_sum_index <= BOARD_SIZE - 1) {
        start = -1;
        end = oblique_sum_index + 1;
    } else {
        start = oblique_sum_index - BOARD_SIZE;
        end = BOARD_SIZE;
    }

    for (int k = start; k <= end; ++k) {
        int ch = k == start || k == end ? 0 - player_side : dfs_status.board[oblique_sum_index - k][k];
        while (tr[cur].trans[ch + COLOR_OFFSET] == -1 && cur != 0) {
            cur = tr[cur].fail;
        }
        cur = tr[cur].trans[ch + COLOR_OFFSET];
        cur = cur == -1 ? 0 : cur;
        int tmp = cur;
        while (tmp != 0 && tr[tmp].type != 0) {
            if (tr[tmp].type) {
                update_type(dfs_status.oblique_sum_type[oblique_sum_index], tr[tmp].type);
            }
            tmp = tr[tmp].fail;
        }
    }

    for (int i = 0; i < PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] += dfs_status.oblique_sum_type[oblique_sum_index][i];
    }
}

inline void update_line_type_oblique_delta(int oblique_delta_index, int player_side) {
    for (int i = 1; i <= PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] -= dfs_status.oblique_delta_type[oblique_delta_index + BOARD_SIZE][i];
    }
    for (int j = 1; j <= PATTERN_TYPES; ++j) {
        dfs_status.oblique_delta_type[oblique_delta_index + BOARD_SIZE][j] = 0;
    }

    int cur = 0;

    int start;
    int end;

    if (oblique_delta_index >= 0) {
        start = -1;
        end = BOARD_SIZE - oblique_delta_index;
    } else {
        start = -oblique_delta_index + 1;
        end = BOARD_SIZE;
    }

    for (int k = start; k <= end; ++k) {
        int ch = k == start || k == end ? 0 - player_side : dfs_status.board[oblique_delta_index + k][k];
        while (tr[cur].trans[ch + COLOR_OFFSET] == -1 && cur != 0) {
            cur = tr[cur].fail;
        }
        cur = tr[cur].trans[ch + COLOR_OFFSET];
        cur = cur == -1 ? 0 : cur;
        int tmp = cur;
        while (tmp != 0 && tr[tmp].type != 0) {
            if (tr[tmp].type) {
                update_type(dfs_status.oblique_delta_type[oblique_delta_index + BOARD_SIZE], tr[tmp].type);
            }
            tmp = tr[tmp].fail;
        }
    }
    for (int i = 1; i < PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] += dfs_status.oblique_delta_type[oblique_delta_index + BOARD_SIZE][i];
    }
}

inline void update_grade(int i, int j) {

    ban_clear(i, j);

    update_line_type_row(i, WHITE);
    update_line_type_col(j, WHITE);
    update_line_type_oblique_sum(i + j, WHITE);
    update_line_type_oblique_delta(i - j, WHITE);

    update_line_type_row(i, BLACK);
    update_line_type_col(j, BLACK);
    update_line_type_oblique_sum(i + j, BLACK);
    update_line_type_oblique_delta(i - j, BLACK);

    ban_detect(i, j);
}


uint8_t is_ban() {
    //todo: there are problems with ban check
#ifndef BAN_DEBUG
    if (ban_cnt[6])
        return 1;
    if (ban_cnt[4] >= 2)
        return 1;
    if (ban_cnt[3] >= 2)
        return 1;
    return 0;
#endif
#ifdef BAN_DEBUG
    return 0;
#endif
}

void evaluate_init() {
    SET0(ban_cnt);
    type_grade[a5w] = FIVE_GRADE;
    type_grade[a5b] = -FIVE_GRADE;
    type_grade[a4w] = CONTINUOUS_FOUR;
    type_grade[a4b] = -CONTINUOUS_FOUR;
    type_grade[a3w] = CONTINUOUS_THREE;
    type_grade[a3b] = -CONTINUOUS_THREE;
    type_grade[a2w] = CONTINUOUS_TWO;
    type_grade[a2b] = -CONTINUOUS_TWO;
    type_grade[a1w] = CONTINUOUS_ONE;
    type_grade[a1b] = -CONTINUOUS_ONE;

    type_grade[h5w] = FIVE_GRADE;
    type_grade[h5b] = -FIVE_GRADE;
    type_grade[h4w] = HALF_FOUR;
    type_grade[h4b] = -HALF_FOUR;
    type_grade[h3w] = HALF_THREE;
    type_grade[h3b] = -HALF_THREE;
    type_grade[h2w] = HALF_TWO;
    type_grade[h2b] = -HALF_TWO;

    type_grade[d5w] = FIVE_GRADE;
    type_grade[d5b] = -FIVE_GRADE;

    type_grade[sa3w] = SPLIT_ALIVE_THREE;
    type_grade[sa3b] = -SPLIT_ALIVE_THREE;
    type_grade[sa4n3w] = SPLIT_ALIVE_FOUR_WITHOUT3;
    type_grade[sa4n3b] = -SPLIT_ALIVE_FOUR_WITHOUT3;
    type_grade[sa4w3w] = SPLIT_ALIVE_FOUR_WITH3;
    type_grade[sa4w3b] = -SPLIT_ALIVE_FOUR_WITH3;
}

inline void update_type(uint8_t type_array[], uint8_t type) {
    switch (type) {
        case sa4w3w:
            ++type_array[sa4w3w];
            --type_array[a3w];
            break;
        case sa4w3b:
            ++type_array[sa4w3b];
            --type_array[a3b];
            break;
        default:
            ++type_array[type];
            break;
    }
}

inline void ban_clear(int i, int j) {
    ban_cnt[3] = ban_cnt[4] = ban_cnt[6] = 0;

    ban_cnt[3] -=
            dfs_status.row_type[i][a3b] + dfs_status.row_type[i][sa3b] + dfs_status.col_type[j][a3b] +
            dfs_status.col_type[j][sa3b]
            + dfs_status.oblique_sum_type[i + j][a3b] + dfs_status.oblique_sum_type[i + j][sa3b] +
            dfs_status.oblique_delta_type[i - j + BOARD_SIZE][a3b]
            + dfs_status.oblique_delta_type[i - j + BOARD_SIZE][sa3b];

    ban_cnt[4] -=
            dfs_status.row_type[i][a4b] + dfs_status.row_type[i][h4b] + dfs_status.row_type[i][sa4w3b] +
            dfs_status.row_type[i][sa4n3b]
            + dfs_status.col_type[j][a4b] + dfs_status.col_type[j][h4b] + dfs_status.col_type[j][sa4w3b] +
            dfs_status.col_type[j][sa4n3b]
            + dfs_status.oblique_sum_type[i + j][a4b] + dfs_status.oblique_sum_type[i + j][h4b] +
            dfs_status.oblique_sum_type[i + j][sa4w3b] + dfs_status.oblique_sum_type[i + j][sa4n3b]
            + dfs_status.oblique_delta_type[i - j + BOARD_SIZE][a4b] +
            dfs_status.oblique_delta_type[i - j + BOARD_SIZE][h4b] +
            dfs_status.oblique_delta_type[i - j + BOARD_SIZE][sa4w3b] +
            dfs_status.oblique_delta_type[i - j + BOARD_SIZE][sa4n3b];

    ban_cnt[6] -= dfs_status.row_type[i][l6b] + dfs_status.col_type[j][l6b] + dfs_status.oblique_sum_type[i + j][l6b] +
                  dfs_status.oblique_delta_type[i - j + BOARD_SIZE][l6b];

}

inline void ban_detect(int i, int j) {
    ban_cnt[3] +=
            dfs_status.row_type[i][a3b] + dfs_status.row_type[i][sa3b] + dfs_status.col_type[j][a3b] +
            dfs_status.col_type[j][sa3b]
            + dfs_status.oblique_sum_type[i + j][a3b] + dfs_status.oblique_sum_type[i + j][sa3b] +
            dfs_status.oblique_delta_type[i - j + BOARD_SIZE][a3b]
            + dfs_status.oblique_delta_type[i - j + BOARD_SIZE][sa3b];

    ban_cnt[4] += dfs_status.row_type[i][a4b] + dfs_status.row_type[i][h4b] + dfs_status.row_type[i][sa4w3b] +
                  dfs_status.row_type[i][sa4n3b]
                  + dfs_status.col_type[j][a4b] + dfs_status.col_type[j][h4b] + dfs_status.col_type[j][sa4w3b] +
                  dfs_status.col_type[j][sa4n3b]
                  + dfs_status.oblique_sum_type[i + j][a4b] + dfs_status.oblique_sum_type[i + j][h4b] +
                  dfs_status.oblique_sum_type[i + j][sa4w3b] + dfs_status.oblique_sum_type[i + j][sa4n3b]
                  + dfs_status.oblique_delta_type[i - j + BOARD_SIZE][a4b] +
                  dfs_status.oblique_delta_type[i - j + BOARD_SIZE][h4b] +
                  dfs_status.oblique_delta_type[i - j + BOARD_SIZE][sa4w3b] +
                  dfs_status.oblique_delta_type[i - j + BOARD_SIZE][sa4n3b];

    ban_cnt[6] += dfs_status.row_type[i][l6b] + dfs_status.col_type[j][l6b] + dfs_status.oblique_sum_type[i + j][l6b] +
                  dfs_status.oblique_delta_type[i - j + BOARD_SIZE][l6b];
}

#undef TRIE_SIZE
#undef PATTERNS
#undef END
#undef MAX_PATTERN_LEN
#undef COLOR_OFFSET