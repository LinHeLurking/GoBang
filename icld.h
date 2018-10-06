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
#define DFS_DEPTH 2
#define INF 1000000007
#define DEBUG
#define DFS_BOARD_DEBUG

#define GRADE_DEBUG printf("Direct grade estimate for BLACK: %d\n",grade_estimate(BLACK));printf("Direct grade estimate for WHITE: %d\n",grade_estimate(WHITE));

typedef struct {
    int grade;
    int i, j;
} drop_choice;

#endif //GOBANG_ICLD_H
