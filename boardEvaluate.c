//
// Created by Nine_yota on 2018-09-29.
//

#include "boardEvaluate.h"

#define __WHITE 1
#define __BLACK 0

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

inline int64_t grade_estimate(int32_t player_side) {
    int64_t __grade = 0;
    if (player_side == WHITE) {
        for (int i = 1; i <= PATTERN_TYPES; ++i) {
            if (type_grade[i] < 0)
                continue;
            __grade += dfs_status.total_type[i] * type_grade[i];
        }
    } else {
        for (int i = 1; i <= PATTERN_TYPES; ++i) {
            if (type_grade[i] > 0)
                continue;
            __grade += dfs_status.total_type[i] * type_grade[i];
        }
    }
    return __grade;
}

//todo: now the 4 finds of type update could be merged into a single function using board_access()
inline void update_line_type_row(int row_index) {
    for (int32_t i = 1; i <= PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] -= dfs_status.row_type[row_index][i];
    }
    for (int32_t j = 1; j <= PATTERN_TYPES; ++j) {
        dfs_status.row_type[row_index][j] = 0;
        for (int32_t k = 0; k < BOARD_SIZE; ++k) {
            dfs_status.row_increment[row_index][k][j] = 0;
        }
    }

    int32_t cur = 0;

    int32_t left, right;

    left = dfs_status.board[row_index][0] == VOID ? VOID : 0 - dfs_status.board[row_index][0];
    right = dfs_status.board[row_index][BOARD_SIZE - 1] == VOID ?
            VOID : 0 - dfs_status.board[row_index][BOARD_SIZE - 1];

    for (int k = -1; k <= BOARD_SIZE; ++k) {
        int ch = k == -1 ? left : k == BOARD_SIZE ? right : dfs_status.board[row_index][k];
        while (tr[cur].trans[ch + COLOR_OFFSET] == -1 && cur != 0) {
            cur = tr[cur].fail;
        }
        cur = tr[cur].trans[ch + COLOR_OFFSET];
        cur = cur == -1 ? 0 : cur;
        int tmp = cur;
        while (tmp != 0 && tr[tmp].nd.type != 0) {
            if (tr[tmp].nd.type) {
                //todo: add void place estimate to avoid drop an extra piece when searching
                update_type(dfs_status.row_type[row_index], tr[tmp].nd.type);
                increment_void(dfs_status.row_increment[row_index], &tr[tmp].nd, k);
            }
            tmp = tr[tmp].fail;
        }
    }
    for (int i = 1; i < PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] += dfs_status.row_type[row_index][i];
    }
}

inline void update_line_type_col(int col_index) {
    for (int32_t i = 1; i <= PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] -= dfs_status.col_type[col_index][i];
    }
    for (int32_t j = 1; j <= PATTERN_TYPES; ++j) {
        dfs_status.col_type[col_index][j] = 0;
        for (int32_t k = 0; k < BOARD_SIZE; ++k) {
            dfs_status.col_increment[col_index][k][j] = 0;
        }
    }

    int cur = 0;

    int32_t left, right;

    left = dfs_status.board[0][col_index] == VOID ? VOID : 0 - dfs_status.board[0][col_index];
    right = dfs_status.board[BOARD_SIZE - 1][col_index] == VOID ?
            VOID : 0 - dfs_status.board[BOARD_SIZE - 1][col_index];

    for (int k = -1; k <= BOARD_SIZE; ++k) {
        int ch = k == -1 ? left : k == BOARD_SIZE ? right : dfs_status.board[k][col_index];
        while (tr[cur].trans[ch + COLOR_OFFSET] == -1 && cur != 0) {
            cur = tr[cur].fail;
        }
        cur = tr[cur].trans[ch + COLOR_OFFSET];
        cur = cur == -1 ? 0 : cur;
        int tmp = cur;
        while (tmp != 0 && tr[tmp].nd.type != 0) {
            if (tr[tmp].nd.type) {
                update_type(dfs_status.col_type[col_index], tr[tmp].nd.type);
                increment_void(dfs_status.col_increment[col_index], &tr[tmp].nd, k);
            }
            tmp = tr[tmp].fail;
        }
    }
    for (int i = 1; i < PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] += dfs_status.col_type[col_index][i];
    }
}

inline void update_line_type_oblique_sum(int oblique_sum_index) {
    for (int32_t i = 1; i <= PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] -= dfs_status.oblique_sum_type[oblique_sum_index][i];
    }
    for (int32_t j = 1; j <= PATTERN_TYPES; ++j) {
        dfs_status.oblique_sum_type[oblique_sum_index][j] = 0;
        for (int32_t k = 0; k < BOARD_SIZE; ++k) {
            dfs_status.oblique_sum_increment[oblique_sum_index][k][j] = 0;
        }
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

    int32_t left, right;

    left = dfs_status.board[oblique_sum_index - start][start] == VOID ?
           VOID : 0 - dfs_status.board[oblique_sum_index - start][start];
    right = dfs_status.board[oblique_sum_index - end][end] == VOID ?
            VOID : 0 - dfs_status.board[oblique_sum_index - start][start];

    for (int k = start; k <= end; ++k) {
        int ch = k == -1 ? left : k == BOARD_SIZE ? right : dfs_status.board[oblique_sum_index - k][k];
        while (tr[cur].trans[ch + COLOR_OFFSET] == -1 && cur != 0) {
            cur = tr[cur].fail;
        }
        cur = tr[cur].trans[ch + COLOR_OFFSET];
        cur = cur == -1 ? 0 : cur;
        int tmp = cur;
        while (tmp != 0 && tr[tmp].nd.type != 0) {
            if (tr[tmp].nd.type) {
                update_type(dfs_status.oblique_sum_type[oblique_sum_index], tr[tmp].nd.type);
                increment_void(dfs_status.oblique_sum_increment[oblique_sum_index], &tr[tmp].nd, k);
            }
            tmp = tr[tmp].fail;
        }
    }

    for (int i = 0; i < PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] += dfs_status.oblique_sum_type[oblique_sum_index][i];
    }
}

inline void update_line_type_oblique_delta(int oblique_delta_index) {
    for (int32_t i = 1; i <= PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] -= dfs_status.oblique_delta_type[oblique_delta_index + BOARD_SIZE][i];
    }
    for (int32_t j = 1; j <= PATTERN_TYPES; ++j) {
        dfs_status.oblique_delta_type[oblique_delta_index + BOARD_SIZE][j] = 0;
        for (int32_t k = 0; k < BOARD_SIZE; ++k) {
            dfs_status.oblique_delta_increment[oblique_delta_index + BOARD_SIZE][k][j] = 0;
        }
    }

    int cur = 0;

    int start;
    int end;

    if (oblique_delta_index >= 0) {
        start = -1;
        end = BOARD_SIZE - oblique_delta_index;
    } else {
        start = -oblique_delta_index - 1;
        end = BOARD_SIZE;
    }

    int32_t left, right;

    left = dfs_status.board[oblique_delta_index + start][start] == VOID ?
           VOID : 0 - dfs_status.board[oblique_delta_index + start][start];
    right = dfs_status.board[oblique_delta_index + end][end] == VOID ?
            VOID : 0 - dfs_status.board[oblique_delta_index + end][end];

    for (int k = start; k <= end; ++k) {
        int ch = k == -1 ? left : k == BOARD_SIZE ? right : dfs_status.board[oblique_delta_index + k][k];
        while (tr[cur].trans[ch + COLOR_OFFSET] == -1 && cur != 0) {
            cur = tr[cur].fail;
        }
        cur = tr[cur].trans[ch + COLOR_OFFSET];
        cur = cur == -1 ? 0 : cur;
        int tmp = cur;
        while (tmp != 0 && tr[tmp].nd.type != 0) {
            if (tr[tmp].nd.type) {
                update_type(dfs_status.oblique_delta_type[oblique_delta_index + BOARD_SIZE], tr[tmp].nd.type);
                increment_void(dfs_status.oblique_delta_increment[oblique_delta_index + BOARD_SIZE], &tr[tmp].nd, k);
            }
            tmp = tr[tmp].fail;
        }
    }
    for (int i = 1; i < PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] += dfs_status.oblique_delta_type[oblique_delta_index + BOARD_SIZE][i];
    }
}

inline void update_grade(int i, int j) {

    ban_clear();

    update_line_type_row(i);
    update_line_type_col(j);
    update_line_type_oblique_sum(i + j);
    update_line_type_oblique_delta(i - j);

    ban_detect();
}


bool is_ban() {
    //todo: there are problems with ban check
    if (ban_cnt[6])
        return 1;
    if (ban_cnt[4] >= 2)
        return 1;
    if (ban_cnt[3] >= 2)
        return 1;
    return 0;
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

inline void ban_clear() {
    ban_cnt[3] = ban_cnt[4] = ban_cnt[6] = 0;

    ban_cnt[3] -= dfs_status.total_type[a3b] + dfs_status.total_type[sa3b];


    ban_cnt[4] -= dfs_status.total_type[a4b] + dfs_status.total_type[h4b] + dfs_status.total_type[sa4w3b] +
                  dfs_status.total_type[sa4n3b];

    ban_cnt[6] -= dfs_status.total_type[l6b];

}

inline void ban_detect() {
    ban_cnt[3] += dfs_status.total_type[a3b] + dfs_status.total_type[sa3b];


    ban_cnt[4] += dfs_status.total_type[a4b] + dfs_status.total_type[h4b] + dfs_status.total_type[sa4w3b] +
                  dfs_status.total_type[sa4n3b];

    ban_cnt[6] += dfs_status.total_type[l6b];
}

inline void increment_void(int64_t v[][PATTERN_TYPES + 5], __AC_node *p, int32_t index) {
    for (int32_t i = 0; p->back[i] != END; ++i) {
        if (index - p->back[i] >= 0)
            v[index + p->back[i]][p->type]++;
    }
}

inline int64_t pos_estimate(int i, int j, int8_t player_side) {
    int64_t ans = 0;
    for (int8_t t = 1; t <= PATTERN_TYPES; ++t) {
        int64_t shift = type_shift(t, player_side);
        ans += shift * dfs_status.row_increment[i][j][t];
        ans += shift * dfs_status.col_increment[j][i][t];
        ans += shift * dfs_status.oblique_sum_increment[i + j][j][t];
        ans += shift * dfs_status.oblique_delta_increment[i - j + BOARD_SIZE][j][t];
    }
    return ans;
}

inline int64_t type_shift(int8_t type, int8_t player_side) {
    if (player_side == WHITE) {
        switch (type) {
            case a2w:
                return CONTINUOUS_THREE;
            case a3w:
                return CONTINUOUS_FOUR;
            case a4w:
                return FIVE_GRADE;
            case h3w:
                return HALF_FOUR;
            case h4w:
                return FIVE_GRADE;
            case sa3w:
                return CONTINUOUS_FOUR;
            case sa4n3w:
                FIVE_GRADE;
            case sa4w3w:
                CONTINUOUS_FOUR;
            default:
                return 0;
        }
    } else {
        switch (type) {
            case a2b:
                return -CONTINUOUS_THREE;
            case a3b:
                return -CONTINUOUS_THREE;
            case a4b:
                return -FIVE_GRADE;
            case h3b:
                return -HALF_FOUR;
            case h4b:
                return -FIVE_GRADE;
            case sa3b:
                return CONTINUOUS_FOUR;
            case sa4n3b:
                return -FIVE_GRADE;
            case sa4w3b:
                return -CONTINUOUS_FOUR;
            default:
                return 0;
        }
    }
}

#undef TRIE_SIZE
#undef PATTERNS
#undef END
#undef MAX_PATTERN_LEN
#undef COLOR_OFFSET
#undef __WHITE
#undef __BLACK