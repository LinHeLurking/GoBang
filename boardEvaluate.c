//
// Created by Nine_yota on 2018-09-29.
//

#include "boardEvaluate.h"


#define TRIE_SIZE 100
#define STATUS_NUM 3
#define STR_TO_RECOGNIZE 28
#define END 7
#define MAX_STR_SIZE 8
#define OFFSET 1


extern boardStatus status;
extern boardStatus dfs_status;

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


/*  evaluate the grade of this status board using AC auto-machine
 *  0111110 -> 100000
 *  011110 -> 10000
 *  01110 -> 1000
 *  0110 -> 100
 *  010 -> 10
 *
 *  01111-1/-111110 -> 1000
 *  0111-1/-11110 -> 100
 *  011-1/-1110 -> 10
 *
 * */

int grade[STR_TO_RECOGNIZE] = {100000, 10000, 1000, 100, 10, 100000, 100000, 1100, 1100, 100, 100, 10, 10, -100000,
                               -10000, -1000, -100, -10, -100000, -100000, -1100, -1100, -100, -100, -10, -10, 100000,
                               -100000};
//cautions!! if you change this array, the grade[] also needs changing
int pattern[STR_TO_RECOGNIZE][MAX_STR_SIZE] = {
        //WHITE: 0 <= index < 11 BLACK: 11 <= index < STR_TO_RECOGNIZE

        //alive
        //white
        {VOID,  WHITE, WHITE, WHITE, WHITE, WHITE, VOID,  END},//100000
        {VOID,  WHITE, WHITE, WHITE, WHITE, VOID,  END},//10000
        {VOID,  WHITE, WHITE, WHITE, VOID,  END},//1000
        {VOID,  WHITE, WHITE, VOID,  END},//100
        {VOID,  WHITE, VOID,  END},//10
        //half alive
        //white
        {BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, VOID,  END},//100000
        {VOID,  WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, END},//100000
        {BLACK, WHITE, WHITE, WHITE, WHITE, VOID,  END},//1100
        {VOID,  WHITE, WHITE, WHITE, WHITE, BLACK, END},//1100
        {BLACK, WHITE, WHITE, WHITE, VOID,  END},//100
        {VOID,  WHITE, WHITE, WHITE, BLACK, END},//100
        {BLACK, WHITE, WHITE, VOID,  END},//10
        {VOID,  WHITE, WHITE, BLACK, END},//10

        //alive
        //black
        {VOID,  BLACK, BLACK, BLACK, BLACK, BLACK, VOID,  END},//-100000
        {VOID,  BLACK, BLACK, BLACK, BLACK, VOID,  END},//-10000
        {VOID,  BLACK, BLACK, BLACK, VOID,  END},//-1000
        {VOID,  BLACK, BLACK, VOID,  END},//-100
        {VOID,  BLACK, VOID,  END},//-10
        //half alive
        //black
        {WHITE, BLACK, BLACK, BLACK, BLACK, BLACK, VOID,  END},//-100000
        {VOID,  BLACK, BLACK, BLACK, BLACK, BLACK, WHITE, END},//-100000
        {WHITE, BLACK, BLACK, BLACK, BLACK, VOID,  END},//-1100
        {VOID,  BLACK, BLACK, BLACK, BLACK, WHITE, END},//-1100
        {WHITE, BLACK, BLACK, BLACK, VOID,  END},//-100
        {VOID,  BLACK, BLACK, BLACK, WHITE, END},//-100
        {WHITE, BLACK, BLACK, VOID,  END},//-10
        {VOID,  BLACK, BLACK, WHITE, END},//-10

        //totally dead five
        {BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, END},//100000
        {WHITE, BLACK, BLACK, BLACK, BLACK, BLACK, WHITE, END}//-100000

};


trie tr[TRIE_SIZE];

int root = 0;
int count = 0;

void AC_init() {
    root = count = 0;
    for (int i = 0; i < TRIE_SIZE; ++i) {
        tr[i].grade = 0;
        tr[i].fail = -1;
        for (int j = 0; j < STATUS_NUM; ++j) {
            tr[i].trans[j] = -1;
        }
    }
}

void insert(int *s, int grade) {
    int p = root;
    for (int *c = s; *c != END; ++c) {
        int tmp = (*c);
        if (tr[p].trans[tmp + OFFSET] == -1) {
            tr[p].trans[tmp + OFFSET] = ++count;
        }
        p = tr[p].trans[tmp + OFFSET];
        if ((*(c + 1)) == END)
            tr[p].grade = grade;
    }
}


void build_AC_fail() {
    int AC_list[TRIE_SIZE];
    memset(AC_list, 0, sizeof(AC_list));
    int head = 0, tail = 0;
    AC_list[tail++] = root;
    while (head < tail) {
        int cur = AC_list[head++];
        for (int i = 0; i < STATUS_NUM; ++i) {
            if (tr[cur].trans[i] != -1) {
                if (cur == root) { tr[tr[cur].trans[i]].fail = root; }
                else {
                    int p = tr[cur].fail;
                    while (p != -1) {
                        if (tr[p].trans[i] != -1) {
                            tr[tr[cur].trans[i]].fail = tr[p].trans[i];
                            break;
                        }
                        p = tr[p].fail;
                    }
                    if (p == -1) { tr[tr[cur].trans[i]].fail = root; }
                }
                AC_list[tail++] = tr[cur].trans[i];
            }
        }
    }
}

void AC_build() {
    AC_init();
    for (int i = 0; i < STR_TO_RECOGNIZE; ++i) {
        insert(pattern[i], grade[i]);
    }
    build_AC_fail();
}


int pos_estimate(int i, int j, int player_side) {
    int grade = 0;
    int cur = 0;
    for (int k = -1; k <= BOARD_SIZE; ++k) {
        int ch = k == -1 || k == BOARD_SIZE ? 0 - player_side : dfs_status.board[i][k];
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

    cur = 0;
    for (int k = -1; k <= BOARD_SIZE; ++k) {
        int ch = k == -1 || k == BOARD_SIZE ? 0 - player_side : dfs_status.board[k][j];
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

    cur = 0;
    int sum = i + j;
    for (int k = -1; k <= BOARD_SIZE; ++k) {
        int ch = k == int_max(-1, sum - 15) || k == min(sum + 1, BOARD_SIZE) ? 0 - player_side
                                                                             : dfs_status.oblique_lines_1[sum][k];
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

    cur = 0;
    int delta = i - j;
    for (int k = -1; k <= BOARD_SIZE; ++k) {
        int ch = delta + k == -1 || k == BOARD_SIZE ? 0 - player_side : dfs_status.oblique_lines_2[
                delta + BOARD_SIZE][k];
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
    return grade;
}


int grade_estimate(int player_side) {
    //TODO: this function needs modified to a incremental method!(using pos_estimate)
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
            //int_max(-1, sum - 15)
            int ch = j == int_max(-1, sum - 15) || j == int_min(sum + 1, BOARD_SIZE) ? 0 - player_side
                                                                                     : dfs_status.oblique_lines_1[sum][j];
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
            int ch = j == int_max(-1, -1 - delta) || j == int_min(15 - delta, BOARD_SIZE) ? 0 - player_side
                                                                                          : dfs_status.oblique_lines_2[
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
}


#undef TRIE_SIZE
#undef STR_TO_RECOGNIZE
#undef END
#undef MAX_STR_SIZE
#undef OFFSET