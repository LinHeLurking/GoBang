//
// Created by Nine_yota on 2018-09-29.
//

#include "boardEvaluate.h"

#define __WHITE 1
#define __BLACK 0

long long type_grade[PATTERN_TYPES];
//long long last_bias[PATTERN_TYPES];

extern boardStatus status;
extern boardStatus dfs_status;
extern drop_record record[BOARD_SIZE * BOARD_SIZE + 5];

extern trie tr[TRIE_SIZE];
// values in ban_cnt are only valid right after calling update_grade()
int ban_cnt[7];

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
            continuous5[j % 5] = dfs_status.board[i][j];
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
            continuous5[i % 5] = dfs_status.board[i][j];
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
            continuous5[i % 5] = dfs_status.board[i][j];
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
            continuous5[i % 5] = dfs_status.board[i][j];
            sum += continuous5[i % 5];
            if (sum == 5)
                __flag = WHITE;
            else if (sum == -5)
                __flag = BLACK;
        }
    }


    int flag;
    if (dfs_status.total_type[a5w] || dfs_status.total_type[d5w] || dfs_status.total_type[h5w] ||
        dfs_status.total_type[l6w] || dfs_status.total_type[l6b]) {
        //return WHITE;
        flag = WHITE;
    } else if (dfs_status.total_type[a5b] || dfs_status.total_type[d5b] || dfs_status.total_type[h5b]) {
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

inline long long int grade_estimate(int player_side) {

    long long __grade = 0;

    if (player_side == WHITE) {
        for (int type = 1; type <= PATTERN_TYPES; ++type) {
            if (type_grade[type] <= 0)
                continue;
            __grade += dfs_status.total_type[type] *
                       (type_grade[type]);
        }
    } else {
        for (int type = 1; type <= PATTERN_TYPES; ++type) {
            if (type_grade[type] >= 0)
                continue;
            __grade += dfs_status.total_type[type] *
                       (type_grade[type]);
        }
    }

    __grade += last_bias_generator(player_side);

    return __grade;
}

inline void update_line_type_row(int row_index) {
    for (int i = 1; i <= PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] -= dfs_status.row_type[row_index][i];
    }
    for (int i = 1; i <= PATTERN_TYPES; ++i) {
        for (int k = 0; k < BOARD_SIZE; ++k) {
            dfs_status.total_increment[i] -= dfs_status.row_increment[row_index][k][i];
        }
    }
    for (int j = 1; j <= PATTERN_TYPES; ++j) {
        dfs_status.row_type[row_index][j] = 0;
        for (int k = 0; k < BOARD_SIZE; ++k) {
            dfs_status.row_increment[row_index][k][j] = 0;
        }
    }

    int cur = 0;

    int left, right;

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
                update_type(dfs_status.row_type[row_index], tr[tmp].nd.type);
                increment_void(dfs_status.row_increment[row_index], &tr[tmp].nd, k);
            }
            tmp = tr[tmp].fail;
        }
    }
    for (int i = 1; i <= PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] += dfs_status.row_type[row_index][i];
    }
    for (int i = 1; i <= PATTERN_TYPES; ++i) {
        for (int k = 0; k < BOARD_SIZE; ++k) {
            dfs_status.total_increment[i] += dfs_status.row_increment[row_index][k][i];
        }
    }
}

inline void update_line_type_col(int col_index) {
    for (int i = 1; i <= PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] -= dfs_status.col_type[col_index][i];
    }
    for (int i = 1; i <= PATTERN_TYPES; ++i) {
        for (int k = 0; k < BOARD_SIZE; ++k) {
            dfs_status.total_increment[i] -= dfs_status.col_increment[col_index][k][i];
        }
    }
    for (int j = 1; j <= PATTERN_TYPES; ++j) {
        dfs_status.col_type[col_index][j] = 0;
        for (int k = 0; k < BOARD_SIZE; ++k) {
            dfs_status.col_increment[col_index][k][j] = 0;
        }
    }

    int cur = 0;

    int left, right;

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
    for (int i = 1; i <= PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] += dfs_status.col_type[col_index][i];
    }
    for (int i = 1; i <= PATTERN_TYPES; ++i) {
        for (int k = 0; k < BOARD_SIZE; ++k) {
            dfs_status.total_increment[i] += dfs_status.col_increment[col_index][k][i];
        }
    }
}

inline void update_line_type_oblique_sum(int oblique_sum_index) {
    for (int i = 1; i <= PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] -= dfs_status.oblique_sum_type[oblique_sum_index][i];
    }
    for (int type = 1; type <= PATTERN_TYPES; ++type) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            dfs_status.total_increment[type] -= dfs_status.oblique_sum_increment[oblique_sum_index][j][type];
        }
    }
    for (int j = 1; j <= PATTERN_TYPES; ++j) {
        dfs_status.oblique_sum_type[oblique_sum_index][j] = 0;
        for (int k = 0; k < BOARD_SIZE; ++k) {
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

    int left, right;

    left = dfs_status.board[oblique_sum_index - start - 1][start + 1] == VOID ?
           VOID : 0 - dfs_status.board[oblique_sum_index - start][start];
    right = dfs_status.board[oblique_sum_index - end + 1][end - 1] == VOID ?
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

    for (int i = 1; i <= PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] += dfs_status.oblique_sum_type[oblique_sum_index][i];
    }
    for (int type = 1; type <= PATTERN_TYPES; ++type) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            dfs_status.total_increment[type] += dfs_status.oblique_sum_increment[oblique_sum_index][j][type];
        }
    }
}

inline void update_line_type_oblique_delta(int oblique_delta_index) {
    for (int type = 1; type <= PATTERN_TYPES; ++type) {
        dfs_status.total_type[type] -= dfs_status.oblique_delta_type[oblique_delta_index + BOARD_SIZE][type];
    }
    for (int type = 1; type <= PATTERN_TYPES; ++type) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            dfs_status.total_increment[type] -=
                    dfs_status.oblique_delta_increment[oblique_delta_index + BOARD_SIZE][j][type];
        }
    }
    for (int type = 1; type <= PATTERN_TYPES; ++type) {
        dfs_status.oblique_delta_type[oblique_delta_index + BOARD_SIZE][type] = 0;
        for (int k = 0; k < BOARD_SIZE; ++k) {
            dfs_status.oblique_delta_increment[oblique_delta_index + BOARD_SIZE][k][type] = 0;
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

    int left, right;

    left = dfs_status.board[oblique_delta_index + start + 1][start + 1] == VOID ?
           VOID : 0 - dfs_status.board[oblique_delta_index + start][start];
    right = dfs_status.board[oblique_delta_index + end - 1][end - 1] == VOID ?
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
    for (int i = 1; i <= PATTERN_TYPES; ++i) {
        dfs_status.total_type[i] += dfs_status.oblique_delta_type[oblique_delta_index + BOARD_SIZE][i];
    }
    for (int type = 1; type <= PATTERN_TYPES; ++type) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            dfs_status.total_increment[type] +=
                    dfs_status.oblique_delta_increment[oblique_delta_index + BOARD_SIZE][j][type];
        }
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
    //todo: there are problems with ban check, really.
    if (ban_cnt[6] > 0)
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
    type_grade[s4n3w] = SPLIT_ALIVE_FOUR_WITHOUT3;
    type_grade[s4n3b] = -SPLIT_ALIVE_FOUR_WITHOUT3;
    type_grade[s4w3w] = SPLIT_ALIVE_FOUR_WITH3;
    type_grade[s4w3b] = -SPLIT_ALIVE_FOUR_WITH3;

    /*
    SET0(last_bias);
    //pay attention that signs for players are different from type grades because these are used by adding.
    last_bias[a3w] = (int) (-0.8 * CONTINUOUS_THREE);
    last_bias[a3b] = (int) (0.8 * CONTINUOUS_THREE);
    last_bias[a2w] = (int) (-0.8 * CONTINUOUS_TWO);
    last_bias[a2b] = (int) (0.8 * CONTINUOUS_TWO);

    last_bias[h4w] = (int) (-0.8 * HALF_FOUR);
    last_bias[h4b] = (int) (0.8 * HALF_FOUR);
    last_bias[h3w] = (int) (-0.8 * HALF_THREE);
    last_bias[h3b] = (int) (0.8 * HALF_THREE);
    last_bias[h2w] = (int) (-0.8 * HALF_TWO);
    last_bias[h2b] = (int) (0.8 * HALF_TWO);

    last_bias[sa3w] = (int) (-0.8 * SPLIT_ALIVE_THREE);
    last_bias[sa3b] = (int) (0.8 * SPLIT_ALIVE_THREE);
    last_bias[s4n3w] = (int) (-0.8 * SPLIT_ALIVE_FOUR_WITHOUT3);
    last_bias[s4n3b] = (int) (0.8 * SPLIT_ALIVE_FOUR_WITHOUT3);
    last_bias[s4w3w] = (int) (-0.8 * SPLIT_ALIVE_FOUR_WITH3);
    last_bias[s4w3b] = (int) (0.8 * SPLIT_ALIVE_FOUR_WITH3);
     */
}

inline void update_type(int *type_array, int type) {
    switch (type) {
        case s4w3w:
            ++type_array[s4w3w];
            --type_array[a3w];
            break;
        case s4w3b:
            ++type_array[s4w3b];
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


    ban_cnt[4] -= dfs_status.total_type[a4b] + dfs_status.total_type[h4b] + dfs_status.total_type[s4w3b] +
                  dfs_status.total_type[s4n3b];

    ban_cnt[6] -= dfs_status.total_type[l6b];

}

inline void ban_detect() {
    ban_cnt[3] += dfs_status.total_type[a3b] + dfs_status.total_type[sa3b];


    ban_cnt[4] += dfs_status.total_type[a4b] + dfs_status.total_type[h4b] + dfs_status.total_type[s4w3b] +
                  dfs_status.total_type[s4n3b];

    ban_cnt[6] += dfs_status.total_type[l6b];
}

inline void increment_void(long long v[][PATTERN_TYPES + 5], __AC_node *p, int index) {
    for (int i = 0; p->back[i] != END; ++i) {
        if (index + p->back[i] >= 0)
            v[index + p->back[i]][p->blank_type[i]]++;
    }
}

inline long long int pos_estimate(int i, int j, int player_side) {
    long long ans = 0;
    for (int t = 1; t <= PATTERN_TYPES; ++t) {
        if (type_grade[t] * player_side < 0)continue;
        ans += type_grade[t] * dfs_status.row_increment[i][j][t];
        ans += type_grade[t] * dfs_status.col_increment[j][i][t];
        ans += type_grade[t] * dfs_status.oblique_sum_increment[i + j][j][t];
        ans += type_grade[t] * dfs_status.oblique_delta_increment[i - j + BOARD_SIZE][j][t];
    }
    return ans;
}

inline long long int type_shift(int type, int player_side) {
    if (player_side == WHITE) {
        switch (type) {
            case a1w:
                return CONTINUOUS_TWO;
            case a2w:
                return CONTINUOUS_THREE;
            case a3w:
                return CONTINUOUS_FOUR;
            case a4w:
                return FIVE_GRADE;
            case a5w:
                return FIVE_GRADE;
            case h2w:
                return HALF_THREE;
            case h3w:
                return HALF_FOUR;
            case h4w:
                return FIVE_GRADE;
            case h5w:
                return FIVE_GRADE;
            case d5w:
                return FIVE_GRADE;
            case l6w:
                return LONG_CONTINUOUS;
            case sa3w:
                return CONTINUOUS_FOUR;
            case s4n3w:
                FIVE_GRADE;
            case s4w3w:
                FIVE_GRADE;
            default:
                return 0;
        }
    } else {
        switch (type) {
            case a1b:
                return -CONTINUOUS_TWO;
            case a2b:
                return -CONTINUOUS_THREE;
            case a3b:
                return -CONTINUOUS_FOUR;
            case a4b:
                return -FIVE_GRADE;
            case a5b:
                return -FIVE_GRADE;
            case h2b:
                return -HALF_THREE;
            case h3b:
                return -HALF_FOUR;
            case h4b:
                return -FIVE_GRADE;
            case h5b:
                return -FIVE_GRADE;
            case d5b:
                return -FIVE_GRADE;
            case sa3b:
                return -CONTINUOUS_FOUR;
            case s4n3b:
                return -FIVE_GRADE;
            case s4w3b:
                return -FIVE_GRADE;
            case l6b:
                return LONG_CONTINUOUS;
            default:
                return 0;
        }
    }
}

inline long long int last_bias_generator(int player_side) {
    //todo: finish bias check here.
    long long __ret = 0;
    if (player_side == WHITE && record[dfs_status.steps].player == WHITE) {
        if (dfs_status.total_type[a3b] &&
            !(dfs_status.total_type[h4w] || dfs_status.total_type[a4w])) {
            __ret += 2 * type_grade[a3b];
        }
        if (dfs_status.total_type[sa3b] &&
            !(dfs_status.total_type[h4w] || dfs_status.total_type[a4w])) {
            __ret += 2 * type_grade[sa3b];
        }
        if (dfs_status.total_type[h4b] &&
            !(dfs_status.total_type[a5w] || dfs_status.total_type[h5w] ||
              dfs_status.total_type[d5w] || dfs_status.total_type[l6w])) {
            __ret += 2 * type_grade[h4b];
        }
    } else if (player_side == BLACK && record[dfs_status.steps].player == BLACK) {
        if (dfs_status.total_type[a3w] &&
            !(dfs_status.total_type[h4b] || dfs_status.total_type[a4b])) {
            __ret += 2 * type_grade[a3w];
        }
        if (dfs_status.total_type[sa3w] &&
            !(dfs_status.total_type[h4b] || dfs_status.total_type[a4b])) {
            __ret += 2 * type_grade[sa3w];
        }
        if (dfs_status.total_type[h4w] &&
            !(dfs_status.total_type[a5b] || dfs_status.total_type[h5b] || dfs_status.total_type[d5b])) {
            __ret += 2 * type_grade[h4w];
        }
    }

    return __ret;
}

#undef TRIE_SIZE
#undef PATTERNS
#undef END
#undef MAX_PATTERN_LEN
#undef COLOR_OFFSET
#undef __WHITE
#undef __BLACK