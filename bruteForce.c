//
// Created by les on 18-12-25.
//

#include "bruteForce.h"
/*

extern trie tr[TRIE_SIZE];


int8_t line_status[1073741824 + 5][15];

static int64_t MAXP = 28697814;

inline void brute_force() {
    int8_t line[BOARD_SIZE];
    SET0(line);
    int64_t cnt = 0;
    for (int8_t i0 = -1; i0 < 2; ++i0) {
        line[0] = i0;
        for (int8_t i1 = -1; i1 < 2; ++i1) {
            line[1] = i1;
            for (int8_t i2 = -1; i2 < 2; ++i2) {
                line[2] = i2;
                for (int8_t i3 = -1; i3 < 2; ++i3) {
                    line[3] = i3;
                    for (int8_t i4 = -1; i4 < 2; ++i4) {
                        line[4] = i4;
                        for (int8_t i5 = -1; i5 < 2; ++i5) {
                            line[5] = i5;
                            for (int8_t i6 = -1; i6 < 2; ++i6) {
                                line[6] = i6;
                                for (int8_t i7 = -1; i7 < 2; ++i7) {
                                    line[7] = i7;
                                    for (int8_t i8 = -1; i8 < 2; ++i8) {
                                        line[8] = i8;
                                        for (int8_t i9 = -1; i9 < 2; ++i9) {
                                            line[9] = i9;
                                            for (int8_t i10 = -1; i10 < 2; ++i10) {
                                                line[10] = i10;
                                                for (int8_t i11 = -1; i11 < 2; ++i11) {
                                                    line[11] = i11;
                                                    for (int8_t i12 = -1; i12 < 2; ++i12) {
                                                        line[12] = i12;
                                                        for (int8_t i13 = -1; i13 < 2; ++i13) {
                                                            line[13] = i13;
                                                            for (int8_t i14 = -1; i14 < 2; ++i14) {
                                                                line[14] = i14;
                                                                line_pattern_parse(line);
                                                                process_print(++cnt, MAXP);
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

inline void line_pattern_parse(int8_t line[]) {
    int cur = 0;
    int8_t player_side = WHITE;
    for (int k = -1; k <= BOARD_SIZE; ++k) {
        int ch = k == -1 || k == BOARD_SIZE ? 0 - player_side : line[k];
        while (tr[cur].trans[ch + COLOR_OFFSET] == -1 && cur != 0) {
            cur = tr[cur].fail;
        }
        cur = tr[cur].trans[ch + COLOR_OFFSET];
        cur = cur == -1 ? 0 : cur;
        int tmp = cur;
        while (tmp != 0 && tr[tmp].type != 0) {
            if (tr[tmp].type) {
                if (k != -1 && k != BOARD_SIZE) {
                    //line_status[get_index(line)][k];
                }
            }
            tmp = tr[tmp].fail;
        }
    }
    player_side = -player_side;
    for (int k = -1; k <= BOARD_SIZE; ++k) {
        int ch = k == -1 || k == BOARD_SIZE ? 0 - player_side : line[k];
        while (tr[cur].trans[ch + COLOR_OFFSET] == -1 && cur != 0) {
            cur = tr[cur].fail;
        }
        cur = tr[cur].trans[ch + COLOR_OFFSET];
        cur = cur == -1 ? 0 : cur;
        int tmp = cur;
        while (tmp != 0 && tr[tmp].type != 0) {
            if (tr[tmp].type) {
                if (k != -1 && k != BOARD_SIZE) {
                    //line_status[get_index(line)][k] = tr[tmp].type;
                }
            }
            tmp = tr[tmp].fail;
        }
    }
}

inline void process_print(int64_t cnt, int64_t MAX) {
    int8_t proc = (int8_t) (100 * cnt / MAX);
    printf("[");
    for (int8_t i = 0; i < proc - 1; ++i) {
        printf("=");
    }
    printf(">");
    for (int8_t i = proc; i < 100; ++i) {
        printf("_");
    }
    printf("]");
}

inline uint64_t get_index(int8_t line[]) {
    uint64_t ret = 0;
    uint8_t offset;
    for (int8_t i = 0; i < BOARD_SIZE; ++i) {
        offset = (uint8_t) line[i];
        ret += offset;
        ret <<= 2;
    }
    return ret;
}
 */