//
// Created by Nine_yota on 2018-09-29.
//

#include "boardEvaluate.h"
#include "statusBoard.h"
#include "mathFunction.h"

#define DFS_DEPTH 4
#define TRIE_SIZE 50

extern int best_i;
extern int best_j;

extern int status_board[BOARD_SIZE][BOARD_SIZE];
extern int oblique_lines_1[2 * BOARD_SIZE][BOARD_SIZE];
extern int oblique_lines_2[2 * BOARD_SIZE][BOARD_SIZE];

extern int dfs_status_board[BOARD_SIZE][BOARD_SIZE];
extern int dfs_oblique_lines_1[2 * BOARD_SIZE][BOARD_SIZE];
extern int dfs_oblique_lines_2[2 * BOARD_SIZE][BOARD_SIZE];

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

int get_status(int player_side, int *array_to_querry, int array_len, int index) {
    if (index == -1 || index > array_len - 1)
        return 0 - player_side;
    else
        return array_to_querry[index];
}

typedef struct {
    //offset==1:
    int trans[3];
    int grade;
} trie;

trie tr[TRIE_SIZE];
#define STR_TO_RECOGNIZE 22
#define END 11
#define MAX_STR_SIZE 8
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
#define OFFSET 1
int grade[STR_TO_RECOGNIZE] = {100000, 10000, 1000, 100, 10, 1000, 1000, 100, 100, 10, 10, -100000, -10000, -1000, -100,
                               -10, -1000, -1000, -100, -100, -10, -10};
//cautions!! if you change this array, the grade[] also needs changing
int pattern[STR_TO_RECOGNIZE][MAX_STR_SIZE] = {
        //WHITE: 0 <= index < 11 BLACK: 11 <= index < STR_TO_RECOGNIZE

        //alive
        //white
        {VOID,  WHITE, WHITE, WHITE, WHITE, WHITE, VOID, END},//100000
        {VOID,  WHITE, WHITE, WHITE, WHITE, VOID,  END},//10000
        {VOID,  WHITE, WHITE, WHITE, VOID,  END},//1000
        {VOID,  WHITE, WHITE, VOID,  END},//100
        {VOID,  WHITE, VOID,  END},//10
        //half alive
        //white
        {BLACK, WHITE, WHITE, WHITE, WHITE, VOID,  END},//1000
        {VOID,  WHITE, WHITE, WHITE, WHITE, BLACK, END},//1000
        {BLACK, WHITE, WHITE, WHITE, VOID,  END},//100
        {VOID,  WHITE, WHITE, WHITE, BLACK, END},//100
        {BLACK, WHITE, WHITE, VOID,  END},//10
        {VOID,  WHITE, WHITE, BLACK, END},//10

        //alive
        //black
        {VOID,  BLACK, BLACK, BLACK, BLACK, BLACK, VOID, END},//-100000
        {VOID,  BLACK, BLACK, BLACK, BLACK, VOID,  END},//-10000
        {VOID,  BLACK, BLACK, BLACK, VOID,  END},//-1000
        {VOID,  BLACK, BLACK, VOID,  END},//-100
        {VOID,  BLACK, VOID,  END},//-10
        //half alive
        //black
        {WHITE, BLACK, BLACK, BLACK, BLACK, VOID,  END},//-1000
        {VOID,  BLACK, BLACK, BLACK, BLACK, WHITE, END},//-1000
        {WHITE, BLACK, BLACK, BLACK, VOID,  END},//-100
        {VOID,  BLACK, BLACK, BLACK, WHITE, END},//-100
        {WHITE, BLACK, BLACK, VOID,  END},//-10
        {VOID,  BLACK, BLACK, WHITE, END},//-10
};

void build_trie() {
    //!! remember the OFFSET !!
    int cnt = 0;
    for (int i = 0; i < TRIE_SIZE; ++i) {
        tr[i].grade = 0;
        memset(tr[i].trans, 0, sizeof(tr[i].trans));
    }
    //build the trie here
    for (int i = 0; i < STR_TO_RECOGNIZE; ++i) {
        int cur = 0;
        for (int k = 0;; ++k) {
            if (tr[cur].trans[pattern[i][k] + OFFSET] == 0) {
                tr[cur].trans[pattern[i][k] + OFFSET] = ++cnt;
            }
            if (pattern[i][k + 1] == END) {
                tr[cnt].grade = grade[i];
                break;
            }
            cur = tr[cur].trans[pattern[i][k] + OFFSET];
        }
    }
};

void trie_test() {
    for (int i = 0; i < TRIE_SIZE; ++i) {
        printf("%d ", tr[i].grade);
        if ((i + 1) % 20 == 0)
            printf("\n");
    }
    printf("\n");
    for (int i = 0; i < STR_TO_RECOGNIZE; ++i) {
        printf("Testing the ");
        for (int j = 0; pattern[i][j] != END; ++j) {
            printf(" %d", pattern[i][j]);
        }
        printf(" \n\n");
        int cur = 0;
        for (int j = 0;; ++j) {
            if (pattern[i][j] == END) {
                if (tr[cur].grade != 0)
                    printf("match successfully\n");
                else
                    printf("match failed\n");
                break;
            }
            cur = tr[cur].trans[pattern[i][j] + OFFSET];
            printf("current 'cur': %d\n", cur);
        }
        printf("\n");
    }
}

int grade_estimate(int player_side) {
    //grades in nodes of trie is for WHITE if they are larger than 0, and vice versa.
    int grade = 0;
    //attention! when loop in oblique,loop must exceed the boundary of array using get_status()

    //loop for row
    for (int i = 0; i < BOARD_SIZE; ++i) {
        int cur = 0;
        for (int j = -1; j <= BOARD_SIZE; ++j) {
            cur = j == -1 || j == BOARD_SIZE ? tr[cur].trans[0 - player_side + OFFSET] : tr[cur].trans[
                    status_board[i][j] + OFFSET];
            if ((tr[cur].grade > 0 && player_side == WHITE) || (tr[cur].grade < 0 && player_side == BLACK)) {
                grade += tr[cur].grade;
                --j;
                continue;
            }
            if (cur == 0) {
                --j;
            }
        }
    }
    //loop in col
    for (int j = 0; j < BOARD_SIZE; ++j) {
        int cur = 0;
        for (int i = -1; i <= BOARD_SIZE; ++i) {
            cur = i == -1 || i == BOARD_SIZE ? tr[cur].trans[0 - player_side + OFFSET] : tr[cur].trans[
                    status_board[i][j] + OFFSET];
            if ((tr[cur].grade > 0 && player_side == WHITE) || (tr[cur].grade < 0 && player_side == BLACK)) {
                grade += tr[cur].grade;
                --i;
                continue;
            }
            if (cur == 0) {
                --i;
            }
        }
    }
    //loop in oblique
    //sum==i+j delta=i-j
    for (int sum = 0; sum < 2 * BOARD_SIZE; ++sum) {
        int cur = 0;
        for (int j = -1; j <= BOARD_SIZE; ++j) {
            cur = j == -1 || j == BOARD_SIZE ? tr[cur].trans[0 - player_side + OFFSET] : tr[cur].trans[
                    oblique_lines_1[sum][j] + OFFSET];
            if ((tr[cur].grade > 0 && player_side == WHITE) || (tr[cur].grade < 0 && player_side == BLACK)) {
                grade += tr[cur].grade;
                --j;
                continue;
            }
            if (cur == 0) {
                --j;
            }
        }
    }
    for (int delta = -14; delta < BOARD_SIZE; ++delta) {
        int cur = 0;
        for (int j = -1; j <= BOARD_SIZE; ++j) {
            cur = j == -1 || j == BOARD_SIZE ? tr[cur].trans[0 - player_side + OFFSET] : tr[cur].trans[
                    oblique_lines_2[delta + BOARD_SIZE][j] + OFFSET];
            if ((tr[cur].grade > 0 && player_side == WHITE) || (tr[cur].grade < 0 && player_side == BLACK)) {
                grade += tr[cur].grade;
                --j;
                continue;
            }
            if (cur == 0) {
                --j;
            }
        }
    }
    return grade;
}


#undef TRIE_SIZE
#undef STR_TO_RECOGNIZE
#undef END
#undef MAX_STR_SIZE


/*
 *  DFS starts here
 * */
#define MAX_POS 225
#define I 0
#define J 1
#define GRADE 2

void generate_possible_pos(int (*pos)[3], int *num) {
    *num = 0;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (dfs_status_board[i][j] != 0) {
                pos[*num][I] = i;
                pos[*num][J] = j;
                ++(*num);
            }
        }
    }
}


//remember that the larger the grade is, the better the status is for WHITE. and vice versa.
int alpha_beta_dfs(int my_player_side, int search_player_side, int search_depth, int alpha, int beta) {
    if (search_depth == 0) {
        return grade_estimate(my_player_side);
    }
    //the first one is i second one is j and the third one is the grade
    int possible_pos[MAX_POS][3] = {};
    int pos_num = 0;
    generate_possible_pos(possible_pos, &pos_num);
    int grade = 0;
    for (int k = 0; k < pos_num; ++k) {
        dfs_add_piece(possible_pos[k][I], possible_pos[k][J], my_player_side);

        possible_pos[k][GRADE] = alpha_beta_dfs(my_player_side, 0 - search_player_side, search_depth - 1, alpha, beta);

        //look back
        dfs_add_piece(possible_pos[k][I], possible_pos[k][J], VOID);
    }
    int best_pos = -1;
    for (int k = 0; k < pos_num; ++k) {
        int tmp = better(grade, possible_pos[k][GRADE], search_player_side);
        if (tmp != grade) {
            grade = tmp;
            best_pos = k;
        }
    }
    best_i = possible_pos[best_pos][I];
    best_j = possible_pos[best_pos][J];
    return grade;
}

#undef I
#undef J
#undef GRADE
#undef OFFSET