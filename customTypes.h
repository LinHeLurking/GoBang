//
// Created by les on 18-12-22.
//

#ifndef GOBANG_CUSTOMTYPES_H
#define GOBANG_CUSTOMTYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include "AC_Automachine.h"

#define MAX_PATTERN_LEN 20

#define PATTERN_TYPES 28

enum {
    /* axw: alive x piece(s) of white
     * hyb: half alive y piece(s) of black
     * saxw: split alive x piece(s) of white
     * sa4n3w: split alive 4 without 3 of white
     * l6b: long continue of black
     **/

    //enum starts at 1 and ends at 28!!

    a5w = 1,
    a5b,
    a4w,
    a4b,
    a3w,
    a3b,
    a2w,
    a2b,
    a1w,
    a1b,

    h5w,
    h5b,
    h4w,
    h4b,
    h3w,
    h3b,
    h2w,
    h2b,

    d5w,
    d5b,

    sa3w,
    sa3b,
    sa4n3w,
    sa4n3b,
    sa4w3w,
    sa4w3b,

    l6w,
    l6b
};

typedef struct {
    uint8_t type;
    int8_t pattern[MAX_PATTERN_LEN];
    int32_t back[MAX_PATTERN_LEN];
    int64_t grade;
} __AC_node;


typedef struct {
    int8_t trans[3];
    int32_t grade;
    //uint8_t type;
    int8_t fail;
    __AC_node nd;
} trie;

typedef struct {
    //these are used to record the board status
    int32_t board[BOARD_SIZE][BOARD_SIZE];

    uint8_t row_type[BOARD_SIZE][PATTERN_TYPES + 5];
    uint8_t col_type[BOARD_SIZE][PATTERN_TYPES + 5];
    uint8_t oblique_sum_type[BOARD_SIZE * 2][PATTERN_TYPES + 5];
    uint8_t oblique_delta_type[BOARD_SIZE * 2][PATTERN_TYPES + 5];
    uint8_t total_type[PATTERN_TYPES + 5];

    int64_t row_increment[BOARD_SIZE][BOARD_SIZE][PATTERN_TYPES + 5];
    int64_t col_increment[BOARD_SIZE][BOARD_SIZE][PATTERN_TYPES + 5];
    int64_t oblique_sum_increment[BOARD_SIZE * 2][BOARD_SIZE][PATTERN_TYPES + 5];
    int64_t oblique_delta_increment[BOARD_SIZE * 2][BOARD_SIZE][PATTERN_TYPES + 5];
    //int64_t total_increment;

    //these are used to record the last place and the total steps
    int32_t steps;

    int8_t __WHITE, __BLACK;

} boardStatus;

typedef struct {
    int i, j, player;
} drop_record;

typedef struct {
    int64_t grade;
    int i, j;
    //this is used for searching
    int player;
    int64_t grade_estimate;
} drop_choice;

typedef struct {
    int8_t type[PATTERN_TYPES];
} __line_status;

#endif //GOBANG_CUSTOMTYPES_H
