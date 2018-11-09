//
// Created by Nine_yota on 2018-10-18.
//

#include "Zoribrist_hash.h"

unsigned long long hash_key[BOARD_SIZE][BOARD_SIZE];
long long cache_total_grade[2][CACHE_SIZE];
long long cache_col_grade[2][CACHE_SIZE];
long long cache_row_grade[2][CACHE_SIZE];
long long cache_oblique_sum_grade[2][CACHE_SIZE];
long long cache_oblique_delta_grade[2][CACHE_SIZE];
int cache_record_step[CACHE_SIZE];


void hash_init() {
    SET0(cache_total_grade);
    SET0(cache_row_grade);
    SET0(cache_col_grade);
    SET0(cache_oblique_sum_grade);
    SET0(cache_oblique_delta_grade);
    memset(cache_record_step,-1, sizeof(cache_record_step));
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
