#ifndef GOBANG_DRAWBOARD_H
#define GOBANG_DRAWBOARD_H

#include "icld.h"
#include "statusBoard.h"

#define CHAR_SIZE 4
#define CHAR_GAP 6

void output_board(int clear);

void dfs_output_board();

#endif //GOBANG_DRAWBOARD_H
