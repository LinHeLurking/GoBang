//
// Created by Nine_yota on 2018-09-29.
//

#include "mathFunction.h"
#include "icld.h"

int min(int x, int y) {
    return x < y ? x : y;
}

int max(int x, int y) {
    return x < y ? y : x;
}

int better(int x, int y, int player_side) {
    if (player_side == WHITE)
        return max(x, y);
    else
        return min(x, y);
}

