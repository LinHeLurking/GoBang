//
// Created by les on 18-10-10.
//

#include "AC_Automachine.h"
#include "icld.h"

trie tr[TRIE_SIZE];


// evaluate the grade of this status board using AC auto-machine

// values may need changing or the program would be too greedy. QAQ
#define FIVE_GRADE 1000000
#define CONTINUOUS_FOUR 100000
#define CONTINUOUS_THREE 1000
#define CONTINUOUS_TWO 100
#define CONTINUOUS_ONE 10

#define HALF_FOUR 50000
#define HALF_THREE 500
#define HALF_TWO 50

#define SPLIT_ALIVE_THREE 850
#define SPLIT_ALIVE_FOUR 35000


int grade[PATTERNS] = {
        //alive white
        FIVE_GRADE, CONTINUOUS_FOUR, CONTINUOUS_THREE, CONTINUOUS_TWO, CONTINUOUS_ONE,
        //half alive white
        FIVE_GRADE, FIVE_GRADE, HALF_FOUR, HALF_FOUR, HALF_THREE, HALF_THREE, HALF_TWO, HALF_TWO,
        //alive black
        -FIVE_GRADE, -CONTINUOUS_FOUR, -CONTINUOUS_THREE, -CONTINUOUS_TWO, -CONTINUOUS_ONE,
        //half alive black
        -FIVE_GRADE, -FIVE_GRADE, -HALF_FOUR, -HALF_FOUR, -HALF_THREE, -HALF_THREE, -HALF_TWO, -HALF_TWO,

        //dead five
        FIVE_GRADE, -FIVE_GRADE,

        //split alive four
        -SPLIT_ALIVE_FOUR, -SPLIT_ALIVE_FOUR, -SPLIT_ALIVE_FOUR, SPLIT_ALIVE_FOUR, SPLIT_ALIVE_FOUR, SPLIT_ALIVE_FOUR,

        //split alive three
        -SPLIT_ALIVE_THREE, -SPLIT_ALIVE_THREE, SPLIT_ALIVE_THREE, SPLIT_ALIVE_THREE
};
//cautions!! if you change this array, the grade[] also needs changing
int pattern[PATTERNS][MAX_PATTERN_LEN] = {

        //alive
        //white
        {VOID,  WHITE, WHITE, WHITE, WHITE, WHITE, VOID,  END},//
        {VOID,  WHITE, WHITE, WHITE, WHITE, VOID,  END},//
        {VOID,  WHITE, WHITE, WHITE, VOID,  END},//
        {VOID,  WHITE, WHITE, VOID,  END},//
        {VOID,  WHITE, VOID,  END},//
        //half alive
        //white
        {BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, VOID,  END},//
        {VOID,  WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, END},//
        {BLACK, WHITE, WHITE, WHITE, WHITE, VOID,  END},//
        {VOID,  WHITE, WHITE, WHITE, WHITE, BLACK, END},//
        {BLACK, WHITE, WHITE, WHITE, VOID,  END},//
        {VOID,  WHITE, WHITE, WHITE, BLACK, END},//
        {BLACK, WHITE, WHITE, VOID,  END},//
        {VOID,  WHITE, WHITE, BLACK, END},//

        //alive
        //black
        {VOID,  BLACK, BLACK, BLACK, BLACK, BLACK, VOID,  END},//
        {VOID,  BLACK, BLACK, BLACK, BLACK, VOID,  END},//
        {VOID,  BLACK, BLACK, BLACK, VOID,  END},//
        {VOID,  BLACK, BLACK, VOID,  END},//
        {VOID,  BLACK, VOID,  END},//
        //half alive
        //black
        {WHITE, BLACK, BLACK, BLACK, BLACK, BLACK, VOID,  END},//
        {VOID,  BLACK, BLACK, BLACK, BLACK, BLACK, WHITE, END},//
        {WHITE, BLACK, BLACK, BLACK, BLACK, VOID,  END},//
        {VOID,  BLACK, BLACK, BLACK, BLACK, WHITE, END},//
        {WHITE, BLACK, BLACK, BLACK, VOID,  END},//
        {VOID,  BLACK, BLACK, BLACK, WHITE, END},//
        {WHITE, BLACK, BLACK, VOID,  END},//
        {VOID,  BLACK, BLACK, WHITE, END},//

        //totally dead five
        {BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, END},//
        {WHITE, BLACK, BLACK, BLACK, BLACK, BLACK, WHITE, END},//

        //split
        //alive four
        {BLACK, BLACK, BLACK, VOID,  BLACK, END},//
        {BLACK, BLACK, VOID,  BLACK, BLACK, END},//
        {BLACK, VOID,  BLACK, BLACK, BLACK, END},//
        {WHITE, WHITE, WHITE, VOID,  WHITE, END},//
        {WHITE, WHITE, VOID,  WHITE, WHITE, END},//
        {WHITE, VOID,  WHITE, WHITE, WHITE, END},//

        // alive three
        {VOID,  BLACK, VOID,  BLACK, BLACK, VOID,  END},//
        {VOID,  BLACK, BLACK, VOID,  BLACK, VOID,  END},//
        {VOID,  WHITE, VOID,  WHITE, WHITE, VOID,  END},//
        {VOID,  WHITE, WHITE, VOID,  WHITE, VOID,  END}//
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
    for (int i = 0; i < PATTERNS; ++i) {
        insert(pattern[i], grade[i]);
    }
    build_AC_fail();
}

#undef FIVE_GRADE
#undef CONTINUOUS_ONE
#undef CONTINUOUS_TWO
#undef CONTINUOUS_THREE
#undef CONTINUOUS_FOUR
#undef HALF_TWO
#undef HALF_THREE
#undef HALF_FOUR
#undef SPLIT_ALIVE_FOUR
#undef SPLIT_ALIVE_THREE