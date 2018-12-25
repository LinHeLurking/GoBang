//
// Created by les on 18-10-10.
//

#ifndef GOBANG_AC_AUTOMACHINE_H
#define GOBANG_AC_AUTOMACHINE_H

#include "icld.h"
#include "customTypes.h"

//CAUTIONS!!! this PATTERNS must be exactly the same as the number of patterns
#define PATTERNS 43
#define END 7

void AC_build();

// values may need changing or the program would be too greedy. QAQ
// make sure that split alive grades are mot the same as continuous ones.
#define FIVE_GRADE 100000
#define CONTINUOUS_FOUR 10000
#define CONTINUOUS_THREE 1000
#define CONTINUOUS_TWO 100
#define CONTINUOUS_ONE 10

#define HALF_FOUR 5000
#define HALF_THREE 500
#define HALF_TWO 50

#define SPLIT_ALIVE_THREE 800
#define SPLIT_ALIVE_FOUR_WITHOUT3 3002
#define SPLIT_ALIVE_FOUR_WITH3 3000

#define LONG_CONTINUOUS 100000


#endif //GOBANG_AC_AUTOMACHINE_H
