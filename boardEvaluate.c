//
// Created by Nine_yota on 2018-09-29.
//

#include "boardEvaluate.h"


extern boardStatus status;
extern boardStatus dfs_status;
extern trie tr[TRIE_SIZE];


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


inline long long pos_estimate(int i, int j, int player_side) {
    return dfs_status.row_grade[PLAYER_IN_LINE][i] + dfs_status.col_grade[PLAYER_IN_LINE][j] +
           dfs_status.oblique_sum_grade[PLAYER_IN_LINE][i + j] +
           dfs_status.oblique_delta_grade[PLAYER_IN_LINE][i - j + BOARD_SIZE];
}


inline long long grade_estimate(int player_side) {
    long long _grade = 0;
    _grade = dfs_status.total_grade[PLAYER_IN_LINE];
    return _grade;
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
            }
            tmp = tr[tmp].fail;
        }
    }
    dfs_status.total_grade[PLAYER_IN_LINE] -= dfs_status.oblique_delta_grade[PLAYER_IN_LINE][oblique_delta_index +
                                                                                             BOARD_SIZE];
    dfs_status.total_grade[PLAYER_IN_LINE] += _grade;

    dfs_status.oblique_delta_grade[PLAYER_IN_LINE][oblique_delta_index + BOARD_SIZE] = _grade;

}



#undef TRIE_SIZE
#undef PATTERNS
#undef END
#undef MAX_PATTERN_LEN
#undef COLOR_OFFSET