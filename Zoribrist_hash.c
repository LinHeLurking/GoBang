//
// Created by Nine_yota on 2018-10-18.
//

#include "Zoribrist_hash.h"

unsigned long long hash_key[BOARD_SIZE][BOARD_SIZE];
int cache_grade[2][CACHE_SIZE];
int cache_grade_found_depth[2][CACHE_SIZE];

void hash_init() {
    SET0(cache_grade);
    SET0(cache_grade_found_depth);
    srand((unsigned int) time(NULL));
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            hash_key[i][j] = 0;
            for (int k = 0; k < 4; ++k) {
                unsigned long long tmp = (unsigned long long) rand() % (1 << 16);
                hash_key[i][j] += tmp;
                hash_key[i][j] <<= 16;
            }
        }
    }
}
