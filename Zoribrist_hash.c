//
// Created by Nine_yota on 2018-10-18.
//

#include "Zoribrist_hash.h"

unsigned int hash_key[2][BOARD_SIZE][BOARD_SIZE];
long long cache_total_grade[2][CACHE_SIZE];
long long cache_col_grade[2][CACHE_SIZE];
long long cache_row_grade[2][CACHE_SIZE];
long long cache_oblique_sum_grade[2][CACHE_SIZE];
long long cache_oblique_delta_grade[2][CACHE_SIZE];
int cache_record_step[CACHE_SIZE];
unsigned int hash;

void hash_init() {
    SET0(cache_total_grade);
    SET0(cache_row_grade);
    SET0(cache_col_grade);
    SET0(cache_oblique_sum_grade);
    SET0(cache_oblique_delta_grade);
    memset(cache_record_step, -1, sizeof(cache_record_step));
    srand((unsigned int) time(NULL));
    for (int m = 0; m < 2; ++m) {
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                hash_key[m][i][j] = 0;
                for (int k = 0; k < 4; ++k) {
                    hash_key[m][i][j] += (unsigned int) rand() % (1U << 8U);
                    hash_key[m][i][j] <<= 8U;
                }
            }
        }
    }

    //TODO: high quality random number needed!!!

#ifdef HASH_DEBUG
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            printf("%llu\t", hash_key[i][j] % CACHE_SIZE);
        }
        printf("\n");
    }
    printf("\n");

#endif
    hash = 0;
    for (int k = 0; k < 4; ++k) {
        hash += (unsigned int) rand() % (1U << 8U);
        hash <<= 8U;
    }
    hash_check();
}

void hash_check() {
    //TODO: found hash collision here!!!

    int cnt[CACHE_SIZE];
    int collision_i[BOARD_SIZE], collision_j[BOARD_SIZE];

    for (int l = 0; l < 2; ++l) {
        int index = 0;
        int collision = 0;
        SET0(collision_i);
        SET0(collision_j);
        SET0(cnt);
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                ++cnt[hash_key[l][i][j] % CACHE_SIZE];
                if (cnt[hash_key[l][i][j] % CACHE_SIZE] > 1) {
                    ++collision;
                    collision_i[index] = i;
                    collision_j[index] = j;
                    ++index;
                }
            }
        }

        while (collision) {
            for (int k = 0; k < index; ++k) {
                unsigned int key = hash_key[l][collision_i[k]][collision_j[k]];
                while (cnt[key % CACHE_SIZE]) {
                    key = (key + 23);
                }
                --cnt[key % CACHE_SIZE];
                --collision;
            }
        }
    }
}
