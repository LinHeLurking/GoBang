//
// Created by Nine_yota on 2018-09-29.
//

#include "boardEvaluate.h"


extern boardStatus status;
extern boardStatus dfs_status;
extern drop_record record[BOARD_SIZE * BOARD_SIZE + 5];
//extern trie tr[TRIE_SIZE];


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


int pos_estimate(int i, int j, int player_side) {
    int _grade = 0;
    _grade += dfs_status.row_grade[PLAYER_IN_LINE][i] + dfs_status.col_grade[PLAYER_IN_LINE][j];
    _grade += dfs_status.oblique_sum_grade[PLAYER_IN_LINE][i + j] +
              dfs_status.oblique_delta_grade[PLAYER_IN_LINE][i - j + BOARD_SIZE];
    return _grade;
}


long long grade_estimate(int player_side) {

    /*
    //grades in nodes of trie is for WHITE if they are larger than 0, and vice versa.
    int grade = 0;
    //loop for row
    for (int i = 0; i < BOARD_SIZE; ++i) {
        int cur = 0;
        for (int j = -1; j <= BOARD_SIZE; ++j) {
            int ch = j == -1 || j == BOARD_SIZE ? 0 - player_side : dfs_status.board[i][j];
            while (tr[cur].trans[ch + OFFSET] == -1 && cur != 0) {
                cur = tr[cur].fail;
            }
            cur = tr[cur].trans[ch + OFFSET];
            cur = cur == -1 ? 0 : cur;
            int tmp = cur;
            while (tmp != 0 && tr[tmp].grade != 0) {
                if ((player_side == WHITE && tr[tmp].grade > 0) || (player_side == BLACK && tr[tmp].grade < 0)) {
                    grade += tr[tmp].grade;
                }
                tmp = tr[tmp].fail;
            }
        }
    }
    //loop in col
    for (int j = 0; j < BOARD_SIZE; ++j) {
        int cur = 0;
        for (int i = -1; i <= BOARD_SIZE; ++i) {
            int ch = i == -1 || i == BOARD_SIZE ? 0 - player_side : dfs_status.board[i][j];
            while (tr[cur].trans[ch + OFFSET] == -1 && cur != 0) {
                cur = tr[cur].fail;
            }
            cur = tr[cur].trans[ch + OFFSET];
            cur = cur == -1 ? 0 : cur;
            int tmp = cur;
            while (tmp != 0 && tr[tmp].grade != 0) {
                if ((player_side == WHITE && tr[tmp].grade > 0) || (player_side == BLACK && tr[tmp].grade < 0)) {
                    grade += tr[tmp].grade;
                }
                tmp = tr[tmp].fail;
            }
        }
    }


    //loop in oblique
    //sum==i+j delta=i-j
    for (int sum = 0; sum < 2 * BOARD_SIZE; ++sum) {
        int cur = 0;
        for (int j = -1; j <= BOARD_SIZE; ++j) {
            int ch = j == -1 || j == min(sum + 1, BOARD_SIZE) ? 0 - player_side
                                                              : dfs_status.oblique_line_sum[sum][j];
            while (tr[cur].trans[ch + OFFSET] == -1 && cur != 0) {
                cur = tr[cur].fail;
            }
            cur = tr[cur].trans[ch + OFFSET];
            cur = cur == -1 ? 0 : cur;
            int tmp = cur;
            while (tmp != 0 && tr[tmp].grade != 0) {
                if ((player_side == WHITE && tr[tmp].grade > 0) || (player_side == BLACK && tr[tmp].grade < 0)) {
                    grade += tr[tmp].grade;
                }
                tmp = tr[tmp].fail;
            }
        }
    }
    for (int delta = -14; delta < BOARD_SIZE; ++delta) {
        int cur = 0;
        for (int j = -1; j <= BOARD_SIZE; ++j) {
            int ch = delta + j == -1 || j == BOARD_SIZE ? 0 - player_side : dfs_status.oblique_line_delta[
                    delta + BOARD_SIZE][j];
            while (tr[cur].trans[ch + OFFSET] == -1 && cur != 0) {
                cur = tr[cur].fail;
            }
            cur = tr[cur].trans[ch + OFFSET];
            cur = cur == -1 ? 0 : cur;
            int tmp = cur;
            while (tmp != 0 && tr[tmp].grade != 0) {
                if ((player_side == WHITE && tr[tmp].grade > 0) || (player_side == BLACK && tr[tmp].grade < 0)) {
                    grade += tr[tmp].grade;
                }
                tmp = tr[tmp].fail;
            }
        }
    }
    return grade;
     */

    long long _grade = 0;
    _grade = dfs_status.total_grade[PLAYER_IN_LINE];

    /*
    for (int i = 0; i < BOARD_SIZE; ++i) {
        _grade += dfs_status.row_grade[PLAYER_IN_LINE][i];
    }
    for (int j = 0; j < BOARD_SIZE; ++j) {
        _grade += dfs_status.col_grade[PLAYER_IN_LINE][j];
    }
    for (int sum = 0; sum < 2 * BOARD_SIZE; ++sum) {
        _grade += dfs_status.oblique_sum_grade[PLAYER_IN_LINE][sum];
    }
    for (int delta = -14; delta < BOARD_SIZE; ++delta) {
        _grade += dfs_status.oblique_delta_grade[PLAYER_IN_LINE][delta + BOARD_SIZE];
    }
    */

#ifdef ESTIMATE_METHOD_DEBUG
    if (dfs_status.total_grade[PLAYER_IN_LINE] != _grade) {
        printf("Not equal!!\n");
    }
#endif

    return _grade;
}


#undef TRIE_SIZE
#undef STR_TO_RECOGNIZE
#undef END
#undef MAX_STR_SIZE
#undef OFFSET