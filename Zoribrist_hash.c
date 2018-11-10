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
unsigned long long hash;

void hash_init() {
    SET0(cache_total_grade);
    SET0(cache_row_grade);
    SET0(cache_col_grade);
    SET0(cache_oblique_sum_grade);
    SET0(cache_oblique_delta_grade);
    memset(cache_record_step, -1, sizeof(cache_record_step));
    srand((unsigned int) time(NULL));
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            hash_key[i][j] = 0;
            for (int k = 0; k < 8; ++k) {
                hash_key[i][j] += (unsigned long long) rand() % (1U << 8U);
                hash_key[i][j] <<= 8;
            }
        }
    }
    hash = 0;
    for (int k = 0; k < 8; ++k) {
        hash += (unsigned long long) rand() % (1U << 8U);
        hash <<= 8;
    }
}


