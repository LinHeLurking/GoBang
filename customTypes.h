/*
 * almost all the custom types are defined in this file:
 *      pattern enum
 *      nodes in AC auto machine
 *      trie of the AC auto machine
 *      struct of the board status
 *      struct of a choice
 * */

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
     * s4n3w: split alive 4 without 3 of white
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
    s4w,
    s4b,

    //special types that does not influence grade estimate but influence type recognition
    s4w_with3,
    s4b_with3,

    l6w,
    l6b
};

typedef struct {
    int type;
    int pattern[MAX_PATTERN_LEN];
    int back[MAX_PATTERN_LEN];
    int blank_type[MAX_PATTERN_LEN];
    long long grade;
} __AC_node;


typedef struct {
    int trans[3];
    int grade;
    //uint8_t type;
    int fail;
    __AC_node nd;
} trie;

typedef struct {
    //these are used to record the board status
    int board[BOARD_SIZE][BOARD_SIZE];

    int row_type[BOARD_SIZE][PATTERN_TYPES + 5];
    int col_type[BOARD_SIZE][PATTERN_TYPES + 5];
    int oblique_sum_type[BOARD_SIZE * 2][PATTERN_TYPES + 5];
    int oblique_delta_type[BOARD_SIZE * 2][PATTERN_TYPES + 5];
    int total_type[PATTERN_TYPES + 5];

    long long row_increment[BOARD_SIZE][BOARD_SIZE][PATTERN_TYPES + 5];
    long long col_increment[BOARD_SIZE][BOARD_SIZE][PATTERN_TYPES + 5];
    long long oblique_sum_increment[BOARD_SIZE * 2][BOARD_SIZE][PATTERN_TYPES + 5];
    long long oblique_delta_increment[BOARD_SIZE * 2][BOARD_SIZE][PATTERN_TYPES + 5];
    long long total_increment[PATTERN_TYPES + 5];

    //these are used to record the last place and the total steps
    int steps;

} boardStatus;

typedef struct {
    int i, j, player;
} drop_record;

typedef struct {
    int64_t grade;
    int i, j;
    //this is used for searching
    int64_t grade_estimate;

    bool broken_search_flag;
} drop_choice;


typedef struct {
    clock_t start_clock;
    clock_t end_clock;
} timer;

#endif //GOBANG_CUSTOMTYPES_H
