//
// Created by Nine_yota on 2018-09-29.
//

#include "boardEvaluate.h"

#define TRIE_SIZE 50
extern int status_board[BOARD_SIZE][BOARD_SIZE];
extern int oblique_lines_1[2 * BOARD_SIZE][BOARD_SIZE];
extern int oblique_lines_2[2 * BOARD_SIZE][BOARD_SIZE];

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
            continuous5[j % 5] = status_board[i][j];
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
            continuous5[i % 5] = status_board[i][j];
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
        int i = min(offset, BOARD_SIZE - 1);
        int j = offset - i;
        for (; i >= 0 && j < BOARD_SIZE; --i, ++j) {
            sum -= continuous5[i % 5];
            continuous5[i % 5] = status_board[i][j];
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
        int i = max(0, offset);
        int j = i - offset;
        for (; i < BOARD_SIZE && j < BOARD_SIZE; ++i, ++j) {
            sum -= continuous5[i % 5];
            continuous5[i % 5] = status_board[i][j];
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
 *  011110/011110 -> 1000
 *  01110/01110 -> 100
 *  0110/0110 -> 10
 *
 * */

int get_status(int cur_color, int *array_to_querry, int array_len, int index) {
    if (index == -1 || index > array_len - 1)
        return 0 - cur_color;
    else
        return array_to_querry[index];
}

int OFFSET = 1;
typedef struct {
    //offset==1:
    int trans[3];
    int status;
} trie;

trie tr[TRIE_SIZE];
#define STR_TO_RECOGNIZE 22
#define END 10
#define MAX_STR_SIZE 8
int pattern[STR_TO_RECOGNIZE][MAX_STR_SIZE] = {
        //alive
        //white
        {VOID,  WHITE, WHITE, WHITE, WHITE, WHITE, VOID, END},
        {VOID,  WHITE, WHITE, WHITE, WHITE, VOID,  END},
        {VOID,  WHITE, WHITE, WHITE, VOID,  END},
        {VOID,  WHITE, WHITE, VOID,  END},
        {VOID,  WHITE, VOID,  END},
        //half alive
        //white
        {BLACK, WHITE, WHITE, WHITE, WHITE, VOID,  END},
        {VOID,  WHITE, WHITE, WHITE, WHITE, BLACK, END},
        {BLACK, WHITE, WHITE, WHITE, VOID,  END},
        {VOID,  WHITE, WHITE, WHITE, BLACK, END},
        {BLACK, WHITE, WHITE, VOID,  END},
        {VOID,  WHITE, WHITE, BLACK, END},
        //alive
        //black
        {VOID,  BLACK, BLACK, BLACK, BLACK, BLACK, VOID, END},
        {VOID,  BLACK, BLACK, BLACK, BLACK, VOID,  END},
        {VOID,  BLACK, BLACK, BLACK, VOID,  END},
        {VOID,  BLACK, BLACK, VOID,  END},
        {VOID,  BLACK, VOID,  END},
        //half alive
        //black
        {WHITE, BLACK, BLACK, BLACK, BLACK, VOID,  END},
        {VOID,  BLACK, BLACK, BLACK, BLACK, WHITE, END},
        {WHITE, BLACK, BLACK, BLACK, VOID,  END},
        {VOID,  BLACK, BLACK, BLACK, WHITE, END},
        {WHITE, BLACK, BLACK, VOID,  END},
        {VOID,  BLACK, BLACK, WHITE, END},
};

void build_trie() {
    int cnt = 0;
    for (int i = 0; i < TRIE_SIZE; ++i) {
        tr[i].status = 0;
        memset(tr[i].trans, 0, sizeof(tr[i].trans));
    }
    //build the trie here
    for (int i = 0; i < STR_TO_RECOGNIZE; ++i) {
        int cur = 0;
        for (int k = 0; pattern[i][k] == END; ++k) {
            if (tr[cur].trans[pattern[i][k]] == 0) {
                tr[cur].trans[pattern[i][k]] = ++cnt;
            }
            if (pattern[i][k + 1] == END) {
                tr[cnt].status = 1;
                break;
            }
            cur = tr[cur].trans[pattern[i][k]];
        }
    }
};
#undef STR_TO_RECOGNIZE
#undef END
#undef MAX_STR_SIZE

int grade_estimate() {
    //int grade = 0;
}

#undef TRIE_SIZE