//
// Created by Nine_yota on 2018-09-29.
//

#ifndef GOBANG_ICLD_H
#define GOBANG_ICLD_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define BOARD_SIZE 15
#define CHAR_SIZE 4
#define GAP 6
#define BLACK (-1)
#define WHITE 1
#define VOID 0
#define HUMAN_VS_HUMAN 0
#define HUMAN_VS_COMPUTER 1
#define DFS_DEPTH 4
#define INF 20000000000007
#define DEBUG
//#define DFS_BOARD_DEBUG
#define PRUNE_DEBUG
#define PLYAER_SIDE_DEBUG

#define TRIE_SIZE 100
#define STATUS_NUM 3
#define STR_TO_RECOGNIZE 28
#define END 7
#define MAX_STR_SIZE 8
#define OFFSET 1

#define GRADE_DEBUG printf("Direct grade estimate for BLACK: %lld\n",grade_estimate(BLACK));printf("Direct grade estimate for WHITE: %lld\n",grade_estimate(WHITE));

#define SET0(a) memset(a,0,sizeof(a))

#define PLAYER_IN_LINE player_side==1?1:0

typedef struct {
    long long grade;
    int i, j;
    //this is used for searching
    long double grade_estimate;
} drop_choice;

typedef struct {
    int trans[3];
    int grade;
    int fail;
} trie;

#endif //GOBANG_ICLD_H
