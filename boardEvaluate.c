//
// Created by Nine_yota on 2018-09-29.
//

#include "boardEvaluate.h"
#include "statusBoard.h"
#include "mathFunction.h"
#include "icld.h"
//
//#include "drawBoard.h"

#define TRIE_SIZE 50

//extern int best_i;
//extern int best_j;
extern int last_i;
extern int last_j;

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

typedef struct {
    //offset==1:
    int trans[3];
    int grade;
    int fail;
} trie;

trie tr[TRIE_SIZE];
#define STR_TO_RECOGNIZE 22
#define END 3
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
        tr[i].fail = -1;
        memset(tr[i].trans, -1, sizeof(tr[i].trans));
    }
    //build the trie here
    for (int i = 0; i < STR_TO_RECOGNIZE; ++i) {
        int cur = 0;
        for (int k = 0;; ++k) {
            if (tr[cur].trans[pattern[i][k] + OFFSET] == -1) {
                tr[cur].trans[pattern[i][k] + OFFSET] = ++cnt;
            }
            if (pattern[i][k + 1] == END) {
                tr[cnt].grade = grade[i];
                break;
            }
            cur = tr[cur].trans[pattern[i][k] + OFFSET];
        }
    }
    //build fail ptr here
    int root = 0;
    tr[root].fail = -1;
    int head = 0, tail = 1;
    int q[TRIE_SIZE] = {};
    q[head] = root;
    while (head != tail) {
        int tmp = q[head++];
        int p;
        for (int i = 0; i < 3; ++i) {
            if (tr[tmp].trans[i] != -1) {
                if (tmp == root)
                    tr[tr[tmp].trans[i]].fail = root;
                else {
                    p = tr[tmp].fail;
                    while (p != -1) {
                        if (tr[p].trans[i] != -1) {
                            tr[tr[tmp].trans[i]].fail = tr[p].trans[i];
                            break;
                        }
                        p = tr[p].fail;
                    }
                    if (p == -1)
                        tr[tr[tmp].trans[i]].fail = root;
                }
                q[tail++] = tr[tmp].trans[i];
            }
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
                if (tr[cur].grade == grade[i])
                    printf("match successfully\n");
                else
                    printf("match failed\n");
                break;
            }
            int p = tr[cur].trans[pattern[i][j] + OFFSET];
            if (p == -1) {
                cur = tr[cur].fail;
            } else {
                cur = p;
            }
            //printf("current 'cur': %d\n", cur);
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
            int ch = j == -1 || j == BOARD_SIZE ? 0 - player_side : status_board[i][j];
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
            int ch = i == -1 || i == BOARD_SIZE ? 0 - player_side : status_board[i][j];
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
            int ch = j == -1 || j == BOARD_SIZE ? 0 - player_side : oblique_lines_1[sum][j];
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
        for (int j = -1; j < BOARD_SIZE; ++j) {
            int ch = j == -1 || j == BOARD_SIZE ? 0 - player_side : oblique_lines_2[delta + BOARD_SIZE][j];
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


/*
 *  DFS starts here
 * */
#define MAX_POS 225
#define I 0
#define J 1
#define GRADE 2

void generate_possible_pos(drop_choice *drop_choice1, int *num) {
    *num = 0;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (dfs_status_board[i][j] == VOID) {
                drop_choice1[*num].i = i;
                drop_choice1[*num].j = j;
                ++(*num);
            }
        }
    }
}


//remember that the larger the grade is, the better the status is for WHITE. and vice versa.
drop_choice alpha_beta_dfs(int search_player_side, int search_depth) {
    drop_choice result;
    result.i = result.j = 0;
    result.grade = 0;

    if (search_depth == 0) {
        result.grade = grade_estimate(search_player_side) + grade_estimate(0 - search_player_side);
        return result;
    }
    //the first one is i second one is j and the third one is the grade

    drop_choice drop_choice1[MAX_POS];
    int pos_num = 0;
    //generate possible places to drop a piece
    generate_possible_pos(drop_choice1, &pos_num);

    //traverse the possible places
    if (search_player_side == WHITE) {
        result.grade = 0 - INF;
        for (int k = 0; k < pos_num; ++k) {
            dfs_add_piece(drop_choice1[k].i, drop_choice1[k].j, WHITE);
            drop_choice tmp;
            tmp = alpha_beta_dfs(0 - search_player_side, search_depth - 1);
            tmp.i = drop_choice1[k].i, tmp.j = drop_choice1[k].j;
            if (tmp.grade > result.grade) {
                result = tmp;
            }
            dfs_add_piece(tmp.i, tmp.j, VOID);
        }
        return result;
    } else {
        result.grade = INF;
        for (int k = 0; k < pos_num; ++k) {
            dfs_add_piece(drop_choice1[k].i, drop_choice1[k].j, BLACK);
            drop_choice tmp;
            tmp = alpha_beta_dfs(0 - search_player_side, search_depth - 1);
            tmp.i = drop_choice1[k].i, tmp.j = drop_choice1[k].j;
            if (tmp.grade < result.grade) {
                result = tmp;
            }
            dfs_add_piece(tmp.i, tmp.j, VOID);
        }
        return result;
    }
}

#undef I
#undef J
#undef GRADE
#undef OFFSET