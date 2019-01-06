//
// Created by Nine_yota on 2018-09-29.
//

#ifndef GOBANG_ICLD_H
#define GOBANG_ICLD_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#define BOARD_SIZE 15
#define BLACK (-1)
#define WHITE 1
#define VOID 0
#define HUMAN_VS_HUMAN 0
#define HUMAN_VS_COMPUTER 1
#define COMPUTER_VS_COMPUTER 2

#ifndef DFS_MAX_DEPTH
#define DFS_MAX_DEPTH 12
#endif

#ifndef TIME_LIMIT
#define TIME_LIMIT 10
#endif

#define INF 20000000000007
#define CACHE_SIZE 10009897
#define HASH (hash%CACHE_SIZE)
#define COMPUTER_FIRST 2
#define HUMAN_FIRST 1

#ifndef NONE_DEBUG_DRAW
#define DEBUG_DRAW
#endif

#define USE_HASH
//#define DFS_BOARD_DEBUG
//#define PRUNE_DEBUG
//#define PLAYER_SIDE_DEBUG
//#define HASH_DEBUG
//#define RECORD_DEBUG
//#define BAN_DEBUG

#define TRIE_SIZE 1000
#define STATUS_NUM 3


#define COLOR_OFFSET 1

#define GRADE_DEBUG printf("Direct grade estimate for BLACK: %lld\n",grade_estimate(BLACK));printf("Direct grade estimate for WHITE: %lld\n",grade_estimate(WHITE));

#define SET0(a) memset(a,0,sizeof(a))

#define PLAYER_IN_LINE player_side==1?1:0

#endif //GOBANG_ICLD_H
