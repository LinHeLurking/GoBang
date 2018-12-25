//
// Created by les on 18-10-10.
//

#include "AC_Automachine.h"
#include "icld.h"

trie tr[TRIE_SIZE];

__AC_node AC_node[PATTERNS] = {
        {
                a5w,
                {VOID,  WHITE, WHITE, WHITE, WHITE, WHITE, VOID,  END},
                FIVE_GRADE
        },
        {
                a4w,
                {VOID,  WHITE, WHITE, WHITE, WHITE, VOID,  END},
                CONTINUOUS_FOUR
        },
        {
                a3w,
                {VOID,  WHITE, WHITE, WHITE, VOID,  END},
                CONTINUOUS_THREE
        },
        {
                a2w,
                {VOID,  WHITE, WHITE, VOID,  END},
                CONTINUOUS_TWO
        },
        {
                a1w,
                {VOID,  WHITE, VOID,  END},
                CONTINUOUS_ONE
        },
        {
                a5b,
                {VOID,  BLACK, BLACK, BLACK, BLACK, BLACK, VOID,  END},
                -FIVE_GRADE
        },
        {
                a4b,
                {VOID,  BLACK, BLACK, BLACK, BLACK, VOID,  END},
                -CONTINUOUS_FOUR
        },
        {
                a3b,
                {VOID,  BLACK, BLACK, BLACK, VOID,  END},
                -CONTINUOUS_THREE
        },
        {
                a2b,
                {VOID,  BLACK, BLACK, VOID,  END},
                -CONTINUOUS_TWO
        },
        {
                a1b,
                {VOID,  BLACK, VOID,  END},
                -CONTINUOUS_ONE
        },
        {
                h5w,
                {BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, VOID,  END},
                FIVE_GRADE
        },
        {
                h5w,
                {VOID,  WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, END},
                FIVE_GRADE
        },
        {
                h4w,
                {BLACK, WHITE, WHITE, WHITE, WHITE, VOID,  END},
                HALF_FOUR
        },
        {
                h4w,
                {VOID,  WHITE, WHITE, WHITE, WHITE, BLACK, END},
                HALF_FOUR
        },
        {
                h3w,
                {BLACK, WHITE, WHITE, WHITE, VOID,  END},
                HALF_THREE
        },
        {
                h3w,
                {VOID,  WHITE, WHITE, WHITE, BLACK, END},
                HALF_THREE
        },
        {
                h2w,
                {BLACK, WHITE, WHITE, VOID,  END},
                HALF_TWO
        },
        {
                h2w,
                {VOID,  WHITE, WHITE, BLACK, END},
                HALF_TWO
        },
        {
                h5b,
                {WHITE, BLACK, BLACK, BLACK, BLACK, BLACK, VOID,  END},
                -FIVE_GRADE
        },
        {
                h5b,
                {VOID,  BLACK, BLACK, BLACK, BLACK, BLACK, WHITE, END},
                -FIVE_GRADE
        },
        {
                h4b,
                {WHITE, BLACK, BLACK, BLACK, BLACK, VOID,  END},
                -HALF_FOUR
        },
        {
                h4b,
                {VOID,  BLACK, BLACK, BLACK, BLACK, WHITE, END},
                -HALF_FOUR
        },
        {
                h3b,
                {WHITE, BLACK, BLACK, BLACK, VOID,  END},
                -HALF_THREE
        },
        {
                h3b,
                {VOID,  BLACK, BLACK, BLACK, WHITE, END},
                -HALF_THREE
        },
        {
                h2b,
                {WHITE, BLACK, BLACK, VOID,  END},
                -HALF_TWO
        },
        {
                h2b,
                {VOID,  BLACK, BLACK, WHITE, END},
                -HALF_TWO
        },
        {
                d5w,
                {BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, END},
                FIVE_GRADE
        },
        {
                d5b,
                {WHITE, BLACK, BLACK, BLACK, BLACK, BLACK, WHITE, END},
                -FIVE_GRADE
        },
        {
                sa3w,
                {VOID,  WHITE, VOID,  WHITE, WHITE, VOID,  END},
                SPLIT_ALIVE_THREE
        },
        {
                sa3w,
                {VOID,  WHITE, WHITE, VOID,  WHITE, VOID,  END},
                SPLIT_ALIVE_THREE
        },
        {
                sa3b,
                {VOID,  BLACK, VOID,  BLACK, BLACK, VOID,  END},
                -SPLIT_ALIVE_THREE
        },
        {
                sa3b,
                {VOID,  BLACK, BLACK, VOID,  BLACK, VOID,  END},
                -SPLIT_ALIVE_THREE
        },
        {
                sa4n3w,
                {BLACK, WHITE, WHITE, WHITE, VOID,  WHITE, END},
                SPLIT_ALIVE_FOUR_WITHOUT3
        },
        {
                sa4n3w,
                {WHITE, VOID,  WHITE, WHITE, WHITE, BLACK, END},
                SPLIT_ALIVE_FOUR_WITHOUT3
        },
        {
                sa4n3w,
                {WHITE, WHITE, VOID,  WHITE, WHITE, END},
                SPLIT_ALIVE_FOUR_WITHOUT3
        },
        {
                sa4n3b,
                {WHITE, BLACK, BLACK, BLACK, VOID,  BLACK, END},
                -SPLIT_ALIVE_FOUR_WITHOUT3
        },
        {
                sa4n3b,
                {BLACK, VOID,  BLACK, BLACK, BLACK, WHITE, END},
                -SPLIT_ALIVE_FOUR_WITHOUT3
        },
        {
                sa4n3b,
                {BLACK, BLACK, VOID, BLACK, BLACK, END},
                -SPLIT_ALIVE_FOUR_WITHOUT3
        },
        {
                sa4w3w,
                {VOID,  WHITE, WHITE, WHITE, VOID,  WHITE, END},
                SPLIT_ALIVE_FOUR_WITH3
        },
        {
                sa4w3w,
                {WHITE, VOID,  WHITE, WHITE, WHITE, VOID,  END},
                SPLIT_ALIVE_FOUR_WITH3
        },
        {
                sa4w3b,
                {VOID,  BLACK, BLACK, BLACK, VOID,  BLACK, END},
                -SPLIT_ALIVE_FOUR_WITH3
        },
        {
                sa4w3b,
                {BLACK, VOID,  BLACK, BLACK, BLACK, VOID,  END},
                -SPLIT_ALIVE_FOUR_WITH3
        },
        {
                l6b,
                {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, END},
                LONG_CONTINUOUS
        }
};
uint8_t root = 0;
uint8_t count = 0;

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

void insert(int8_t *s, uint8_t type_code) {
    int p = root;
    for (int8_t *c = s; *c != END; ++c) {
        int8_t tmp = (*c);
        if (tr[p].trans[tmp + COLOR_OFFSET] == -1) {
            tr[p].trans[tmp + COLOR_OFFSET] = ++count;
        }
        p = tr[p].trans[tmp + COLOR_OFFSET];
        if ((*(c + 1)) == END)
            tr[p].type = type_code;
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
        insert(AC_node[i].pattern, AC_node[i].type);
    }
    build_AC_fail();
}
