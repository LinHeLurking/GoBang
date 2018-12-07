//
// Created by les on 18-10-10.
//

#include "AC_Automachine.h"
#include "icld.h"

trie tr[TRIE_SIZE];


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

#define FIVE_GRADE 100000

int grade[STR_TO_RECOGNIZE] = {FIVE_GRADE, 10000, 1000, 100, 10, FIVE_GRADE, FIVE_GRADE, 1100, 1100, 100, 100, 10, 10,
                               -FIVE_GRADE, -10000, -1000, -100, -10, -100000, -100000, -1100, -1100, -100, -100, -10,
                               -10, FIVE_GRADE, -FIVE_GRADE, -1000, -1000, 1000, 1000, -10000, -10000, 10000, 10000};
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
        {WHITE, BLACK, BLACK, BLACK, BLACK, BLACK, WHITE, END},//-100000

        //special continue
        {VOID,  BLACK, BLACK, BLACK, VOID,  BLACK, VOID,  END},//-1100+100=-1000
        {VOID,  BLACK, VOID,  BLACK, BLACK, BLACK, VOID,  END},//-1000
        {VOID,  WHITE, WHITE, WHITE, VOID,  WHITE, VOID,  END},//1100-100=1000
        {VOID,  WHITE, VOID,  WHITE, WHITE, WHITE, VOID,  END},//1100-100=1000

        {BLACK, BLACK, BLACK, VOID,  BLACK, BLACK, END},//-10000
        {BLACK, BLACK, VOID,  BLACK, BLACK, BLACK, END},//-10000
        {WHITE, WHITE, WHITE, VOID,  WHITE, WHITE, END},//10000
        {WHITE, WHITE, VOID,  WHITE, WHITE, WHITE, END}//10000

};


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

#undef FIVE_GRADE