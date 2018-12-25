//
// Created by Nine_yota on 2018-09-29.
//


#ifndef GOBANG_STATUSBOARD_H
#define GOBANG_STATUSBOARD_H

#include "icld.h"
#include "AC_Automachine.h"
#include "Zoribrist_hash.h"
#include "mathFunc.h"
#include "boardEvaluate.h"
#include "customTypes.h"

int add_piece(int i, int j, int piece_color);

int dfs_add_piece(int i, int j, int piece_color);

void status_init();

int32_t *board_acess(int32_t index1, int32_t index2, int32_t line_type);


#endif //GOBANG_STATUSBOARD_H
