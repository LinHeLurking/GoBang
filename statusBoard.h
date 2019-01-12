#ifndef GOBANG_STATUSBOARD_H
#define GOBANG_STATUSBOARD_H

#include "icld.h"
#include "AC_Automachine.h"
#include "mathFunc.h"
#include "boardEvaluate.h"
#include "customTypes.h"

int add_piece(int i, int j, int piece_color);

int dfs_add_piece(int i, int j, int piece_color);

void status_init();

#endif //GOBANG_STATUSBOARD_H
