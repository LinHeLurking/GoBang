//
// Created by les on 18-10-10.
//

#ifndef GOBANG_AC_AUTOMACHINE_H
#define GOBANG_AC_AUTOMACHINE_H

#include "icld.h"
#include "customTypes.h"

//CAUTIONS!!! this PATTERNS must be exactly the same as the number of patterns
#define PATTERNS 43
#define END 107

void AC_build();

// values may need changing or the program would be too greedy. QAQ
#define FIVE_GRADE 500000
#define CONTINUOUS_FOUR 20000
#define CONTINUOUS_THREE 2000
#define CONTINUOUS_TWO 100
#define CONTINUOUS_ONE 10

#define HALF_FOUR 10000
#define HALF_THREE 800
#define HALF_TWO 50

#define SPLIT_ALIVE_THREE 1500
#define SPLIT_ALIVE_FOUR_WITHOUT3 10000
#define SPLIT_ALIVE_FOUR_WITH3 8000

#define LONG_CONTINUOUS 100000


#endif //GOBANG_AC_AUTOMACHINE_H
