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
#define DFS_MAX_DEPTH 10
#endif

#ifndef TIME_LIMIT
#define TIME_LIMIT 14
#endif

#define INF 20000000000007
#define COMPUTER_FIRST 2
#define HUMAN_FIRST 1

#ifndef NONE_DEBUG_DRAW
#define DEBUG_DRAW
#endif

#define TRIE_SIZE 1000
#define STATUS_NUM 3

#define COLOR_OFFSET 1

#define SET0(a) memset(a,0,sizeof(a))

#endif //GOBANG_ICLD_H
