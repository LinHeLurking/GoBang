//
// Created by Nine_yota on 2018-09-30.
//

#include "mathFunction.h"

int min(int x, int y) {
    if (x < y)
        return x;
    else
        return y;
}

int max(int x, int y) {
    if (x < y)
        return y;
    else
        return x;
}

drop_choice *min_drop_choice(drop_choice *a, drop_choice *b) {
    if (a->grade > b->grade) {
        return b;
    } else {
        return a;
    }
}

drop_choice *max_drop_choice(drop_choice *a, drop_choice *b) {
    if (a->grade > b->grade) {
        return a;
    } else {
        return b;
    }
}
