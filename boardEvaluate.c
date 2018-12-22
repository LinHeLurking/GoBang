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
//todo: add type cnt here

//1==white wins -1== black wins 0==no one wins
int winner_check() {
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
                return 1;
            else if (sum == -5)
                return -1;
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
                return 1;
            else if (sum == -5)
                return -1;
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
                return 1;
            else if (sum == -5)
                return -1;
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
                return 1;
            else if (sum == -5)
                return -1;
        }
    }
    return 0;
}


inline int64_t pos_estimate(int i, int j, int player_side) {
    //todo: add pos estimate here.
}


inline int64_t grade_estimate() {
    int64_t _grade = 0;
    for (int i = 0; i < PATTERN_TYPES; ++i) {
        _grade += dfs_status.total_type[i] * type_grade[i];
    }
    return _grade;
}

inline void update_line_grade_row(int row_index, int player_side) {
    for (int i = 1; i < PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] -= dfs_status.row_type[i];
    }
    SET0(dfs_status.row_type);
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
                ++dfs_status.row_type[tr[tmp].type];
            }
            tmp = tr[tmp].fail;
        }
    }
    for (int i = 1; i < PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] += dfs_status.row_type[i];
    }
}

inline void update_line_grade_col(int col_index, int player_side) {
    for (int i = 1; i < PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] -= dfs_status.col_type[i];
    }
    SET0(dfs_status.col_type);
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
                ++dfs_status.col_type[tr[tmp].type];
            }
            tmp = tr[tmp].fail;
        }
    }
    for (int i = 1; i < PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] += dfs_status.col_type[i];
    }
}

inline void update_line_grade_oblique_sum(int oblique_sum_index, int player_side) {
    for (int i = 1; i < PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] -= dfs_status.oblique_sum_type[i];
    }

    SET0(dfs_status.oblique_sum_type);
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
                ++dfs_status.oblique_sum_type[tr[tmp].type];
            }
            tmp = tr[tmp].fail;
        }
    }

    for (int i = 0; i < PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] += dfs_status.oblique_sum_type[i];
    }
}

inline void update_line_grade_oblique_delta(int oblique_delta_index, int player_side) {
    for (int i = 1; i < PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] -= dfs_status.oblique_delta_type[i];
    }

    SET0(dfs_status.oblique_delta_type);
    int cur = 0;

    int start;
    int end;

    if (oblique_delta_index >= 0) {
        start = -1;
        end = BOARD_SIZE - oblique_delta_index;
    } else {
        start = oblique_delta_index - 1;
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
                ++dfs_status.oblique_delta_type[tr[tmp].type];
            }
            tmp = tr[tmp].fail;
        }
    }
    for (int i = 1; i < PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] -= dfs_status.oblique_delta_type[i];
    }
}

inline void update_grade(int i, int j) {

    ban_cnt[3] = ban_cnt[4] = ban_cnt[6] = 0;

    ban_cnt[3] -=
            dfs_status.row_type[a3b] + dfs_status.row_type[sa3b] + dfs_status.col_type[a3b] + dfs_status.col_type[sa3b]
            + dfs_status.oblique_sum_type[a3b] + dfs_status.oblique_sum_type[sa3b] + dfs_status.oblique_delta_type[a3b]
            + dfs_status.oblique_delta_type[sa3b];

    ban_cnt[4] -=
            dfs_status.row_type[a4b] + dfs_status.row_type[h4b] + dfs_status.row_type[sa4w3b] +
            dfs_status.row_type[sa4n3b]
            + dfs_status.col_type[a4b] + dfs_status.col_type[h4b] + dfs_status.col_type[sa4w3b] +
            dfs_status.col_type[sa4n3b]
            + dfs_status.oblique_sum_type[a4b] + dfs_status.oblique_sum_type[h4b] +
            dfs_status.oblique_sum_type[sa4w3b] + dfs_status.oblique_sum_type[sa4n3b]
            + dfs_status.oblique_delta_type[a4b] + dfs_status.oblique_delta_type[h4b] +
            dfs_status.oblique_delta_type[sa4w3b] + dfs_status.oblique_delta_type[sa4n3b];

    ban_cnt[6] -= dfs_status.row_type[l6b] + dfs_status.col_type[l6b] + dfs_status.oblique_sum_type[l6b] +
                  dfs_status.oblique_delta_type[l6b];

    update_line_grade_row(i, WHITE);
    update_line_grade_col(j, WHITE);
    update_line_grade_oblique_sum(i + j, WHITE);
    update_line_grade_oblique_delta(i - j, WHITE);

    update_line_grade_row(i, BLACK);
    update_line_grade_col(j, BLACK);
    update_line_grade_oblique_sum(i + j, BLACK);
    update_line_grade_oblique_delta(i - j, BLACK);

    ban_cnt[3] +=
            dfs_status.row_type[a3b] + dfs_status.row_type[sa3b] + dfs_status.col_type[a3b] + dfs_status.col_type[sa3b]
            + dfs_status.oblique_sum_type[a3b] + dfs_status.oblique_sum_type[sa3b] + dfs_status.oblique_delta_type[a3b]
            + dfs_status.oblique_delta_type[sa3b];

    ban_cnt[4] += dfs_status.row_type[a4b] + dfs_status.row_type[h4b] + dfs_status.row_type[sa4w3b] +
                  dfs_status.row_type[sa4n3b]
                  + dfs_status.col_type[a4b] + dfs_status.col_type[h4b] + dfs_status.col_type[sa4w3b] +
                  dfs_status.col_type[sa4n3b]
                  + dfs_status.oblique_sum_type[a4b] + dfs_status.oblique_sum_type[h4b] +
                  dfs_status.oblique_sum_type[sa4w3b] + dfs_status.oblique_sum_type[sa4n3b]
                  + dfs_status.oblique_delta_type[a4b] + dfs_status.oblique_delta_type[h4b] +
                  dfs_status.oblique_delta_type[sa4w3b] + dfs_status.oblique_delta_type[sa4n3b];

    ban_cnt[6] += dfs_status.row_type[l6b] + dfs_status.col_type[l6b] + dfs_status.oblique_sum_type[l6b] +
                  dfs_status.oblique_delta_type[l6b];

}


uint8_t is_ban() {
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

#undef TRIE_SIZE
#undef PATTERNS
#undef END
#undef MAX_PATTERN_LEN
#undef COLOR_OFFSET